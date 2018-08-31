/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_PERIPHERAL_CAMERA
#define CUTE_VR_COMPONENTS_PERIPHERAL_CAMERA

#include <QtCore/QSize>
#include <QtGui/QMatrix4x3>

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Peripheral {
    /// @brief A Camera that takes pictures or videos in the non-virtual reality and is e.g. mounted on a device.
    struct Camera final :
            public CategorizedComponent<Component::Category::peripheral, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief Contains the five intrinsic parameters of a camera.
        Q_PROPERTY(QMatrix4x3 intrinsicParameters MEMBER intrinsicParameters FINAL)
        /// @brief The highest possible resolution of the camera in pixels.
        Q_PROPERTY(QSize resolution MEMBER resolution FINAL)
        /// @brief The number of possible frames per second.
        Q_PROPERTY(qreal frameRate MEMBER frameRate FINAL)

    public: // destructor
        ~Camera() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        QMatrix4x3 intrinsicParameters{};
        QSize resolution{};
        qreal frameRate{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Peripheral::Camera)

#endif // CUTE_VR_COMPONENTS_PERIPHERAL_CAMERA
