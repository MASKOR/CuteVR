/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_UPDATABLE
#define CUTE_VR_INTERFACE_UPDATABLE

namespace CuteVR { namespace Interface {
    /// @interface Updatable
    /// @brief The objects of the derived class can update their data independently.
    class Updatable {
    public: // destructor
        virtual ~Updatable() = default;

    public: // methods
        /// @brief The object updates its associated data.
        virtual void update() = 0;

        /// @brief Checks whether the associated data of the object is already current.
        /// @return `true` if the object doesn't need an update.
        virtual bool isCurrent() const noexcept = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_UPDATABLE
