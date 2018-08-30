/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Magnetometer.hpp>

using namespace CuteVR;
using Components::Sensor::Magnetometer;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Magnetometer>();
            qRegisterMetaType<Extension::Optional<QVector3D>>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Magnetometer::clone() const {
    return QSharedPointer<Cloneable>{new Magnetometer{*this}};
}

bool Magnetometer::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Magnetometer const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (bias == another->bias) &&
               (fieldStrength == another->fieldStrength);
    }
    return false;
}

QDataStream &Magnetometer::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << bias << fieldStrength;
}

QDataStream &Magnetometer::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> bias >> fieldStrength;
}

#include "../../../include/CuteVR/Components/Sensor/moc_Magnetometer.cpp" // LEGACY: CMake 3.8 ignores include paths
