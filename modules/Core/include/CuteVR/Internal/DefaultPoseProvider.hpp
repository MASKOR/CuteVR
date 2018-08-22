/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_DEFAULT_POSE_PROVIDER
#define CUTE_VR_INTERNAL_DEFAULT_POSE_PROVIDER

#include <functional>

#include <CuteVR/Components/Pose.hpp>
#include <CuteVR/Interface/TrackingHandler.hpp>

namespace CuteVR { namespace Internal {
    /// @private
    class DefaultPoseProvider :
            public Interface::TrackingHandler {
    public: // constructor/destructor
        DefaultPoseProvider(Identifier device, std::function<void(Components::Pose const &)> callback);

        ~DefaultPoseProvider() override;

        Q_DISABLE_COPY(DefaultPoseProvider)

    public: // methods
        bool handleTracking(void const *tracking) override;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;
    };
}}

#endif // CUTE_VR_INTERNAL_DEFAULT_POSE_PROVIDER
