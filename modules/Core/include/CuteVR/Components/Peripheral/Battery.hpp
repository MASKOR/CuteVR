/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_PERIPHERAL_BATTERY
#define CUTE_VR_COMPONENTS_PERIPHERAL_BATTERY

#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Peripheral {
    /// @brief A Battery that is characterized by whether it is being charged and what its charging level is.
    struct Battery final :
            public CategorizedComponent<Component::Category::peripheral, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The Battery is currently charging.
        Q_PROPERTY(CuteVR::Extension::Trilean charging MEMBER charging FINAL)
        /// @brief The current charge level in percent.
        Q_PROPERTY(qreal level MEMBER level FINAL)

    public: // destructor
        ~Battery() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Trilean charging{Extension::maybe};
        qreal level{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Peripheral::Battery)

#endif // CUTE_VR_COMPONENTS_PERIPHERAL_BATTERY
