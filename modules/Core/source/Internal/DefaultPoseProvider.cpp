/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <openvr.h>
#include <QtCore/QDateTime>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Internal/DefaultPoseProvider.hpp>
#include <CuteVR/Internal/Matrix4x4.hpp>
#include <CuteVR/Internal/Vector3.hpp>

using namespace CuteVR;
using Components::Pose;
using Configurations::Core::Feature;
using Configurations::feature;
using Extension::Trilean;
using Internal::DefaultPoseProvider;
using Internal::Matrix4x4::from;
using Internal::Vector3::from;

class DefaultPoseProvider::Private {
public: // constructor
    Private(DefaultPoseProvider *that, Identifier const device, std::function<void(Pose const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {}

public: // variables
    DefaultPoseProvider *that{nullptr};
    Identifier device{};
    std::function<void(Pose const &)> callback{};
    qint64 lastTrackingTime{};
    QVector3D lastLinearVelocity{};
    QVector3D lastAngularVelocity{};
};

DefaultPoseProvider::DefaultPoseProvider(Identifier const device, std::function<void(Pose const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultPoseProvider::~DefaultPoseProvider() = default;

bool DefaultPoseProvider::handleTracking(void const *tracking) {
    auto const linearVelocity{ConfigurationServer::isEnabled(feature(Feature::linearVelocity)).right(false)};
    auto const linearAcceleration{ConfigurationServer::isEnabled(feature(Feature::linearAcceleration)).right(false)};
    auto const angularVelocity{ConfigurationServer::isEnabled(feature(Feature::angularVelocity)).right(false)};
    auto const angularAcceleration{ConfigurationServer::isEnabled(feature(Feature::angularAcceleration)).right(false)};
    auto const newTrackingTime{QDateTime::currentMSecsSinceEpoch()};
    auto const *theTracking{static_cast<vr::TrackedDevicePose_t const *>(tracking)};
    if (theTracking == nullptr) {
        return false;
    }
    Pose pose{};
    // calculate and set new tracking data
    pose.valid = static_cast<Trilean>(theTracking->bPoseIsValid);
    pose.poseTransform = from(theTracking->mDeviceToAbsoluteTracking);
    if (linearAcceleration) {
        pose.linearAcceleration
            .setValue((from(theTracking->vVelocity) - _private->lastLinearVelocity) * 1000.0 /
                      (newTrackingTime - _private->lastTrackingTime));
        _private->lastLinearVelocity = from(theTracking->vVelocity);
    }
    if (linearVelocity) {
        pose.linearVelocity.setValue(from(theTracking->vVelocity));
    }
    if (angularAcceleration) {
        pose.angularAcceleration
            .setValue((from(theTracking->vAngularVelocity) - _private->lastAngularVelocity) * 1000.0 /
                      (newTrackingTime - _private->lastTrackingTime));
        _private->lastLinearVelocity = from(theTracking->vVelocity);
    }
    if (angularVelocity) {
        pose.angularVelocity.setValue(from(theTracking->vAngularVelocity));
    }
    _private->lastTrackingTime = newTrackingTime;
    _private->callback(pose);
    return true;
}
