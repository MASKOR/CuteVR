/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Peripheral/Battery.hpp>

using namespace CuteVR;
using Components::Peripheral::Battery;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Battery>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Battery::clone() const {
    return QSharedPointer<Cloneable>{new Battery{*this}};
}

bool Battery::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Battery const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (charging == another->charging) &&
               (level == another->level);
    }
    return false;
}

QDataStream &Battery::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << charging << level;
}

QDataStream &Battery::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> charging >> level;
}

#include "../../../include/CuteVR/Components/Peripheral/moc_Battery.cpp" // LEGACY: CMake 3.8 ignores include paths
