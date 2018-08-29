/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_INPUT_BUTTON
#define CUTE_VR_COMPONENTS_INPUT_BUTTON

#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Input {
    /// @brief A digital Button that is either pressed or not, and may be touch sensitive.
    struct Button final :
            public CategorizedComponent<Component::Category::input, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief Indicates whether the button is pressed.
        Q_PROPERTY(CuteVR::Extension::Trilean pressed MEMBER pressed FINAL)
        /// @brief Indicates whether the button is touched.
        /// @details If supported, it is also possible to check whether the button is touched. An untouched button is
        /// always unpressed and a pressed one is always touched.
        Q_PROPERTY(CuteVR::Extension::Trilean touched MEMBER touched FINAL)

    public: // destructor
        ~Button() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Trilean pressed{Extension::maybe};
        CuteVR::Extension::Trilean touched{Extension::maybe};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Input::Button)

#endif // CUTE_VR_COMPONENTS_INPUT_BUTTON
