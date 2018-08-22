/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_SENSOR_MAGNETOMETER
#define CUTE_VR_COMPONENTS_SENSOR_MAGNETOMETER

#include <QtGui/QVector3D>

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Sensor {
    /// @brief A Magnetometer is a sensor that measures the surrounding magnetic field.
    struct Magnetometer final :
            public CategorizedComponent<Component::Category::sensor, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The effect of the surrounding hardware and measurement errors on the output in micro tesla.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> bias MEMBER bias FINAL)
        /// @brief The strength of the surrounding magnetic field in micro tesla.
        Q_PROPERTY(QVector3D fieldStrength MEMBER fieldStrength FINAL)

    public: // destructor
        ~Magnetometer() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Optional<QVector3D> bias{};
        QVector3D fieldStrength{};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Sensor::Magnetometer)

#endif // CUTE_VR_COMPONENTS_SENSOR_MAGNETOMETER
