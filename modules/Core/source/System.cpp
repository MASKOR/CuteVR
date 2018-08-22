/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <openvr.h>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVector>

#include <CuteVR/Components/Geometry/Cube.hpp>
#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Internal/Property.hpp>
#include <CuteVR/Interface/EventHandler.hpp>
#include <CuteVR/Interface/TrackingHandler.hpp>
#include <CuteVR/DeviceServer.hpp>
#include <CuteVR/System.hpp>

using namespace CuteVR;
using Configurations::Core::Feature;
using Configurations::feature;
using Extension::Optional;
using Extension::Trilean;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<QSharedPointer<Device>>();
            qRegisterMetaType<QMap<Identifier, QSharedPointer<Device>>>();
            qRegisterMetaType<System::Equipment>();
            qRegisterMetaType<QMap<Identifier, System::Equipment>>();
            qRegisterMetaType<System::Cell>();
            qRegisterMetaType<QMap<Identifier, System::Cell>>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR {
    bool operator==(System::Equipment const &left, System::Equipment const &right) {
        return (left.identifier == right.identifier) &&
               (left.headMountedDisplays == right.headMountedDisplays) &&
               (left.headMountedAudios == right.headMountedAudios) &&
               (left.controllers == right.controllers);
    }

    bool operator!=(System::Equipment const &left, System::Equipment const &right) {
        return !(left == right);
    }

    bool operator==(System::Cell const &left, System::Cell const &right) {
        return (left.identifier == right.identifier) &&
               (left.trackingReferences == right.trackingReferences) &&
               (left.trackers == right.trackers) &&
               (left.equipments == right.equipments) &&
               // FIXME: no == operator for category (left.boundary == right.boundary) &&
               (left.globalTransform == right.globalTransform);
    }

    bool operator!=(System::Cell const &left, System::Cell const &right) {
        return !(left == right);
    }
}

class System::Private {
public: // types
    class SystemEventProvider :
            public Interface::EventHandler {
    public: // destructor
        explicit SystemEventProvider(System *that, std::function<void(vr::VREvent_t const &)> callback) :
                that{that},
                callback{std::move(callback)} {}

        ~SystemEventProvider() override = default;

    public: // methods
        bool handleEvent(void const *event, void const *) override {
            auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
            if (!theEvent) {
                return false;
            }
            switch (theEvent->eventType) {
                case vr::VREvent_TrackedDeviceActivated:
                case vr::VREvent_TrackedDeviceDeactivated: {
                    callback(vr::VREvent_t{*theEvent});
                    return true;
                }
                case vr::VREvent_TrackedDeviceUpdated:
                case vr::VREvent_PropertyChanged:
                    // TODO: future needs
                    return false;
                case vr::VREvent_Quit: {
                    qInfo("OpenVR requested quit; acknowledging quit.");
                    DriverServer::synchronized([&] {
                        vr::VRSystem()->AcknowledgeQuit_Exiting();
                    }, Trilean::yes);
                    return true;
                }
                case vr::VREvent_DriverRequestedQuit: {
                    qInfo("DriverServer requested quit; acknowledging quit.");
                    DriverServer::synchronized([&] {
                        vr::VRSystem()->AcknowledgeQuit_Exiting();
                    }, Trilean::yes);
                    return true;
                }
                case vr::VREvent_QuitAcknowledged:qInfo("OpenVR received quit acknowledgement; beginning shutdown.");
                    QtConcurrent::run([]() { DriverServer::instance().destroy(); });
                    return true;
                default:
                    // TODO: handle more events?
                    return false;
            }
        }

    private: // variables
        System *that{nullptr};
        std::function<void(vr::VREvent_t const &)> callback{};
    };

    class SystemTrackingProvider :
            public Interface::TrackingHandler {
    public: // destructor
        ~SystemTrackingProvider() override = default;

    public: // methods
        bool handleTracking(void const *) override {
            return false; // FIXME: currently unused, maybe useful
        }
    };

public: // constructor
    explicit Private(System *that) :
            that{that} {
    }

public: // methods
    void queryDevices() {
        // TODO: move device query from initialize to here
    }

