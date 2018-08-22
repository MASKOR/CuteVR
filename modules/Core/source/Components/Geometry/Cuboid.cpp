/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Cuboid.hpp>

using namespace CuteVR;
using Components::Geometry::Cuboid;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Cuboid>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Cuboid::clone() const {
    return QSharedPointer<Cloneable>{new Cuboid{*this}};
}

bool Cuboid::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Cuboid const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (a == another->a) &&
               (b == another->b) &&
               (c == another->c);
    }
    return false;
}

QDataStream &Cuboid::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << a << b << c;
}

QDataStream &Cuboid::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> a >> b >> c;
}

#include "../../../include/CuteVR/Components/Geometry/moc_Cuboid.cpp" // LEGACY: CMake 3.8 ignores include paths
