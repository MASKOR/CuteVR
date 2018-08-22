/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <openvr.h>
#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVector>
#include <QtCore/QWeakPointer>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Configurations::Core::Feature;
using Configurations::Core::Parameter;
using Configurations::feature;
using Configurations::parameter;
using Extension::CuteException;
using Extension::Optional;
using Extension::Trilean;
using Interface::CyclicHandler;
using Interface::EventHandler;
using Interface::TrackingHandler;

class DriverServer::Private {
public: // constructor
    explicit Private(DriverServer *that) :
            that{that} {}

public: // methods
    void garbageCollectCyclicHandlers() {
        QWriteLocker{&announceLock};
        for (auto const key : cyclicHandlers.keys()) {
            if (cyclicHandlers.value(key).first.isNull()) {
                cyclicHandlers.remove(key);
            }
        }
    }

    void garbageCollectEventHandlers() {
        QWriteLocker{&announceLock};
        for (auto const key : eventHandlers.keys()) {
            if (eventHandlers.value(key).isNull()) {
                eventHandlers.remove(key);
                for (auto deviceKey : devicesToEventHandlers.keys(key)) {
                    devicesToEventHandlers.remove(deviceKey, key);
                }
                for (auto eventKey : eventsToEventHandlers.keys(key)) {
                    eventsToEventHandlers.remove(eventKey, key);
                }
            }
        }
    }

    void garbageCollectTrackingHandlers() {
        QWriteLocker{&announceLock};
        for (auto const key : trackingHandlers.keys()) {
            if (trackingHandlers.value(key).isNull()) {
                trackingHandlers.remove(key);
                for (auto deviceKey : devicesToEventHandlers.keys(key)) {
                    devicesToEventHandlers.remove(deviceKey, key);
                }
            }
        }
    }

public: // variables
    DriverServer *that{nullptr};
    QReadWriteLock mutex{QReadWriteLock::RecursionMode::Recursive};
    bool preInitialized{false};
    bool initialized{false};
    QReadWriteLock announceLock{QReadWriteLock::RecursionMode::Recursive};
    QHash<qintptr, QPair<QWeakPointer<CyclicHandler>, std::function<void *(void)>>> cyclicHandlers{};
    QHash<qintptr, QWeakPointer<EventHandler>> eventHandlers{};
    QHash<qintptr, QWeakPointer<TrackingHandler>> trackingHandlers{};
    QMultiHash<Identifier, qintptr> devicesToEventHandlers{};
    QMultiHash<qint64, qintptr> eventsToEventHandlers{};
    QMultiHash<Identifier, qintptr> devicesToTrackingHandlers{};
};

DriverServer::~DriverServer() = default;

DriverServer &DriverServer::instance() noexcept {
    static DriverServer instance;
    return instance;
}

void DriverServer::synchronized(std::function<void()> const &functor, Trilean const initialized) {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (initialized == Trilean::yes && !_private->initialized) {
        NotInitialized().raise();
    } else if (initialized == Extension::no && _private->initialized) {
        NotTerminated().raise();
    }
    functor();
}

void DriverServer::announce(QWeakPointer<EventHandler> eventHandler, QSet<Identifier> const &devices,
                            QSet<qint64> const &events) noexcept {
    auto const address{reinterpret_cast<qintptr>(eventHandler.data())};
    if (eventHandler.isNull()) {
        return;
    }
    auto &_private{instance()._private};
    QWriteLocker{&_private->announceLock};
    if (!_private->eventHandlers.contains(address) || _private->eventHandlers.value(address).isNull()) {
        _private->eventHandlers.insert(address, eventHandler);
    }
    auto devicesOrAny{!devices.empty() ? devices : QSet<Identifier>{invalidIdentifier}};
    for (auto const device : devicesOrAny) {
        if (!_private->devicesToEventHandlers.contains(device, address)) {
            _private->devicesToEventHandlers.insert(device, address);
        }
    }
    auto eventsOrAny{!events.empty() ? events : QSet<qint64>{-1}};
    for (auto const event : eventsOrAny) {
        if (!_private->eventsToEventHandlers.contains(event, address)) {
            _private->eventsToEventHandlers.insert(event, address);
        }
    }
}

