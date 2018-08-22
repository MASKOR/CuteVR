/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_ORDER_COMPARABLE
#define CUTE_VR_INTERFACE_ORDER_COMPARABLE

#include <QtCore/QtGlobal>

namespace CuteVR { namespace Interface {
    /// @interface OrderComparable
    /// @brief The objects of the derived class can be ordered.
    /// @tparam DerivedT The derived class that is to be comparable in order.
    template<class DerivedT>
    class OrderComparable {
        friend bool operator>(DerivedT const &left, DerivedT const &right) noexcept {
            return left.orders(right) > 0;
        }

        friend bool operator<(DerivedT const &left, DerivedT const &right) noexcept {
            return left.orders(right) < 0;
        }

        friend bool operator>=(DerivedT const &left, DerivedT const &right) noexcept {
            return left.orders(right) >= 0;
        }

        friend bool operator<=(DerivedT const &left, DerivedT const &right) noexcept {
            return left.orders(right) <= 0;
        }

    public: // destructor
        virtual ~OrderComparable() = default;

    public: // methods
        /// @brief Specifies whether (and possibly how far) the current object is ordered after or before the other.
        /// @param other The other object that is to be ordered.
        /// @return A positive integer if the current object orders itself after the other, a negative integer if it
        /// orders itself before the other, and zero if both are at the same level.
        virtual qint32 orders(DerivedT const &other) const noexcept = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_ORDER_COMPARABLE
