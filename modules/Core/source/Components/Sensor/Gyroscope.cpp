/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Gyroscope.hpp>

using namespace CuteVR;
using Components::Sensor::Gyroscope;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Gyroscope>();
            qRegisterMetaType<Extension::Optional<QVector3D>>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Gyroscope::clone() const {
    return QSharedPointer<Cloneable>{new Gyroscope{*this}};
}

bool Gyroscope::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Gyroscope const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (bias == another->bias) &&
               (angularVelocity == another->angularVelocity);
    }
    return false;
}

QDataStream &Gyroscope::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << bias << angularVelocity;
}

QDataStream &Gyroscope::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> bias >> angularVelocity;
}

#include "../../../include/CuteVR/Components/Sensor/moc_Gyroscope.cpp" // LEGACY: CMake 3.8 ignores include paths
