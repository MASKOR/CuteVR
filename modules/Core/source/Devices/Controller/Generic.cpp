/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QReadWriteLock>
#include <openvr.h>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Devices/Controller/Generic.hpp>
#include <CuteVR/Internal/DefaultAxesProvider.hpp>
#include <CuteVR/Internal/DefaultButtonsProvider.hpp>
#include <CuteVR/Internal/DefaultHandsProvider.hpp>
#include <CuteVR/DeviceServer.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Input::Axis;
using Components::Input::Button;
using Components::Interaction::Hand;
using Components::Description;
using Configurations::Core::Feature;
using Configurations::feature;
using Devices::Controller::Generic;
using Internal::DefaultAxesProvider;
using Internal::DefaultButtonsProvider;
using Internal::DefaultHandsProvider;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<QMap<Identifier, Axis>>();
            qRegisterMetaType<QMap<Identifier, Button>>();
            qRegisterMetaType<QMap<Identifier, Hand>>();
        }
    } registerMetaTypes; // NOLINT

    struct RegisterDevice {
        RegisterDevice() {
            DeviceServer::registerDevice(
                    [](Identifier const identifier) {
                        auto const enabled{ConfigurationServer::isEnabled(feature(Feature::controllerGeneric))};
                        return QSharedPointer<Device>{enabled ? new Generic{identifier} : nullptr};
                    }, QStringList{"Controller_Generic_1_0_0", "Controller_Generic_1_x"});
        }
    } registerDevice; // NOLINT
}

class Generic::Private {
public: // variables
    QReadWriteLock initializeLock{};
    bool initialized{false};
    QSharedPointer<DefaultAxesProvider> axesProvider;
    QSharedPointer<DefaultButtonsProvider> buttonsProvider;
    QSharedPointer<DefaultHandsProvider> handsProvider;
    QReadWriteLock updateLock{};
    bool current{true};
    QMap<Identifier, Axis> axisCurrent{};
    QMap<Identifier, Button> buttonsCurrent{};
    QMap<Identifier, Hand> handsCurrent{};
    QMap<Hand::Type, Identifier> handsByType{};
};

Generic::Generic(Identifier identifier) :
        CategorizedDevice<Device::Category::controller, TrackedDevice>(identifier),
        _private{new Private} {
    Description cuteDeviceName{"Controller_Generic_1_0_0"};
    cuteDeviceName.type = Description::Type::cuteDeviceName;
    cuteDeviceName.identifier = 1337;
    descriptions.insert(cuteDeviceName.identifier, cuteDeviceName);
}

Generic::~Generic() = default;

Extension::Optional<Identifier>
Devices::Controller::Generic::hand(Components::Interaction::Hand::Type type) const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->handsByType.contains(type) ? Extension::Optional<Identifier>{_private->handsByType.value(type)}
                                                : Extension::Optional<Identifier>{};
}

void Generic::destroy() {
    QWriteLocker{&_private->initializeLock};
    if (_private->initialized) {
        _private->axesProvider.clear();
        _private->buttonsProvider.clear();
        _private->handsProvider.clear();
        _private->initialized = false;
    }
    CategorizedDevice::destroy();
}

bool Generic::isDestroyed() const noexcept {
    QReadLocker{&_private->initializeLock};
    return !_private->initialized && CategorizedDevice::isDestroyed();
}

void Generic::initialize() {
    QWriteLocker{&_private->initializeLock};
    if (!_private->initialized) {
        _private->axesProvider.reset(new DefaultAxesProvider{identifier, [&](Axis const &axis) {
            QWriteLocker{&_private->updateLock};
            if (!_private->axisCurrent.contains(axis.identifier) ||
                _private->axisCurrent.value(axis.identifier) != axis) {
                _private->axisCurrent.insert(axis.identifier, axis);
                _private->current = false;
                emit axesChanged(_private->axisCurrent);
                emit axisChanged(axis.identifier, axis);
            }
        }});
        DriverServer::announce(_private->axesProvider.toWeakRef());
        _private->buttonsProvider.reset(new DefaultButtonsProvider{identifier, [&](Button const &button) {
            QWriteLocker{&_private->updateLock};
            if (!_private->buttonsCurrent.contains(button.identifier) ||
                _private->buttonsCurrent.value(button.identifier) != button) {
                _private->buttonsCurrent.insert(button.identifier, button);
                _private->current = false;
                emit buttonsChanged(_private->buttonsCurrent);
                emit buttonChanged(button.identifier, button);
            }
        }});
        DriverServer::announce(_private->buttonsProvider.toWeakRef(), {identifier}, {
                vr::VREvent_ButtonPress,
                vr::VREvent_ButtonUnpress,
                vr::VREvent_ButtonTouch,
                vr::VREvent_ButtonUntouch,
        });
        _private->handsProvider.reset(new DefaultHandsProvider{identifier, [&](Hand const &hand) {
            QWriteLocker{&_private->updateLock};
            if (!_private->handsCurrent.contains(hand.identifier) ||
                _private->handsCurrent.value(hand.identifier) != hand) {
                _private->handsCurrent.insert(hand.identifier, hand);
                _private->current = false;
                emit handsChanged(_private->handsCurrent);
                emit handChanged(hand.identifier, hand);
            }
        }});
        DriverServer::announce(_private->handsProvider.toWeakRef(), {}, {
                vr::VREvent_TrackedDeviceRoleChanged,
                vr::VREvent_PropertyChanged,
        });
        _private->initialized = true;
    }
    CategorizedDevice::initialize();
}

bool Generic::isInitialized() const noexcept {
    QReadLocker{&_private->initializeLock};
    return _private->initialized && CategorizedDevice::isInitialized();
}

void Devices::Controller::Generic::update() {
    QWriteLocker{&_private->updateLock};
    if (!_private->current) {
        axes = _private->axisCurrent;
        buttons = _private->buttonsCurrent;
        hands = _private->handsCurrent;
        _private->handsByType.clear();
        for (auto const &hand : hands) {
            _private->handsByType.insert(hand.type, hand.identifier);
        }
        _private->current = true;
    }
    CategorizedDevice::update();
}

bool Devices::Controller::Generic::isCurrent() const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->current && CategorizedDevice::isCurrent();
}

#include "../../../include/CuteVR/Devices/Controller/moc_Generic.cpp" // LEGACY: CMake 3.8 ignores include paths
