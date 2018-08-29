/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_INPUT_AXIS
#define CUTE_VR_COMPONENTS_INPUT_AXIS

#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Input {
    /// @brief An Axis has a value which indicates the current (relative) position on a reference line.
    struct Axis final :
            public CategorizedComponent<Component::Category::input, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The axis is currently in use.
        /// @details Depending on the type of the axis, this can mean that it is e.g. touched or pressed.
        Q_PROPERTY(CuteVR::Extension::Trilean used MEMBER used FINAL)
        /// @brief The current (relative) position on the reference line.
        /// @details This value indicates, for example, how much a trigger is stressed or at which position on a
        /// touchpad the input takes place.
        Q_PROPERTY(qreal position MEMBER position FINAL)

    public: // destructor
        ~Axis() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Trilean used{Extension::maybe};
        qreal position{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Input::Axis)

#endif // CUTE_VR_COMPONENTS_INPUT_AXIS
