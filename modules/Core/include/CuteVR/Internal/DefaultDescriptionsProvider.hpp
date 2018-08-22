/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_DESCRIPTIONS_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_DESCRIPTIONS_PROVIDER

#include <functional>

#include <CuteVR/Components/Description.hpp>
#include <CuteVR/Interface/EventHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultDescriptionsProvider :
            public Interface::EventHandler {
    public: // constructor/destructor
        DefaultDescriptionsProvider(Identifier device, std::function<void(Components::Description const &)> callback);

        ~DefaultDescriptionsProvider() override;

        Q_DISABLE_COPY(DefaultDescriptionsProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_DESCRIPTIONS_PROVIDER
