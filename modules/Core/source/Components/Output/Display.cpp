/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Output/Display.hpp>

using namespace CuteVR;
using Components::Output::Display;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Display>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Display::clone() const {
    return QSharedPointer<Cloneable>{new Display{*this}};
}

bool Display::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Display const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (resolution == another->resolution) &&
               (refreshRate == another->refreshRate);
    }
    return false;
}

QDataStream &Display::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << resolution << refreshRate;
}

QDataStream &Display::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> resolution >> refreshRate;
}

#include "../../../include/CuteVR/Components/Output/moc_Display.cpp" // LEGACY: CMake 3.8 ignores include paths
