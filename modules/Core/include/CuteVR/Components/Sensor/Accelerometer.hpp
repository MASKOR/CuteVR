/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_SENSOR_ACCELEROMETER
#define CUTE_VR_COMPONENTS_SENSOR_ACCELEROMETER

#include <QtGui/QVector3D>

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Sensor {
    /// @brief An Accelerometer is a sensor that measures the acceleration in its own rest frame.
    struct Accelerometer final :
            public CategorizedComponent<Component::Category::sensor, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The expected output due to measurement inaccuracy when there is no acceleration present in meters
        /// per second squared.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> bias MEMBER bias FINAL)
        /// @brief The measured acceleration in meters per second squared.
        Q_PROPERTY(QVector3D linearAcceleration MEMBER linearAcceleration FINAL)

    public: // destructor
        ~Accelerometer() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Optional<QVector3D> bias{};
        QVector3D linearAcceleration{};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Sensor::Accelerometer)

#endif // CUTE_VR_COMPONENTS_SENSOR_ACCELEROMETER