Optional<QSharedPointer<CuteException>> DriverServer::pollEvents() {
    auto const eventsEnabled{ConfigurationServer::isEnabled(feature(Feature::events)).right(false)};
    if (!eventsEnabled) {
        return {};
    }
    auto const drawingEnabled{ConfigurationServer::isEnabled(feature(Feature::drawing)).right(false)};
    auto const eventTrackingEnabled{ConfigurationServer::isEnabled(feature(Feature::eventTracking)).right(false)};
    auto const &_private{instance()._private};
    bool garbageFound{false};
    QReadLocker locker{&_private->announceLock};

    // poll next event
    vr::VREvent_t vrEvent{};
    vr::TrackedDevicePose_t vrPose{};
    DriverServer::synchronized([&] {
        vr::TrackingUniverseOrigin vrUniverse{drawingEnabled && vr::VRCompositor()
                                              ? vr::VRCompositor()->GetTrackingSpace()
                                              : vr::TrackingUniverseRawAndUncalibrated};
        while (eventTrackingEnabled
               ? vr::VRSystem()->PollNextEventWithPose(vrUniverse, &vrEvent, sizeof(vrEvent), &vrPose)
               : vr::VRSystem()->PollNextEvent(&vrEvent, sizeof(vrEvent))) {
            auto accepted{0};
            void const *event{&vrEvent};
            void const *tracking{eventTrackingEnabled ? &vrPose : nullptr};

            // first find all event handlers that subscribed to this device and event, then send the event to them
            auto eventHandlersForDevice{
                    QSet<qintptr>::fromList(_private->devicesToEventHandlers.values(vrEvent.trackedDeviceIndex))};
            auto eventHandlersForAnyDevice{
                    QSet<qintptr>::fromList(_private->devicesToEventHandlers.values(invalidIdentifier))};
            eventHandlersForDevice.unite(eventHandlersForAnyDevice);
            auto eventHandlersForEvent{
                    QSet<qintptr>::fromList(_private->eventsToEventHandlers.values(vrEvent.eventType))};
            auto eventHandlersForAnyEvent{
                    QSet<qintptr>::fromList(_private->eventsToEventHandlers.values(-1))};
            eventHandlersForEvent.unite(eventHandlersForAnyEvent);
            for (auto const eventHandlerKey : eventHandlersForDevice.intersect(eventHandlersForEvent)) {
                auto eventHandler{_private->eventHandlers.value(eventHandlerKey).toStrongRef()};
                if (!eventHandler.isNull()) {
                    accepted += eventHandler->handleEvent(event, tracking);
                } else {
                    garbageFound = true;
                }
            }

            // test unusual accept numbers while debugging
            if (accepted == 0) {
                qDebug("Event type '%s (%d)' for device %d not accepted.",
                       vr::VRSystem()->GetEventTypeNameFromEnum((vr::EVREventType) vrEvent.eventType),
                       vrEvent.eventType, vrEvent.trackedDeviceIndex);
            }
        }
    }, Trilean::yes);
    if (garbageFound) {
        locker.unlock();
        _private->garbageCollectEventHandlers();
    }
    return {};
}

void DriverServer::announce(QWeakPointer<TrackingHandler> trackingHandler,
                            QSet<Identifier> const &devices) noexcept {
    auto const address{reinterpret_cast<qintptr>(trackingHandler.data())};
    if (trackingHandler.isNull()) {
        return;
    }
    auto &_private{instance()._private};
    QWriteLocker{&_private->announceLock};
    if (!_private->trackingHandlers.contains(address) || _private->trackingHandlers.value(address).isNull()) {
        _private->trackingHandlers.insert(address, trackingHandler);
    }
    auto devicesOrAny{!devices.empty() ? devices : QSet<Identifier>{invalidIdentifier}};
    for (auto const device : devicesOrAny) {
        if (!_private->devicesToTrackingHandlers.contains(device, address)) {
            _private->devicesToTrackingHandlers.insert(device, address);
        }
    }
}

