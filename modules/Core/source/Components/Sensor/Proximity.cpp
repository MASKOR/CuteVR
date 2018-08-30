/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Proximity.hpp>

using namespace CuteVR;
using Components::Sensor::Proximity;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Proximity>();
            qRegisterMetaType<Extension::Optional<qreal>>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Proximity::clone() const {
    return QSharedPointer<Cloneable>{new Proximity{*this}};
}

bool Proximity::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Proximity const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (bias == another->bias) &&
               (nearestDistance == another->nearestDistance);
    }
    return false;
}

QDataStream &Proximity::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << bias << nearestDistance;
}

QDataStream &Proximity::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> bias >> nearestDistance;
}

#include "../../../include/CuteVR/Components/Sensor/moc_Proximity.cpp" // LEGACY: CMake 3.8 ignores include paths
