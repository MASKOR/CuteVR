/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_BUTTONS_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_BUTTONS_PROVIDER

#include <functional>

#include <CuteVR/Components/Input/Button.hpp>
#include <CuteVR/Interface/EventHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultButtonsProvider :
            public Interface::EventHandler {
    public: // constructor/destructor
        DefaultButtonsProvider(Identifier device, std::function<void(Components::Input::Button const &)> callback);

        ~DefaultButtonsProvider() override;

        Q_DISABLE_COPY(DefaultButtonsProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_BUTTONS_PROVIDER
