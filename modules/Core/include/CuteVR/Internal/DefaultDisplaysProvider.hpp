/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_DISPLAYS_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_DISPLAYS_PROVIDER

#include <functional>

#include <CuteVR/Components/Output/Display.hpp>
#include <CuteVR/Interface/EventHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultDisplaysProvider :
            public Interface::EventHandler {
    public: // constructor/destructor
        DefaultDisplaysProvider(Identifier device, std::function<void(Components::Output::Display const &)> callback);

        ~DefaultDisplaysProvider() override;

        Q_DISABLE_COPY(DefaultDisplaysProvider)

    public: // methods
        bool handleEvent(void const *event, void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_DISPLAYS_PROVIDER