    void handleDeviceActivated(Identifier const identifier) {
        // FIXME: move cell and equipment changes to own functions, be more dry
        QWriteLocker{&updateLock};
        auto either{DeviceServer::create(identifier)};
        if (either.isRight()) {
            auto device{either.right()};
            device->initialize();
            device->update();
            devicesCurrent.insert(device->identifier, device);
            emit that->devicesChanged(devicesCurrent);
            emit that->deviceChanged(device->identifier, device);
            if (ConfigurationServer::isEnabled(feature(Feature::cell)).right(false)) {
                if (cellsCurrent.empty()) {
                    cellsCurrent.insert(0, {});
                    emit that->cellsChanged(cellsCurrent);
                    emit that->cellChanged(0, cellsCurrent.value(0));
                }
                switch (device->category()) {
                    case Device::Category::trackingReference: {
                        cellsCurrent[0].trackingReferences.insert(device->identifier);
                        emit that->cellsChanged(cellsCurrent);
                        emit that->cellChanged(0, cellsCurrent.value(0));
                        break;
                    }
                    case Device::Category::tracker: {
                        cellsCurrent[0].trackers.insert(device->identifier);
                        emit that->cellsChanged(cellsCurrent);
                        emit that->cellChanged(0, cellsCurrent.value(0));
                        break;
                    }
                    default: break;
                }
            }

            if (ConfigurationServer::isEnabled(feature(Feature::equipment)).right(false)) {
                if (equipmentsCurrent.empty()) {
                    equipmentsCurrent.insert(0, {});
                    if (cellsCurrent.contains(0)) {
                        cellsCurrent[0].equipments.insert(0);
                        emit that->cellsChanged(cellsCurrent);
                        emit that->cellChanged(0, cellsCurrent.value(0));
                    }
                    emit that->equipmentsChanged(equipmentsCurrent);
                    emit that->equipmentChanged(0, equipmentsCurrent.value(0));
                }
                switch (device->category()) {
                    case Device::Category::headMountedDisplay: {
                        equipmentsCurrent[0].headMountedDisplays.insert(device->identifier);
                        emit that->equipmentsChanged(equipmentsCurrent);
                        emit that->equipmentChanged(0, equipmentsCurrent.value(0));
                        break;
                    }
                    case Device::Category::headMountedAudio: {
                        equipmentsCurrent[0].headMountedAudios.insert(device->identifier);
                        emit that->equipmentsChanged(equipmentsCurrent);
                        emit that->equipmentChanged(0, equipmentsCurrent.value(0));
                        break;
                    }
                    case Device::Category::controller: {
                        equipmentsCurrent[0].controllers.insert(device->identifier);
                        emit that->equipmentsChanged(equipmentsCurrent);
                        emit that->equipmentChanged(0, equipmentsCurrent.value(0));
                        break;
                    }
                    default: break;
                }
            }
            current = false;
        }
    }

    void handleDeviceDeactivated(Identifier const identifier) {
        QWriteLocker{&updateLock};
        if (devicesCurrent.contains(identifier)) {
            devicesCurrent[identifier]->destroy();
            devicesCurrent.remove(identifier);
            emit that->devicesChanged(devicesCurrent);
            emit that->deviceChanged(identifier, QSharedPointer<Device>{nullptr});
        }
        if (cellsCurrent.contains(0)) {
            cellsCurrent[0].trackers.remove(identifier);
            cellsCurrent[0].trackingReferences.remove(identifier);
            emit that->cellsChanged(cellsCurrent);
            emit that->cellChanged(0, cellsCurrent.value(0));
        }
        if (equipmentsCurrent.contains(0)) {
            equipmentsCurrent[0].controllers.remove(identifier);
            equipmentsCurrent[0].headMountedDisplays.remove(identifier);
            equipmentsCurrent[0].headMountedAudios.remove(identifier);
            emit that->equipmentsChanged(equipmentsCurrent);
            emit that->equipmentChanged(0, equipmentsCurrent.value(0));
        }
    }

public: // variables
    System *that{nullptr};
    QReadWriteLock initializeLock{QReadWriteLock::RecursionMode::Recursive};
    bool initialized{false};
    QSharedPointer<SystemEventProvider> eventProvider;
    QSharedPointer<SystemTrackingProvider> trackingProvider;
    QReadWriteLock updateLock{QReadWriteLock::RecursionMode::Recursive};
    bool current{true};
    QMap<CuteVR::Identifier, QSharedPointer<CuteVR::Device>> devicesCurrent{};
    QMap<CuteVR::Identifier, CuteVR::System::Equipment> equipmentsCurrent{};
    QMap<CuteVR::Identifier, CuteVR::System::Cell> cellsCurrent{};
};

System::System() :
        _private{new Private{this}} {}

System::~System() = default;

QList<Identifier> System::filteredDevices(Optional<Identifier> const cell,
                                          Optional<Identifier> const equipment) const noexcept {
    auto const cellEnabled{
            ConfigurationServer::isEnabled(feature(Feature::cell)).right(false) && cell.hasValue()};
    auto const equipmentEnabled{
            ConfigurationServer::isEnabled(feature(Feature::equipment)).right(false) && equipment.hasValue()};
    if ((!cellEnabled && !equipmentEnabled) ||
        (cellEnabled && cell.value() != 0) ||
        (equipmentEnabled && equipment.value() != 0)) {
        return {};
    }
    QVector<Device::Category> const included{
            Device::Category::headMountedDisplay,
            Device::Category::headMountedAudio,
            Device::Category::controller,
    };
    QReadLocker{&_private->updateLock};
    QList<Identifier> filtered{};
    for (auto const &device : devices) {
        if ((cellEnabled && !equipmentEnabled) ||
            (equipmentEnabled && included.contains(device->category()))) {
            filtered.append(device->identifier);
        }
    }
    return filtered;
}

