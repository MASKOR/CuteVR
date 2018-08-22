/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QList>
#include <openvr.h>

#include <CuteVR/Internal/DefaultAxesProvider.hpp>
#include <CuteVR/Internal/Property.hpp>

using namespace CuteVR;
using Components::Input::Axis;
using Extension::Trilean;
using Internal::DefaultAxesProvider;
using Internal::Property::query;

class DefaultAxesProvider::Private {
public: // constructor
    explicit Private(DefaultAxesProvider *that, Identifier const device, std::function<void(Axis const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryAxisIdentifiers();
        queryAxes();
    }

public: // methods
    void queryAxisIdentifiers() {
        QList<qint32> const axisTypes{
                query<qint32>(device, vr::Prop_Axis0Type_Int32),
                query<qint32>(device, vr::Prop_Axis1Type_Int32),
                query<qint32>(device, vr::Prop_Axis2Type_Int32),
                query<qint32>(device, vr::Prop_Axis3Type_Int32),
                query<qint32>(device, vr::Prop_Axis4Type_Int32),
        };
        Identifier identifier = 0;
        for (auto axisType : axisTypes) {
            switch (static_cast<vr::EVRControllerAxisType>(axisType)) {
                case vr::k_eControllerAxis_None: break;
                case vr::k_eControllerAxis_TrackPad:
                case vr::k_eControllerAxis_Joystick: {
                    axisIdentifiers.append(identifier);
                    axisIdentifiers.append(identifier + 1);
                    break;
                }
                case vr::k_eControllerAxis_Trigger: {
                    axisIdentifiers.append(identifier);
                    break;
                }
                default:break;
            }
            identifier += 2;
        }
    }

    void queryAxes() {
        vr::VRControllerState_t state{};
        DriverServer::synchronized([&] {
            vr::VRSystem()->GetControllerState(device, &state, (sizeof(vr::VRControllerState_t)));
        }, Trilean::yes);
        for (auto const axisIdentifier : axisIdentifiers) {
            Axis axis{};
            axis.identifier = axisIdentifier;
            axis.position = (axisIdentifier % 2) == 0 ? state.rAxis[axisIdentifier / 2].x
                                                      : state.rAxis[axisIdentifier / 2].y;
            callback(axis);
        }
    }

public: // variables
    DefaultAxesProvider *that{nullptr};
    Identifier device{};
    std::function<void(Axis const &)> callback{};
    QList<Identifier> axisIdentifiers{};
};

DefaultAxesProvider::DefaultAxesProvider(Identifier const device, std::function<void(Axis const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultAxesProvider::~DefaultAxesProvider() = default;

bool DefaultAxesProvider::handleCyclic(void const *) {
    _private->queryAxes();
    return false;
}
