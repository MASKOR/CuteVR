/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QReadWriteLock>
#include <openvr.h>

#include <CuteVR/Devices/TrackedDevice.hpp>
#include <CuteVR/Internal/DefaultAvailabilityProvider.hpp>
#include <CuteVR/Internal/DefaultPoseProvider.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Availability;
using Components::Pose;
using Devices::TrackedDevice;
using Internal::DefaultAvailabilityProvider;
using Internal::DefaultPoseProvider;

class TrackedDevice::Private {
public: // variables
    QReadWriteLock initializeLock{QReadWriteLock::RecursionMode::Recursive};
    bool initialized{false};
    QSharedPointer<DefaultAvailabilityProvider> availabilityProvider;
    QSharedPointer<DefaultPoseProvider> poseProvider;
    QReadWriteLock updateLock{QReadWriteLock::RecursionMode::Recursive};
    bool current{true};
    Availability availabilityCurrent{};
    Pose poseCurrent{};
};

TrackedDevice::TrackedDevice(Identifier const identifier) :
        Device(identifier),
        _private{new Private} {}

TrackedDevice::~TrackedDevice() = default;

void TrackedDevice::destroy() {
    QWriteLocker{&_private->initializeLock};
    if (_private->initialized) {
        _private->availabilityProvider.clear();
        _private->poseProvider.clear();
        _private->initialized = false;
    }
    Device::destroy();
}

bool TrackedDevice::isDestroyed() const noexcept {
    QReadLocker{&_private->initializeLock};
    return !_private->initialized && Device::isDestroyed();
}

void TrackedDevice::initialize() {
    QWriteLocker{&_private->initializeLock};
    if (!_private->initialized) {
        _private->availabilityProvider
                .reset(new DefaultAvailabilityProvider{identifier, [&](Availability const &availability) {
                    QWriteLocker{&_private->updateLock};
                    if (_private->availabilityCurrent != availability) {
                        _private->availabilityCurrent = availability;
                        _private->current = false;
                        emit availabilityChanged(availability);
                    }
                }});
        DriverServer::announce(_private->availabilityProvider.toWeakRef(), {identifier});
        DriverServer::announce(_private->availabilityProvider.toWeakRef(), {identifier}, {
                vr::VREvent_TrackedDeviceActivated,
                vr::VREvent_TrackedDeviceDeactivated,
        });
        _private->poseProvider.reset(new DefaultPoseProvider{identifier, [&](Pose const &pose) {
            QWriteLocker{&_private->updateLock};
            if (_private->poseCurrent != pose) {
                _private->poseCurrent = pose;
                _private->current = false;
                emit poseChanged(pose);
            }
        }});
        DriverServer::announce(_private->poseProvider.toWeakRef(), {identifier});
        _private->initialized = true;
    }
    Device::initialize();
}

bool TrackedDevice::isInitialized() const noexcept {
    QReadLocker{&_private->initializeLock};
    return _private->initialized && Device::isInitialized();
}

void TrackedDevice::update() {
    QWriteLocker{&_private->updateLock};
    if (!_private->current) {
        availability = _private->availabilityCurrent;
        pose = _private->poseCurrent;
        _private->current = true;
    }
    Device::update();
}

bool TrackedDevice::isCurrent() const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->current && Device::isCurrent();
}

#include "../../include/CuteVR/Devices/moc_TrackedDevice.cpp" // LEGACY: CMake 3.8 ignores include paths
