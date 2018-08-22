/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_CLONEABLE
#define CUTE_VR_INTERFACE_CLONEABLE

#include <QtCore/QSharedPointer>

namespace CuteVR { namespace Interface {
    /// @interface Cloneable
    /// @brief The objects of the derived class can be cloned, i.e. duplicated.
    class Cloneable {
    public: // destructor
        virtual ~Cloneable() = default;

    public: // methods
        /// @brief Creates a usually deep copy of the object.
        virtual QSharedPointer<Cloneable> clone() const = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_CLONEABLE
