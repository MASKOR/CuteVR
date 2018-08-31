/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_INTERACTION_HAND
#define CUTE_VR_COMPONENTS_INTERACTION_HAND

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Interaction {
    /// @brief A Hand is kind of a conceptual Component that serves to indicate which hand is used to control a Device.
    struct Hand final :
            public CategorizedComponent<Component::Category::interaction, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The type that this hand has.
        Q_PROPERTY(CuteVR::Components::Interaction::Hand::Type type MEMBER type FINAL)

    public: // types
        /// @brief Serves to define the type of the hand.
        enum class Type :
                quint8 {
            undefined, ///< Default if no value has been given.
            left, ///< The left hand is considered to control.
            right, ///< The right hand is considered to control.
            both, ///< It is considered that both hands are used to control at the same time.
            varying, ///< No fixed type can be considered, as it is not possible to decide which hand has control.
        };

        Q_ENUM(Type)

    public: // destructor
        ~Hand() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Components::Interaction::Hand::Type type{Type::undefined};
    };

    /// @ostream{hand type}
    QDataStream &operator<<(QDataStream &stream, Hand::Type type);

    /// @istream{hand type}
    QDataStream &operator>>(QDataStream &stream, Hand::Type &type);
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Hand)

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Hand::Type)

#endif // CUTE_VR_COMPONENTS_INTERACTION_HAND