Optional<QSharedPointer<CuteException>> DriverServer::pollTracking() {
    auto const trackingEnabled{ConfigurationServer::isEnabled(feature(Feature::tracking)).right(false)};
    if (!trackingEnabled) {
        return {};
    }
    auto const drawingEnabled{ConfigurationServer::isEnabled(feature(Feature::drawing)).right(false)};
    auto const &_private{instance()._private};
    bool garbageFound{false};
    QReadLocker locker{&_private->announceLock};

    // get tracking poses
    vr::TrackedDevicePose_t vrPoses[vr::k_unMaxTrackedDeviceCount];
    DriverServer::synchronized([&] {
        if (drawingEnabled) {
            vr::VRCompositor()->WaitGetPoses(vrPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
        } else {
            vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseRawAndUncalibrated, 0, vrPoses,
                                                            vr::k_unMaxTrackedDeviceCount);
        }
    }, Trilean::yes);

    // update devices
    for (quint32 index = 0; index < vr::k_unMaxTrackedDeviceCount; index++) {
        quint64 accepted{0};
        void const *tracking{&vrPoses[index]};

        // iterate over all tracking handlers that subscribed to this device, and send the tracking information to them
        auto deviceHandlersForDevice{
                QSet<qint64>::fromList(_private->devicesToTrackingHandlers.values(index))};
        auto deviceHandlersForAnyDevice{
                QSet<qint64>::fromList(_private->devicesToTrackingHandlers.values(invalidIdentifier))};
        deviceHandlersForDevice.unite(deviceHandlersForAnyDevice);
        for (auto const trackingHandlerKey : deviceHandlersForDevice) {
            auto trackingHandler{_private->trackingHandlers.value(trackingHandlerKey).toStrongRef()};
            if (!trackingHandler.isNull()) {
                accepted += trackingHandler->handleTracking(tracking);
            } else {
                garbageFound = true;
            }
        }

        // test unusual accept numbers while debugging
        if (accepted == 0 && vrPoses[index].bDeviceIsConnected) {
            qDebug("Device pose for device %d not handled.", index);
        }
    }
    if (garbageFound) {
        locker.unlock();
        _private->garbageCollectTrackingHandlers();
    }
    return {};
}

void DriverServer::announce(QWeakPointer<CyclicHandler> cyclicHandler,
                            std::function<void *(void)> dataProvider) noexcept {
    auto const address{reinterpret_cast<qintptr>(cyclicHandler.data())};
    if (cyclicHandler.isNull()) {
        return;
    }
    auto const &_private{instance()._private};
    QWriteLocker{&_private->announceLock};
    if (!_private->cyclicHandlers.contains(address) || _private->cyclicHandlers.value(address).first.isNull()) {
        _private->cyclicHandlers.insert(address, qMakePair(cyclicHandler, dataProvider));
    }
}

Optional<QSharedPointer<CuteException>> DriverServer::runCycle() {
    auto const &_private{instance()._private};
    bool garbageFound{false};
    QReadLocker locker{&_private->announceLock};

    // cycle over all cyclic handlers
    for (auto const &cyclicHandlerPair : _private->cyclicHandlers) {
        auto cyclicHandler{cyclicHandlerPair.first.toStrongRef()};
        if (!cyclicHandler.isNull()) {
            cyclicHandler->handleCyclic(cyclicHandlerPair.second ? cyclicHandlerPair.second() : nullptr);
        } else {
            garbageFound = true;
        }
    }
    if (garbageFound) {
        locker.unlock();
        _private->garbageCollectCyclicHandlers();
    }
    return {};
}

