/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_SENSOR_GYROSCOPE
#define CUTE_VR_COMPONENTS_SENSOR_GYROSCOPE

#include <QtGui/QVector3D>

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Sensor {
    /// @brief A Gyroscope is a sensor that measures the speed of rotation in its own rest frame.
    struct Gyroscope final :
            public CategorizedComponent<Component::Category::sensor, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The expected output due to measurement inaccuracy when there is no angular velocity present in
        /// radians per second.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> bias MEMBER bias FINAL)
        /// @brief The measured angular velocity in radians per second.
        Q_PROPERTY(QVector3D angularVelocity MEMBER angularVelocity FINAL)

    public: // destructor
        ~Gyroscope() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Optional<QVector3D> bias{};
        QVector3D angularVelocity{};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Sensor::Gyroscope)

#endif // CUTE_VR_COMPONENTS_SENSOR_GYROSCOPE
