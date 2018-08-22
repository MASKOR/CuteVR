/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_INTERACTION_EYE
#define CUTE_VR_COMPONENTS_INTERACTION_EYE

#include <QtGui/QMatrix4x4>

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Interaction {
    /// @brief An Eye is kind of a conceptual Component that serves to indicate which eye is used for defining a view.
    struct Eye final :
            public CategorizedComponent<Component::Category::interaction, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "b")
        /// @brief The type that this eye has.
        Q_PROPERTY(CuteVR::Components::Interaction::Eye::Type type MEMBER type FINAL)
        /// @brief The matrix for the perspective projection.
        Q_PROPERTY(QMatrix4x4 perspectiveProjection MEMBER perspectiveProjection FINAL)
        /// @brief The transformation matrix between head and the eye.
        Q_PROPERTY(QMatrix4x4 headTransform MEMBER headTransform FINAL)

    public: // types
        /// @brief Serves to define the type of the eye.
        enum class Type :
                quint8 {
            undefined, ///< Default if no value has been given.
            left, ///< The view is meant to belong to the left eye.
            right, ///< The view is meant to belong to the right eye.
            both, ///< The view belongs to both eyes, so the center of the view is in between.
        };

        Q_ENUM(Type)

    public: // destructor
        ~Eye() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Components::Interaction::Eye::Type type{Type::undefined};
        QMatrix4x4 perspectiveProjection{};
        QMatrix4x4 headTransform{};
    };

    /// @ostream{eye type}
    QDataStream &operator<<(QDataStream &stream, Eye::Type type);

    /// @istream{eye type}
    QDataStream &operator>>(QDataStream &stream, Eye::Type &type);
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Eye)

Q_DECLARE_METATYPE(CuteVR::Components::Interaction::Eye::Type)

#endif // CUTE_VR_COMPONENTS_INTERACTION_EYE
