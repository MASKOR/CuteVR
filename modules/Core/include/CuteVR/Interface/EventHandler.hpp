/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_EVENT_HANDLER
#define CUTE_VR_INTERFACE_EVENT_HANDLER

namespace CuteVR { namespace Interface {
    /// @interface EventHandler
    /// @brief The derived class can handle virtual reality system events.
    class EventHandler {
    public: // destructor
        virtual ~EventHandler() = default;

    public: // methods
        /// @brief Handles a virtual reality system event. Mostly intended for internal use.
        /// @param event Virtual reality system event, whose type depends on the underlying driver.
        /// @attention OpenVR implementation of %CuteVR expects a `vr::VREvent_t` structure.
        /// @note We know that a `void*` carries certain risks and at first glance, one suspects an unclean solution.
        /// However, this decision has been made very carefully: a `void*` is one efficient solution to offer a fully
        /// hardware- and driver-independent interface. Due to the large number of different event types, we do not
        /// consider it reasonable to define a "universal" event class or to overload the previously slim interface
        /// with dozens of special event structures. In addition to the high implementation and maintenance costs, a
        /// loss of performance is caused by the continuous required conversion. These are the main reasons why, at
        /// least in this release, there will be no Qt-like or object-oriented system event structures.
        /// @param tracking Optional information about tracking at the event time, whose type depends on the
        /// underlying driver and which should be expected to be a null pointer.
        /// @attention OpenVR implementation of %CuteVR expects a `vr::TrackedDevicePose_t` structure.
        /// @return `true` if the event has been accepted.
        virtual bool handleEvent(void const *event, void const *tracking) = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_EVENT_HANDLER
