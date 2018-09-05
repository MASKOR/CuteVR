/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_EQUALITY_COMPARABLE
#define CUTE_VR_INTERFACE_EQUALITY_COMPARABLE

namespace CuteVR { namespace Interface {
    /// @interface EqualityComparable
    /// @brief The objects of the derived class can be compared for equality.
    /// @tparam DerivedT The derived class that is to be comparable on equality.
    template<class DerivedT>
    class EqualityComparable {
        friend bool operator==(DerivedT const &left, DerivedT const &right) noexcept {
            return left.equals(right) && right.equals(left);
        }

        friend bool operator!=(DerivedT const &left, DerivedT const &right) noexcept {
            return !left.equals(right) || !right.equals(left);
        }

    public: // destructor
        virtual ~EqualityComparable() = default;

    public: // methods
        /// @brief Compares the current object with the other.
        /// @param other The other object to compare with.
        /// @return Returns true if the objects are the same in this way, otherwise false.
        virtual bool equals(DerivedT const &other) const noexcept = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_EQUALITY_COMPARABLE
