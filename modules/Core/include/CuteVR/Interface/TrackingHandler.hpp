/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_TRACKING_HANDLER
#define CUTE_VR_INTERFACE_TRACKING_HANDLER

namespace CuteVR { namespace Interface {
    /// @interface TrackingHandler
    /// @brief The derived class can handle tracking information.
    class TrackingHandler {
    public: // destructor
        virtual ~TrackingHandler() = default;

    public: // methods
        /// @brief Handles tracking information. Mostly intended for internal use.
        /// @param tracking The tracking information, whose type depends on the underlying driver.
        /// @attention OpenVR implementation of %CuteVR expects a `vr::TrackedDevicePose_t` structure.
        /// @note We know that a `void*` carries certain risks and at first glance, one suspects an unclean solution.
        /// However, this decision has been made very carefully: a `void*` is one efficient solution to offer a fully
        /// hardware- and driver-independent interface. We do not consider it reasonable to define a "universal"
        /// tracking information class or to define any hardware dependent structures. At least in this release there
        /// will be no Qt-like or object-oriented tracking information structures.
        /// @return `true` if the information has been processed.
        virtual bool handleTracking(void const *tracking) = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_TRACKING_HANDLER
