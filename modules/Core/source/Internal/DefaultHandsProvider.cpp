/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <openvr.h>

#include <CuteVR/Internal/DefaultHandsProvider.hpp>
#include <CuteVR/Internal/Property.hpp>

using namespace CuteVR;
using Components::Interaction::Hand;
using Extension::Trilean;
using Internal::DefaultHandsProvider;
using Internal::Property::query;

class DefaultHandsProvider::Private {
public: // constructor
    Private(DefaultHandsProvider *that, Identifier const device, std::function<void(Hand const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryHands();
    }

public: // methods
    void queryHands() {
        auto const role{query<qint32>(device, vr::Prop_ControllerRoleHint_Int32)};
        Hand hand{};
        switch (static_cast<vr::ETrackedControllerRole>(role)) {
            case vr::TrackedControllerRole_OptOut: // TODO: what is this
            case vr::TrackedControllerRole_Invalid: {
                // TODO: check when this happens, maybe single controller or for both hands
                hand.type = Hand::Type::undefined;
                break;
            }
            case vr::TrackedControllerRole_LeftHand: {
                hand.type = Hand::Type::left;
                break;
            }
            case vr::TrackedControllerRole_RightHand: {
                hand.type = Hand::Type::right;
                break;
            }
            default:break;
        }
        callback(hand);
    }

public: // variables
    DefaultHandsProvider *that{nullptr};
    Identifier device{};
    std::function<void(Hand const &)> callback{};
};

DefaultHandsProvider::DefaultHandsProvider(Identifier const device, std::function<void(Hand const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultHandsProvider::~DefaultHandsProvider() = default;

bool DefaultHandsProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (theEvent == nullptr) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_TrackedDeviceRoleChanged: {
            _private->queryHands();
            return true;
        }
        case vr::VREvent_PropertyChanged: {
            if (theEvent->trackedDeviceIndex == _private->device) {
                switch (theEvent->data.property.prop) {
                    case vr::Prop_ControllerRoleHint_Int32: {
                        _private->queryHands();
                        return true;
                    }
                    default: return false;
                }
            } else {
                return false;
            }
        }
        default: return false;
    }
}
