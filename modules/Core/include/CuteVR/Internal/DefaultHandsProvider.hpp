/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_HANDS_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_HANDS_PROVIDER

#include <functional>

#include <CuteVR/Components/Interaction/Hand.hpp>
#include <CuteVR/Interface/EventHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultHandsProvider :
            public Interface::EventHandler {
    public: // constructor/destructor
        DefaultHandsProvider(Identifier device, std::function<void(Components::Interaction::Hand const &)> callback);

        ~DefaultHandsProvider() override;

        Q_DISABLE_COPY(DefaultHandsProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_HANDS_PROVIDER
