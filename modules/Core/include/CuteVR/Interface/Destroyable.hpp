/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_DESTROYABLE
#define CUTE_VR_INTERFACE_DESTROYABLE

namespace CuteVR { namespace Interface {
    /// @interface Destroyable
    /// @brief The objects of the derived class can destroy their associated data.
    class Destroyable {
    public: // destructor
        virtual ~Destroyable() = default;

    public: // methods
        /// @brief Destroys the data associated with the object.
        virtual void destroy() = 0;

        /// @brief Checks whether the associated data of the object has already been destroyed.
        /// @return `true` if the object has already been destroyed.
        virtual bool isDestroyed() const noexcept = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_DESTROYABLE