Optional<QSharedPointer<CuteException>> DriverServer::preInitialize(Version version) {
    QWriteLocker{&_private->mutex};
    Optional<QSharedPointer<CuteException>> exception{};
    if (qAbs(version.current - Version{}.current) >= 0x01000000) {
        VersionDiverged::create(CuteException::Severity::critical, 0,
                                QString{"Major version differs, the interfaces are not interoperable. Compiled with "
                                        "version %1 but library version is %2"}
                                        .arg(version.current).arg(Version{}.current),
                                exception.value(QSharedPointer<CuteException>{}))->raise();
    } else if (version.current - Version{}.current >= 0x00010000) {
        exception.setValue(VersionDiverged::create(CuteException::Severity::warning, 0,
                                                   QString{"Higher minor version, new options may be ignored. Compiled "
                                                           "with version %1 but library version is %2"}
                                                           .arg(version.current).arg(Version{}.current),
                                                   exception.value(QSharedPointer<CuteException>{})));
    } else if (Version{}.current - version.current >= 0x00010000) {
        exception.setValue(VersionDiverged::create(CuteException::Severity::warning, 0,
                                                   QString{"Lower minor version, behaviour might have been changed. "
                                                           "Compiled with version %1 but library version is %2"}
                                                           .arg(version.current).arg(Version{}.current),
                                                   exception.value(QSharedPointer<CuteException>{})));
    } else if (version.current != Version{}.current) {
        exception.setValue(VersionDiverged::create(CuteException::Severity::informative, 0,
                                                   QString{"Patch version differs, erroneous behavior might have been "
                                                           "fixed. Compiled with version %1 but library version is %2"}
                                                           .arg(version.current).arg(Version{}.current),
                                                   exception.value(QSharedPointer<CuteException>{})));
    } else {
        qInfo("Shared library and compiled version are the same.");
    }
    if (!vr::VR_IsHmdPresent()) {
        exception.setValue(HardwareSetupNotPresent::create(CuteException::Severity::warning, 0,
                                                           "There is no VR hardware setup present.",
                                                           exception.value(QSharedPointer<CuteException>{})));
    } else {
        qInfo("VR hardware setup is present.");
    }
    if (!vr::VR_IsRuntimeInstalled()) {
        exception.setValue(UnderlyingDriverNotInstalled::create(UnderlyingDriverNotInstalled::Severity::critical, 0,
                                                                "OpenVR runtime is not installed or can not be found.",
                                                                exception.value(QSharedPointer<CuteException>{})));
    } else {
        qInfo("OpenVR runtime is installed in '%s'.", vr::VR_RuntimePath());
    }
    // TODO: test if monitor is running when application will use background mode
    _private->preInitialized = !exception.hasValue();
    return exception;
}

void DriverServer::destroy() {
    destroy2();
}

