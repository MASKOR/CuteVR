/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Peripheral/Camera.hpp>

using namespace CuteVR;
using Components::Peripheral::Camera;
using Interface::Cloneable;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Camera>();
        }
    } registerMetaTypes; // NOLINT
}

QSharedPointer<Cloneable> Camera::clone() const {
    return QSharedPointer<Cloneable>{new Camera{*this}};
}

bool Camera::equals(Component const &other) const noexcept {
    if (auto another = dynamic_cast<Camera const *>(&other)) {
        return another && CategorizedComponent::equals(other) &&
               (intrinsicParameters == another->intrinsicParameters) &&
               (resolution == another->resolution) &&
               (frameRate == another->frameRate);
    }
    return false;
}

QDataStream &Camera::serialize(QDataStream &stream) const {
    return CategorizedComponent::serialize(stream) << intrinsicParameters << resolution << frameRate;
}

QDataStream &Camera::deserialize(QDataStream &stream) {
    return CategorizedComponent::deserialize(stream) >> intrinsicParameters >> resolution >> frameRate;
}

#include "../../../include/CuteVR/Components/Peripheral/moc_Camera.cpp" // LEGACY: CMake 3.8 ignores include paths
