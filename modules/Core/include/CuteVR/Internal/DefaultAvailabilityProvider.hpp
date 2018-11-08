/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_AVAILABILITY_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_AVAILABILITY_PROVIDER

#include <functional>

#include <CuteVR/Components/Availability.hpp>
#include <CuteVR/Interface/EventHandler.hpp>
#include <CuteVR/Interface/TrackingHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultAvailabilityProvider :
            public Interface::EventHandler,
            public Interface::TrackingHandler {
    public: // constructor/destructor
        DefaultAvailabilityProvider(Identifier device, std::function<void(Components::Availability const &)> callback);

        ~DefaultAvailabilityProvider() override;

        Q_DISABLE_COPY(DefaultAvailabilityProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

        bool handleTracking(void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_AVAILABILITY_PROVIDER
