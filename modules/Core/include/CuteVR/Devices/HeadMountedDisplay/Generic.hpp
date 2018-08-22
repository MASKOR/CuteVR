/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DEVICES_HEAD_MOUNTED_DISPLAY_GENERIC
#define CUTE_VR_DEVICES_HEAD_MOUNTED_DISPLAY_GENERIC

#include <CuteVR/Components/Interaction/Eye.hpp>
#include <CuteVR/Components/Output/Display.hpp>
#include <CuteVR/Devices/TrackedDevice.hpp>

namespace CuteVR { namespace Devices { namespace HeadMountedDisplay {
    /// @brief A generic implementation of a head-mounted display that only consists of displays, and also provides
    /// information about the used eyes.
    class Generic final :
            public CategorizedDevice<Device::Category::headMountedDisplay, TrackedDevice> {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief A map of all eyes that interact with this head-mounted display.
        /// @details There is an additional signal which only emits the actually changed eye. To get an eye by its type
        /// use the #eye getter.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Eye>) eyes
                   MEMBER eyes NOTIFY eyesChanged FINAL)
        /// @brief A map of all displays that constitute this controller.
        /// @details There is an additional signal which only emits the actually changed display.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Output::Display>) displays
                   MEMBER displays NOTIFY displaysChanged FINAL)

    public: // constructor/destructor
        explicit Generic(Identifier identifier);

        ~Generic() override;

        Q_DISABLE_COPY(Generic)

    public: // getter
        /// @brief Query the identifier of an eye to which the specified type is assigned.
        /// @param type The type the eye must have.
        /// @return The identifier of an eye that is of the given type or nothing.
        Extension::Optional<Identifier> eye(Components::Interaction::Eye::Type type) const noexcept;

    public: // methods
        void destroy() override;

        bool isDestroyed() const noexcept override;

        void initialize() override;

        bool isInitialized() const noexcept override;

        void update() override;

        bool isCurrent() const noexcept override;

    public: // variables
        QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Eye> eyes;
        QMap<CuteVR::Identifier, CuteVR::Components::Output::Display> displays;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{map of eyes}
        void eyesChanged(QMap<CuteVR::Identifier, CuteVR::Components::Interaction::Eye>);

        /// @signal{individual eye}
        void eyeChanged(CuteVR::Identifier, CuteVR::Components::Interaction::Eye);

        /// @signal{map of displays}
        void displaysChanged(QMap<CuteVR::Identifier, CuteVR::Components::Output::Display>);

        /// @signal{individual display}
        void displayChanged(CuteVR::Identifier, CuteVR::Components::Output::Display);
    };
}}}

#endif // CUTE_VR_DEVICES_HEAD_MOUNTED_DISPLAY_GENERIC
