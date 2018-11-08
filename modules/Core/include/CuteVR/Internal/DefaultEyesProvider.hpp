/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_EYES_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_EYES_PROVIDER

#include <functional>

#include <CuteVR/Components/Interaction/Eye.hpp>
#include <CuteVR/Interface/EventHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultEyesProvider :
            public Interface::EventHandler {
    public: // constructor/destructor
        DefaultEyesProvider(Identifier device, std::function<void(Components::Interaction::Eye const &)> callback);

        ~DefaultEyesProvider() override;

        Q_DISABLE_COPY(DefaultEyesProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_EYES_PROVIDER
