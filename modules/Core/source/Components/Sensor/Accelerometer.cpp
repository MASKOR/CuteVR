/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Accelerometer.hpp>

using namespace CuteVR;
using Components::Sensor::Accelerometer;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Accelerometer>();
            qRegisterMetaType<Extension::Optional<QVector3D>>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Accelerometer::clone() const {
    return QSharedPointer<Cloneable>{new Accelerometer{*this}};
}

bool Accelerometer::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Accelerometer const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (bias == another->bias) &&
               (linearAcceleration == another->linearAcceleration);
    }
    return false;
}

QDataStream &Accelerometer::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << bias << linearAcceleration;
}

QDataStream &Accelerometer::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> bias >> linearAcceleration;
}

#include "../../../include/CuteVR/Components/Sensor/moc_Accelerometer.cpp" // LEGACY: CMake 3.8 ignores include paths
