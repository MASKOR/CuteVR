/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_ACTIVITY
#define CUTE_VR_COMPONENTS_ACTIVITY

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Interaction {
    /// @brief An Activity is kind of a conceptual Component that provides a statement about the usage of a Device.
    struct Activity final :
            public CategorizedComponent<Component::Category::interaction, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The usage gives a hint on the current activity level.
        Q_PROPERTY(CuteVR::Components::Interaction::Activity::Usage usage MEMBER usage FINAL)

    public: // types
        /// @brief Used to indicate when the user last interacted with the device.
        enum class Usage :
                quint8 {
            undefined, ///< Default if no value has been given.
            now, ///< The user interacts with the device at this moment.
            recent, ///< The device has been used recently, but has not yet entered standby mode.
            standby, ///< The device has not been used for a certain period of time and is therefore in standby mode.
        };

        Q_ENUM(Usage)

    public: // destructor
        ~Activity() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Components::Interaction::Activity::Usage usage{Usage::undefined};
    };

    /// @ostream{activity usage}
    QDataStream &operator<<(QDataStream &stream, Activity::Usage usage);

    /// @istream{activity usage}
    QDataStream &operator>>(QDataStream &stream, Activity::Usage &usage);
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Activity)

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Activity::Usage)

#endif // CUTE_VR_COMPONENTS_ACTIVITY
