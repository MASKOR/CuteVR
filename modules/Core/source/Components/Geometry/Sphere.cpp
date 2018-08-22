/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Sphere.hpp>

using namespace CuteVR;
using Components::Geometry::Sphere;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Sphere>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Sphere::clone() const {
    return QSharedPointer<Cloneable>{new Sphere{*this}};
}

bool Sphere::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Sphere const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (r == another->r);
    }
    return false;
}

QDataStream &Sphere::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << r;
}

QDataStream &Sphere::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> r;
}

#include "../../../include/CuteVR/Components/Geometry/moc_Sphere.cpp" // LEGACY: CMake 3.8 ignores include paths
