/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_AXES_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_AXES_PROVIDER

#include <functional>

#include <CuteVR/Components/Input/Axis.hpp>
#include <CuteVR/Interface/CyclicHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultAxesProvider :
            public Interface::CyclicHandler {
    public: // constructor/destructor
        DefaultAxesProvider(Identifier device, std::function<void(Components::Input::Axis const &)> callback);

        ~DefaultAxesProvider() override;

        Q_DISABLE_COPY(DefaultAxesProvider)

    public: // methods
        bool handleCyclic(void const *data) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_AXES_PROVIDER
