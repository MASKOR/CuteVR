/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QMap>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Internal/DefaultEyesProvider.hpp>
#include <CuteVR/Internal/Matrix4x4.hpp>
#include <CuteVR/Internal/Property.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Interaction::Eye;
using Configurations::Core::Parameter;
using Configurations::parameter;
using Extension::Trilean;
using Internal::DefaultEyesProvider;
using Internal::Matrix4x4::from;
using Internal::Property::query;

class DefaultEyesProvider::Private {
public: // constructor
    explicit Private(DefaultEyesProvider *that, Identifier const device, std::function<void(Eye const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryEyes();
    }

public: // methods
    void queryEyes() {
        queryEye(vr::EVREye::Eye_Left);
        queryEye(vr::EVREye::Eye_Right);
    }

    void queryEye(vr::EVREye const type) {
        QMap<vr::EVREye, Eye::Type> const typeMapping{
                {vr::EVREye::Eye_Left, Eye::Type::left},
                {vr::EVREye::Eye_Right, Eye::Type::right},
                // FIXME: not available yet {vr::EVREye::Eye_Center, Eye::Type::both},
        };
        auto const zNearCandidate{query<float>(device, vr::Prop_UserHeadToEyeDepthMeters_Float)};
        auto const zNear{ConfigurationServer::value(parameter(Parameter::zNear)).right(QVariant{zNearCandidate})
                                                                                .toFloat()};
        auto const zFar{ConfigurationServer::value(parameter(Parameter::zFar)).right(QVariant{zNearCandidate * 1e4f})
                                                                               .toFloat()};
        Eye eye{};
        eye.identifier = static_cast<Identifier>(type);
        eye.type = typeMapping.value(type, Eye::Type::undefined);
        DriverServer::synchronized([&] {
            eye.perspectiveProjection = from(vr::VRSystem()->GetProjectionMatrix(type, zNear, zFar));
            eye.headTransform = from(vr::VRSystem()->GetEyeToHeadTransform(type));
        }, Trilean::yes);
        callback(eye);
    }

public: // variables
    DefaultEyesProvider *that{nullptr};
    Identifier device{};
    std::function<void(Eye const &)> callback{};
};

DefaultEyesProvider::DefaultEyesProvider(Identifier const device, std::function<void(Eye const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultEyesProvider::~DefaultEyesProvider() = default;

bool DefaultEyesProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (theEvent == nullptr) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_IpdChanged: {
            _private->queryEyes();
            return true;
        }
        case vr::VREvent_PropertyChanged: {
            if (theEvent->trackedDeviceIndex == _private->device) {
                switch (theEvent->data.property.prop) {
                    case vr::Prop_UserIpdMeters_Float: {
                        _private->queryEyes();
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
