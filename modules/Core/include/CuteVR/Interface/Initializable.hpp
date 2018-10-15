/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_INITIALIZABLE
#define CUTE_VR_INTERFACE_INITIALIZABLE

namespace CuteVR { namespace Interface {
    /// @interface Initializable
    /// @brief The objects of the derived class can initialize themselves.
    class Initializable {
    public: // destructor
        virtual ~Initializable() = default;

    public: // methods
        /// @brief The object initializes itself.
        virtual void initialize() = 0;

        /// @brief Checks whether the object is already initialized.
        /// @return `true` if the object is initialized.
        virtual bool isInitialized() const noexcept = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_INITIALIZABLE
