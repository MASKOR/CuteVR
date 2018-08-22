/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Input/Button.hpp>

using namespace CuteVR;
using Components::Input::Button;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Button>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Button::clone() const {
    return QSharedPointer<Cloneable>{new Button{*this}};
}

bool Button::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Button const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (pressed == another->pressed) &&
               (touched == another->touched);
    }
    return false;
}

QDataStream &Button::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << pressed << touched;
}

QDataStream &Button::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> pressed >> touched;
}

#include "../../../include/CuteVR/Components/Input/moc_Button.cpp" // LEGACY: CMake 3.8 ignores include paths
