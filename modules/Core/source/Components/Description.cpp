/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Description.hpp>

using namespace CuteVR;
using Components::Description;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Description>();
            qRegisterMetaType<Description::Type>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Components {
    QDataStream &operator<<(QDataStream &stream, Description const &description) {
        return description.serialize(stream);
    }

    QDataStream &operator>>(QDataStream &stream, Description &description) {
        return description.deserialize(stream);
    }

    QDataStream &operator<<(QDataStream &stream, Description::Type const type) {
        return stream << static_cast<quint8>(type);
    }

    QDataStream &operator>>(QDataStream &stream, Description::Type &type) {
        return stream >> reinterpret_cast<quint8 &>(type);
    }
}}

QSharedPointer<Cloneable> Description::clone() const {
    return QSharedPointer<Cloneable>{new Description{*this}};
}

bool Description::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Description const *>(&other)) {
        return another && Component::equals(other) &&
               (static_cast<QString const &>(*this) == static_cast<QString const &>(*another)) &&
               (type == another->type);
    }
    return false;
}

bool Description::equals(Description const &other) const noexcept {
    return Component::equals(other) &&
           (static_cast<QString const &>(*this) == static_cast<QString const &>(other)) &&
           (type == other.type);
}

QDataStream &Description::serialize(QDataStream &stream) const {
    return Component::serialize(stream) << static_cast<QString const &>(*this) << type;
}

QDataStream &Description::deserialize(QDataStream &stream) {
    return Component::deserialize(stream) >> static_cast<QString &>(*this) >> type;
}

#include "../../include/CuteVR/Components/moc_Description.cpp" // LEGACY: CMake 3.8 ignores include paths
