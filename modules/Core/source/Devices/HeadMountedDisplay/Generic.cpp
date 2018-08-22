/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QReadWriteLock>
#include <openvr.h>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Devices/HeadMountedDisplay/Generic.hpp>
#include <CuteVR/Internal/DefaultDisplaysProvider.hpp>
#include <CuteVR/Internal/DefaultEyesProvider.hpp>
#include <CuteVR/DeviceServer.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Interaction::Eye;
using Components::Output::Display;
using Components::Description;
using Configurations::Core::Feature;
using Configurations::feature;
using Devices::HeadMountedDisplay::Generic;
using Extension::Optional;
using Internal::DefaultDisplaysProvider;
using Internal::DefaultEyesProvider;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<QMap<Identifier, Display>>();
            qRegisterMetaType<QMap<Identifier, Eye>>();
        }
    } registerMetaTypes; // NOLINT

    struct RegisterDevice {
        RegisterDevice() {
            DeviceServer::registerDevice(
                    [](Identifier const identifier) {
                        auto const enabled{ConfigurationServer::isEnabled(feature(Feature::headMountedDisplayGeneric))};
                        return QSharedPointer<Device>{enabled ? new Generic{identifier} : nullptr};
                    }, QStringList{"HeadMountedDisplay_Generic_1_0_0", "HeadMountedDisplay_Generic_1_x"});
        }
    } registerDevice; // NOLINT
}

class Generic::Private {
public: // variables
    QReadWriteLock initializeLock{QReadWriteLock::RecursionMode::Recursive};
    bool initialized{false};
    QSharedPointer<DefaultDisplaysProvider> displaysProvider;
    QSharedPointer<DefaultEyesProvider> eyesProvider;
    QReadWriteLock updateLock{QReadWriteLock::RecursionMode::Recursive};
    bool current{true};
    QMap<Identifier, Display> displaysCurrent{};
    QMap<Identifier, Eye> eyesCurrent{};
    QMap<Eye::Type, Identifier> eyesByType{};
};

Generic::Generic(Identifier const identifier) :
        CategorizedDevice<Device::Category::headMountedDisplay, TrackedDevice>(identifier),
        _private{new Private} {
    Description cuteDeviceName{"HeadMountedDisplay_Generic_1_0_0"};
    cuteDeviceName.type = Description::Type::cuteDeviceName;
    cuteDeviceName.identifier = 1337;
    descriptions.insert(cuteDeviceName.identifier, cuteDeviceName);
}

Generic::~Generic() = default;

Optional<Identifier> Generic::eye(Eye::Type type) const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->eyesByType.contains(type) ? Extension::Optional<Identifier>{_private->eyesByType.value(type)}
                                               : Extension::Optional<Identifier>{};
}

void Generic::destroy() {
    QWriteLocker{&_private->initializeLock};
    if (_private->initialized) {
        _private->displaysProvider.clear();
        _private->eyesProvider.clear();
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
        _private->displaysProvider.reset(new DefaultDisplaysProvider{identifier, [&](Display const &display) {
            QWriteLocker{&_private->updateLock};
            if (!_private->displaysCurrent.contains(display.identifier) ||
                _private->displaysCurrent.value(display.identifier) != display) {
                _private->displaysCurrent.insert(display.identifier, display);
                _private->current = false;
                emit displaysChanged(_private->displaysCurrent);
                emit displayChanged(display.identifier, display);
            }
        }});
        DriverServer::announce(_private->displaysProvider.toWeakRef(), {identifier}, {
                vr::VREvent_PropertyChanged,
        });
        _private->eyesProvider.reset(new DefaultEyesProvider{identifier, [&](Eye const &eye) {
            QWriteLocker{&_private->updateLock};
            if (!_private->eyesCurrent.contains(eye.identifier) ||
                _private->eyesCurrent.value(eye.identifier) != eye) {
                _private->eyesCurrent.insert(eye.identifier, eye);
                _private->current = false;
                emit eyesChanged(_private->eyesCurrent);
                emit eyeChanged(eye.identifier, eye);
            }
        }});
        DriverServer::announce(_private->eyesProvider.toWeakRef(), {identifier}, {
                vr::VREvent_IpdChanged,
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

void Generic::update() {
    QWriteLocker{&_private->updateLock};
    if (!_private->current) {
        displays = _private->displaysCurrent;
        eyes = _private->eyesCurrent;
        _private->eyesByType.clear();
        for (auto const &eye : eyes) {
            _private->eyesByType.insert(eye.type, eye.identifier);
        }
        _private->current = true;
    }
    CategorizedDevice::update();
}

bool Generic::isCurrent() const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->current && CategorizedDevice::isCurrent();
}

#include "../../../include/CuteVR/Devices/HeadMountedDisplay/moc_Generic.cpp" // LEGACY: CMake 3.8 ignores include paths
