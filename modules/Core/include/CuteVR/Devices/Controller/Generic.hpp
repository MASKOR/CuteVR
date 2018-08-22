/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_GENERIC_CONTROLLER
#define CUTE_VR_GENERIC_CONTROLLER

#include <CuteVR/Components/Input/Axis.hpp>
#include <CuteVR/Components/Input/Button.hpp>
#include <CuteVR/Components/Interaction/Hand.hpp>
#include <CuteVR/Devices/TrackedDevice.hpp>

namespace CuteVR { namespace Devices { namespace Controller {
    /// @brief A generic implementation of a controller that only consists of axes and keys, and also provides
    /// information about the used hands.
    class Generic final :
            public CategorizedDevice<Device::Category::controller, TrackedDevice> {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief A map of all hands that interact with this controller.
        /// @details There is an additional signal which only emits the actually changed hand. To get a hand by its
        /// type use the #hand getter.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Hand>) hands
                   MEMBER hands NOTIFY handsChanged FINAL)
        /// @brief A map of all the axes that constitute this controller.
        /// @details There is an additional signal which only emits the actually changed axis.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Input::Axis>) axes
                   MEMBER axes NOTIFY axesChanged FINAL)
        /// @brief A map of all the buttons that constitute this controller.
        /// @details There is an additional signal which only emits the actually changed button.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Input::Button>) buttons
                   MEMBER buttons NOTIFY buttonsChanged FINAL)

    public: // constructor/destructor
        explicit Generic(Identifier identifier);

        ~Generic() override;

        Q_DISABLE_COPY(Generic)

    public: // getter
        /// @brief Query the identifier of a hand to which the specified type is assigned.
        /// @param type The type the hand must have.
        /// @return The identifier of a hand that is of the given type or nothing.
        Extension::Optional<Identifier> hand(Components::Interaction::Hand::Type type) const noexcept;

    public: // methods
        void destroy() override;

        bool isDestroyed() const noexcept override;

        void initialize() override;

        bool isInitialized() const noexcept override;

        void update() override;

        bool isCurrent() const noexcept override;

    public: // variables
        QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Hand> hands{};
        QMap<CuteVR::Identifier, CuteVR::Components::Input::Axis> axes{};
        QMap<CuteVR::Identifier, CuteVR::Components::Input::Button> buttons{};

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{map of hands}
        void handsChanged(QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Hand>);

        /// @signal{individual hand}
        void handChanged(CuteVR::Identifier, CuteVR::Components::Interaction::Hand);

        /// @signal{map of axes}
        void axesChanged(QMap<CuteVR::Identifier, CuteVR::Components::Input::Axis>);

        /// @signal{individual axis}
        void axisChanged(CuteVR::Identifier, CuteVR::Components::Input::Axis);

        /// @signal{map of buttons}
        void buttonsChanged(QMap<CuteVR::Identifier, CuteVR::Components::Input::Button>);

        /// @signal{individual button}
        void buttonChanged(CuteVR::Identifier, CuteVR::Components::Input::Button);
    };
}}}

#endif // CUTE_VR_GENERIC_CONTROLLER
