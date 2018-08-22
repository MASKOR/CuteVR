/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Component.hpp>

using namespace CuteVR;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Component::Category>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR {
    QDataStream &operator<<(QDataStream &stream, Component::Category const category) {
        return stream << static_cast<quint8>(category);
    }

    QDataStream &operator>>(QDataStream &stream, Component::Category &category) {
        return stream >> reinterpret_cast<quint8 &>(category);
    }
}

Component::Category Component::category() const noexcept {
    return Category::undefined;
}

bool Component::equals(Component const &other) const noexcept {
    return (category() == other.category()) &&
           (identifier == other.identifier);
}

qint32 Component::orders(Component const &other) const noexcept {
    return identifier - other.identifier;
}

QDataStream &Component::serialize(QDataStream &stream) const {
    return stream << identifier;
}

QDataStream &Component::deserialize(QDataStream &stream) {
    return stream >> identifier;
}

#include "../include/CuteVR/moc_Component.cpp" // LEGACY: CMake 3.8 ignores include paths
