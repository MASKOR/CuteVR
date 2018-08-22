/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Availability.hpp>

using namespace CuteVR;
using Components::Availability;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Availability>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Availability::clone() const {
    return QSharedPointer<Cloneable>{new Availability{*this}};
}

bool Availability::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Availability const *>(&other)) {
        return another && Component::equals(other) &&
               (connected == another->connected) &&
               (detected == another->detected) &&
               (calibrated == another->calibrated) &&
               (visible == another->visible);
    }
    return false;
}

QDataStream &Availability::serialize(QDataStream &stream) const {
    return Component::serialize(stream) << connected << detected << calibrated << visible;
}

QDataStream &Availability::deserialize(QDataStream &stream) {
    return Component::deserialize(stream) >> connected >> detected >> calibrated >> visible;
}

#include "../../include/CuteVR/Components/moc_Availability.cpp" // LEGACY: CMake 3.8 ignores include paths
