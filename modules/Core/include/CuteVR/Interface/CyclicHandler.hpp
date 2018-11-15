/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_CYCLIC_HANDLER
#define CUTE_VR_INTERFACE_CYCLIC_HANDLER

namespace CuteVR { namespace Interface {
    /// @interface CyclicHandler
    /// @brief The derived class can handle cyclic published data.
    class CyclicHandler {
    public: // destructor
        virtual ~CyclicHandler() = default;

    public: // methods
        /// @brief Handles data that is published cyclic. Mostly intended for internal use.
        /// @param data Any kind of data, depending on the contract between the registrar and the handler.
        /// @note We know that a `void*` carries certain risks and at first glance, one suspects an unclean solution.
        /// However, this decision has been made very carefully: a `void*` is one efficient solution to offer a fully
        /// hardware- and driver-independent interface. Due to the large number of different data types, we do not
        /// consider it reasonable to define a "universal" class or to overload the previously slim interface with
        /// dozens of special structures. In addition to the high implementation and maintenance costs, a loss of
        /// performance is caused by the continuous required conversion. These are the main reasons why, at least in
        /// this release, there will be no Qt-like or object-oriented data structures.
        /// @return `true` if the data has been processed.
        virtual bool handleCyclic(void const *data) = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_CYCLIC_HANDLER
