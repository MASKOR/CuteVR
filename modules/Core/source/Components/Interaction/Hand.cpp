/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Hand.hpp>

using namespace CuteVR;
using Components::Interaction::Hand;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Hand>();
            qRegisterMetaType<Hand::Type>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Components { namespace Interaction {
    QDataStream &operator<<(QDataStream &stream, Hand::Type const type) {
        return stream << static_cast<quint8>(type);
    }

    QDataStream &operator>>(QDataStream &stream, Hand::Type &type) {
        return stream >> reinterpret_cast<quint8 &>(type);
    }
}}}

QSharedPointer<Cloneable> Hand::clone() const {
    return QSharedPointer<Cloneable>{new Hand{*this}};
}

bool Hand::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Hand const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (type == another->type);
    }
    return false;
}

QDataStream &Hand::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << type;
}

QDataStream &Hand::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> type;
}

#include "../../../include/CuteVR/Components/Interaction/moc_Hand.cpp" // LEGACY: CMake 3.8 ignores include paths
