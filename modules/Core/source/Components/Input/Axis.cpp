/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Input/Axis.hpp>

using namespace CuteVR;
using Components::Input::Axis;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Axis>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Axis::clone() const {
    return QSharedPointer<Cloneable>{new Axis{*this}};
}

bool Axis::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Axis const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (used == another->used) &&
               (position == another->position);
    }
    return false;
}

QDataStream &Axis::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << used << position;
}

QDataStream &Axis::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> used >> position;
}

#include "../../../include/CuteVR/Components/Input/moc_Axis.cpp" // LEGACY: CMake 3.8 ignores include paths
