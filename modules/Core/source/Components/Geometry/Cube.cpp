/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Cube.hpp>

using namespace CuteVR;
using Components::Geometry::Cube;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Cube>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Cube::clone() const {
    return QSharedPointer<Cloneable>{new Cube{*this}};
}

bool Cube::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Cube const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (a == another->a);
    }
    return false;
}

QDataStream &Cube::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << a;
}

QDataStream &Cube::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> a;
}

#include "../../../include/CuteVR/Components/Geometry/moc_Cube.cpp" // LEGACY: CMake 3.8 ignores include paths