QList<Identifier> System::filteredDevices(Device::Category const category) const noexcept {
    QReadLocker{&_private->updateLock};
    QList<Identifier> filtered{};
    for (auto const &device : devices) {
        if (device->category() == category) {
            filtered.append(device->identifier);
        }
    }
    return filtered;
}

Optional<Identifier> System::equipment(Identifier const device) const noexcept {
    if (!ConfigurationServer::isEnabled(feature(Feature::equipment)).right(false)) {
        return {};
    }
    QVector<Device::Category> const included{
            Device::Category::headMountedDisplay,
            Device::Category::headMountedAudio,
            Device::Category::controller,
    };
    QReadLocker{&_private->updateLock};
    return devices.contains(device) &&
           included.contains(devices.value(device)->category()) ? Optional<Identifier>{Identifier{0}}
                                                                : Optional<Identifier>{};
}

Optional<Identifier> System::cell(Identifier const device) const noexcept {
    if (!ConfigurationServer::isEnabled(feature(Feature::cell)).right(false)) {
        return {};
    }
    QReadLocker{&_private->updateLock};
    return devices.contains(device) ? Optional<Identifier>{Identifier{0}}
                                    : Optional<Identifier>{};
}

void System::destroy() {
    QWriteLocker{&_private->initializeLock};
    if (_private->initialized) {
        _private->eventProvider.clear();
        _private->trackingProvider.clear();
        _private->initialized = false;
    }
    // remove all devices
    for (auto &deviceIdentifier : devices.keys()) {
        _private->handleDeviceDeactivated(deviceIdentifier);
    }
}

bool System::isDestroyed() const noexcept {
    QReadLocker{&_private->initializeLock};
    return !_private->initialized;
}

void System::initialize() {
    QWriteLocker{&_private->initializeLock};
    if (!_private->initialized) {
        _private->eventProvider.reset(new Private::SystemEventProvider{this, [&](vr::VREvent_t const &event) {
            switch (event.eventType) {
                case vr::VREvent_TrackedDeviceActivated: {
                    QtConcurrent::run([this,event]() { _private->handleDeviceActivated(event.trackedDeviceIndex);});
                    break;
                }
                case vr::VREvent_TrackedDeviceDeactivated: {
                    QtConcurrent::run([this,event]() { _private->handleDeviceDeactivated(event.trackedDeviceIndex);});
                    break;
                }
                default: break;
            }
        }});
        DriverServer::announce(_private->eventProvider.toWeakRef(), {}, {
                vr::VREvent_TrackedDeviceActivated,
                vr::VREvent_TrackedDeviceDeactivated,
                vr::VREvent_TrackedDeviceUpdated,
                vr::VREvent_PropertyChanged,
                vr::VREvent_Quit,
                vr::VREvent_DriverRequestedQuit,
                vr::VREvent_QuitAcknowledged,
        });
        _private->trackingProvider.reset(new Private::SystemTrackingProvider{});
        DriverServer::announce(_private->trackingProvider.toWeakRef(), QSet<Identifier>{});

        // add devices after all other is initialized
        DriverServer::synchronized([&] {
            for (quint32 index = 0; index < vr::k_unMaxTrackedDeviceCount; index++) {
                if (vr::VRSystem()->IsTrackedDeviceConnected(index)) {
                    _private->handleDeviceActivated(index);
                }
            }
        }, Trilean::yes);

        _private->initialized = true;
    }
}

bool System::isInitialized() const noexcept {
    QReadLocker{&_private->initializeLock};
    return _private->initialized;
}

void System::update() {
    QWriteLocker{&_private->updateLock};
    if (!_private->current) {
        auto const equipmentEnabled{ConfigurationServer::isEnabled(feature(Feature::equipment)).right(false)};
        auto const cellEnabled{ConfigurationServer::isEnabled(feature(Feature::cell)).right(false)};
        devices = _private->devicesCurrent;
        if (equipmentEnabled) {
            equipments = _private->equipmentsCurrent;
        } else {
            equipments.clear();
        }
        if (cellEnabled) {
            cells = _private->cellsCurrent;
        } else {
            cells.clear();
        }
        _private->current = true;
    }
}

bool System::isCurrent() const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->current;
}

#include "../include/CuteVR/moc_System.cpp" // LEGACY: CMake 3.8 ignores include paths
