/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_SENSOR_PROXIMITY
#define CUTE_VR_COMPONENTS_SENSOR_PROXIMITY

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Sensor {
    /// @brief A Proximity sensor is a sensor that measures when an object approaches and how far away it is.
    struct Proximity final :
            public CategorizedComponent<Component::Category::sensor, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The effect of measurement errors on the output in meters.
        Q_PROPERTY(CuteVR::Extension::Optional<qreal> bias MEMBER bias FINAL)
        /// @brief The nearest distance to a proximate object in meters, anything beyond the sensor range is infinite.
        Q_PROPERTY(qreal nearestDistance MEMBER nearestDistance FINAL)

    public: // destructor
        ~Proximity() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Optional<qreal> bias{};
        qreal nearestDistance{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Sensor::Proximity)

#endif // CUTE_VR_COMPONENTS_SENSOR_PROXIMITY
