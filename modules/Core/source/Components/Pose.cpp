/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Pose.hpp>

using namespace CuteVR;
using Components::Pose;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Pose>();
            qRegisterMetaType<Extension::Optional<QVector3D>>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Pose::clone() const {
    return QSharedPointer<Cloneable>{new Pose{*this}};
}

bool Pose::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Pose const *>(&other)) {
        return another && Component::equals(other) &&
               (valid == another->valid) &&
               (poseTransform == another->poseTransform) &&
               (linearVelocity == another->linearVelocity) &&
               (linearAcceleration == another->linearAcceleration) &&
               (angularVelocity == another->angularVelocity) &&
               (angularAcceleration == another->angularAcceleration);
    }
    return false;
}

QDataStream &Pose::serialize(QDataStream &stream) const {
    return Component::serialize(stream) << valid << poseTransform << linearVelocity << linearAcceleration
                                        << angularVelocity << angularAcceleration;
}

QDataStream &Pose::deserialize(QDataStream &stream) {
    return Component::deserialize(stream) >> valid >> poseTransform >> linearVelocity >> linearAcceleration
                                          >> angularVelocity >> angularAcceleration;
}

#include "../../include/CuteVR/Components/moc_Pose.cpp" // LEGACY: CMake 3.8 ignores include paths
