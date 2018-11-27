/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Eye.hpp>

using namespace CuteVR;
using Components::Interaction::Eye;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Eye>();
            qRegisterMetaType<Eye::Type>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Components { namespace Interaction {
    QDataStream &operator<<(QDataStream &stream, Eye::Type const type) {
        return stream << static_cast<quint8>(type);
    }

    QDataStream &operator>>(QDataStream &stream, Eye::Type &type) {
        return stream >> reinterpret_cast<quint8 &>(type);
    }
}}}

QSharedPointer<Cloneable> Eye::clone() const {
    return QSharedPointer<Cloneable>{new Eye{*this}};
}

bool Eye::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Eye const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (type == another->type) &&
               (perspectiveProjection == another->perspectiveProjection) &&
               (headTransform == another->headTransform);
    }
    return false;
}

QDataStream &Eye::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << type << perspectiveProjection << headTransform;
}

QDataStream &Eye::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> type >> perspectiveProjection >> headTransform;
}

#include "../../../include/CuteVR/Components/Interaction/moc_Eye.cpp" // LEGACY: CMake 3.8 ignores include paths
