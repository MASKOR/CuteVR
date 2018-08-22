/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Activity.hpp>

using namespace CuteVR;
using Components::Interaction::Activity;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Activity>();
            qRegisterMetaType<Activity::Usage>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Components { namespace Interaction {
    QDataStream &operator<<(QDataStream &stream, Activity::Usage const usage) {
        return stream << static_cast<quint8>(usage);
    }

    QDataStream &operator>>(QDataStream &stream, Activity::Usage &usage) {
        return stream >> reinterpret_cast<quint8 &>(usage);
    }
}}}

QSharedPointer<Cloneable> Activity::clone() const {
    return QSharedPointer<Cloneable>{new Activity{*this}};
}

bool Activity::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Activity const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (usage == another->usage);
    }
    return false;
}

QDataStream &Activity::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << usage;
}

QDataStream &Activity::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> usage;
}

#include "../../../include/CuteVR/Components/Interaction/moc_Activity.cpp" // LEGACY: CMake 3.8 ignores include paths
