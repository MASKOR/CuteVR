/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QList>
#include <openvr.h>

#include <CuteVR/Internal/DefaultAvailabilityProvider.hpp>
#include <CuteVR/Internal/Property.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Availability;
using Extension::Trilean;
using Internal::DefaultAvailabilityProvider;
using Internal::Property::query;

class DefaultAvailabilityProvider::Private {
public: // constructor
    explicit Private(DefaultAvailabilityProvider *that, Identifier const device,
                     std::function<void(Availability const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryConnection();
    }

public: // methods
    void queryConnection() {
        Availability availability{};
        DriverServer::synchronized([&] {
            availability.connected = static_cast<Trilean>(vr::VRSystem()->IsTrackedDeviceConnected(device));
        }, Trilean::yes);
        callback(availability);
    }

public: // variables
    DefaultAvailabilityProvider *that;
    Identifier device{};
    std::function<void(Availability const &)> callback{};
};

DefaultAvailabilityProvider::DefaultAvailabilityProvider(Identifier const device,
                                                         std::function<void(Availability const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultAvailabilityProvider::~DefaultAvailabilityProvider() = default;

bool DefaultAvailabilityProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (theEvent == nullptr) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_TrackedDeviceActivated: {
            Availability availability{};
            availability.connected = Trilean::yes;
            _private->callback(availability);
            return true;
        }
        case vr::VREvent_TrackedDeviceDeactivated: {
            Availability availability{};
            availability.connected = Trilean::no;
            _private->callback(availability);
            return true;
        }
        default: return false;
    }
}

bool DefaultAvailabilityProvider::handleTracking(void const *tracking) {
    auto const *theTracking{static_cast<vr::TrackedDevicePose_t const *>(tracking)};
    if (theTracking == nullptr) {
        return false;
    }
    Availability availability{};
    availability.connected = static_cast<Trilean>(theTracking->bDeviceIsConnected);
    switch (theTracking->eTrackingResult) {
        case vr::TrackingResult_Uninitialized: {
            availability.detected = Trilean::no;
            availability.calibrated = Trilean::no;
            break;
        }
        case vr::TrackingResult_Calibrating_OutOfRange: {
            availability.detected = Trilean::yes;
            availability.calibrated = Trilean::no;
            availability.visible = Trilean::no;
            break;
        }
        case vr::TrackingResult_Calibrating_InProgress: {
            availability.detected = Trilean::yes;
            availability.calibrated = Trilean::no;
            availability.visible = Trilean::yes;
            break;
        }
        case vr::TrackingResult_Running_OutOfRange: {
            availability.detected = Trilean::yes;
            availability.calibrated = Trilean::yes;
            availability.visible = Trilean::no;
            break;
        }
        case vr::TrackingResult_Running_OK: {
            availability.detected = Trilean::yes;
            availability.calibrated = Trilean::yes;
            availability.visible = Trilean::yes;
            break;
        }
        default:break;
    }
    _private->callback(availability);
    return true;
}