Optional<QSharedPointer<CuteException>> DriverServer::destroy2() {
    auto const cellEnabled{ConfigurationServer::isEnabled(feature(Feature::cell)).right(false)};
    auto const drawingEnabled{ConfigurationServer::isEnabled(feature(Feature::drawing)).right(false)};
    auto const driverLockWarn{ConfigurationServer::value(parameter(Parameter::driverLockWarn)).right(QVariant{5000})};
    auto const driverLockAbort{ConfigurationServer::value(parameter(Parameter::driverLockAbort)).right(QVariant{5000})};
    Optional<QSharedPointer<CuteException>> exception{};

    // lock system for write
    if (!_private->mutex.tryLockForWrite(driverLockWarn.toUInt())) {
        // something else locks the system for a long time
        exception.setValue(CallLocked::create(CuteException::Severity::warning, 0,
                                              QString{"DriverServer write was locked for at least %1 second(s), "
                                                      "waiting some more time for destruction."}
                                                      .arg(driverLockWarn.toUInt()),
                                              exception.value(QSharedPointer<CuteException>{})));
        if (!_private->mutex.tryLockForWrite(driverLockAbort.toUInt())) {
            // something else locks the system for a very long time
            CallLocked::create(CuteException::Severity::critical, 0,
                               QString{"DriverServer write was locked additional %1 second(s), abandon destruction."}
                                       .arg(driverLockAbort.toUInt()),
                               exception.value(QSharedPointer<CuteException>{}))->raise();
        }
    }

    try {
        // test if system has already been shutdown
        if (!_private->initialized) {
            exception.setValue(AlreadyTerminated::create(AlreadyTerminated::Severity::warning, 0,
                                                         "Driver has already been terminated.",
                                                         exception.value(QSharedPointer<CuteException>{})));
            _private->mutex.unlock();
            return exception;
        } else if (!vr::VRSystem()) {
            _private->initialized = false;
            AlreadyTerminated::create(AlreadyTerminated::Severity::critical, 0,
                                      "Driver has already been terminated but not by this method.",
                                      exception.value(QSharedPointer<CuteException>{}))->raise();
        }

        // do the shutdown
        vr::VR_Shutdown();

        // test if all interfaces are destroyed
        if (vr::VRSystem() != nullptr) {
            InterfaceTerminationFailed::create(CuteException::Severity::critical, 0,
                                               "Failed to terminate OpenVR system interface.",
                                               exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (cellEnabled && vr::VRChaperone() != nullptr) {
            InterfaceTerminationFailed::create(CuteException::Severity::critical, 0,
                                               "Failed to terminate OpenVR chaperone interface.",
                                               exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (drawingEnabled && vr::VRCompositor() != nullptr) {
            InterfaceTerminationFailed::create(CuteException::Severity::critical, 0,
                                               "Failed to terminate OpenVR compositor interface.",
                                               exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (drawingEnabled && vr::VRRenderModels() != nullptr) {
            InterfaceTerminationFailed::create(CuteException::Severity::critical, 0,
                                               "Failed to terminate OpenVR render models interface.",
                                               exception.value(QSharedPointer<CuteException>{}))->raise();
        }

        _private->initialized = false;
    } catch (...) {
        _private->mutex.unlock();
        throw;
    }
    _private->mutex.unlock();
    return exception;
}

bool DriverServer::isDestroyed() const noexcept {
    QReadLocker{&_private->mutex};
    return !_private->initialized;
}

void DriverServer::initialize() {
    initialize2();
}

Optional<QSharedPointer<CuteException>> DriverServer::initialize2() {
    auto const cellEnabled{ConfigurationServer::isEnabled(feature(Feature::cell)).right(false)};
    auto const drawingEnabled{ConfigurationServer::isEnabled(feature(Feature::drawing)).right(false)};
    auto const driverLockWarn{ConfigurationServer::value(parameter(Parameter::driverLockWarn)).right(QVariant{5000})};
    auto const driverLockAbort{ConfigurationServer::value(parameter(Parameter::driverLockAbort)).right(QVariant{5000})};
    Optional<QSharedPointer<CuteException>> exception{};

    // lock system for write
    if (!_private->mutex.tryLockForWrite(driverLockWarn.toUInt())) {
        // something else locks the system for a long time
        exception.setValue(CallLocked::create(CuteException::Severity::warning, 0,
                                              QString{"DriverServer write was locked for at least %1 second(s), "
                                                      "waiting some more time for initialization."}
                                                      .arg(driverLockWarn.toUInt()),
                                              exception.value(QSharedPointer<CuteException>{})));
        if (!_private->mutex.tryLockForWrite(driverLockAbort.toUInt())) {
            // something else locks the system for a very long time
            CallLocked::create(CuteException::Severity::critical, 0,
                               QString{"DriverServer write was locked additional %1 second(s), abandon initialization."}
                                       .arg(driverLockAbort.toUInt()),
                               exception.value(QSharedPointer<CuteException>{}))->raise();
        }
    }

    QWriteLocker{&_private->mutex};
    _private->mutex.unlock(); // hand-over to RAII locker

    if (!_private->preInitialized) {
        exception.setValue(InitializationFailed::create(AlreadyTerminated::Severity::informative, 0,
                                                        "There has no pre initialization, I hope you know what you do.",
                                                        exception.value(QSharedPointer<CuteException>{})));
    }

    try {
        // test if system has already been initialized
        if (_private->initialized) {
            exception.setValue(AlreadyInitialized::create(AlreadyInitialized::Severity::warning, 0,
                                                          "Driver has already been initialized.",
                                                          exception.value(QSharedPointer<CuteException>{})));
            return exception;
        } else if (vr::VRSystem() != nullptr) {
            _private->initialized = true;
            AlreadyInitialized::create(AlreadyInitialized::Severity::critical, 0,
                                       "Driver has not been initialized properly by this method.",
                                       exception.value(QSharedPointer<CuteException>{}))->raise();
        }

        // try to initialize underlying system
        vr::EVRInitError initError{vr::VRInitError_None};
        vr::IVRSystem *system{vr::VR_Init(&initError,
                                          drawingEnabled ? vr::EVRApplicationType::VRApplication_Scene
                                                         : vr::EVRApplicationType::VRApplication_Background)};
        if (initError != vr::VRInitError_None || system == nullptr) {
            RuntimeInitializationFailed::create(CuteException::Severity::critical, initError,
                                                QString{"OpenVR initialization failed with error '%1': '%2'."}
                                                        .arg(vr::VR_GetVRInitErrorAsSymbol(initError))
                                                        .arg(vr::VR_GetVRInitErrorAsEnglishDescription(initError)),
                                                exception.value(QSharedPointer<CuteException>{}))->raise();
        } else {
            qInfo("OpenVR initialization completed.");
        }

        // test if all interfaces are available
        if (vr::VRSystem() == nullptr) {
            InterfaceInitializationFailed::create(CuteException::Severity::critical, 0,
                                                  "Failed to initialize OpenVR system interface.",
                                                  exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (cellEnabled && vr::VRChaperone() == nullptr) {
            InterfaceInitializationFailed::create(CuteException::Severity::critical, 0,
                                                  "Failed to initialize OpenVR chaperone interface.",
                                                  exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (drawingEnabled && vr::VRCompositor() == nullptr) {
            InterfaceInitializationFailed::create(CuteException::Severity::critical, 0,
                                                  "Failed to initialize OpenVR compositor interface.",
                                                  exception.value(QSharedPointer<CuteException>{}))->raise();
        } else if (drawingEnabled && vr::VRRenderModels() == nullptr) {
            // FIXME: happens if there is no physical (?) controller present, this should actually throw
            //SystemDriverInterfaceInitializationFailed::create(
            //        "Failed to initialize OpenVR render models interface.")->raise();
            exception.setValue(InterfaceInitializationFailed::create(CuteException::Severity::critical, 0,
                                                                     "Failed to initialize OpenVR render models interface.",
                                                                     exception.value(QSharedPointer<CuteException>{})));
        } else {
            qInfo("OpenVR interfaces initialized.");
        }

        // test if interface versions are valid
        QVector<char const *> versions{
                vr::IVRApplications_Version,
                vr::IVRSystem_Version,
        };
        if (cellEnabled) {
            versions.append(vr::IVRChaperone_Version);
        }
        if (drawingEnabled) {
            versions.append(vr::IVRCompositor_Version);
            versions.append(vr::IVRRenderModels_Version);
        }
        for (auto const &version : versions) {
            if (!vr::VR_IsInterfaceVersionValid(version)) {
                InterfaceVersionInvalid::create(CuteException::Severity::warning, 0,
                                                QString{"Interface version '%1' is invalid."}.arg(version),
                                                exception.value(QSharedPointer<CuteException>{}))->raise();
            } else {
                qInfo("Interface version '%s' is valid.", version);
            }
        }

        // set new state, the system can potentially be used now
        _private->initialized = true;
    } catch (...) {
        vr::VR_Shutdown();
        throw;
    }
    return exception;
}

bool DriverServer::isInitialized() const noexcept {
    QReadLocker{&_private->mutex};
    return _private->initialized;
}

DriverServer::DriverServer() :
        _private{new Private{this}} {}

#include "../include/CuteVR/moc_DriverServer.cpp" // LEGACY: CMake 3.8 ignores include paths
