/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DEVICES_TRACKING_REFERENCE_GENERIC
#define CUTE_VR_DEVICES_TRACKING_REFERENCE_GENERIC

#include <CuteVR/Devices/TrackedDevice.hpp>

namespace CuteVR { namespace Devices { namespace TrackingReference {
    /// @brief A generic implementation of a tacking reference that, apart from the appropriate category, has no
    /// additional properties than those inherited from TrackedDevice.
    class Generic final :
            public CategorizedDevice<Device::Category::trackingReference, TrackedDevice> {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")

    public: // constructor/destructor
        explicit Generic(Identifier identifier);

        ~Generic() override = default;

        Q_DISABLE_COPY(Generic)
    };
}}}

#endif // CUTE_VR_DEVICES_TRACKING_REFERENCE_GENERIC
