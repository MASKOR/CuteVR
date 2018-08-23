/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_EXTENSION_EITHER
#define CUTE_VR_EXTENSION_EITHER

#include <CuteVR/Extension/Optional.hpp>

namespace CuteVR { namespace Extension {
    /// @brief The class has only two options, it has either a left value or a right value.
    /// @details The type is usually used to represent a value that is either correct (right by convention) or
    /// incorrect (left by convention).
    /// @tparam LeftT The type of the left value.
    /// @pre LeftT is copy and move constructible, destructible and not a pointer.
    /// @tparam RightT The type of the right value.
    /// @pre RightT is copy and move constructible, destructible and not a pointer.
    template<class LeftT,
             class RightT>
    class Either final :
            public Interface::Cloneable,
            public Interface::EqualityComparable<Extension::Either<LeftT, RightT>>,
            public Interface::Serializable {
    public: // types
        /// @brief Either is not left but the left getter method was called.
        struct IsNotLeft final :
                public CuriousCuteException<IsNotLeft> {
        };

        /// @brief Either is not right but the right getter method was called.
        struct IsNotRight final :
                public CuriousCuteException<IsNotRight> {
        };

    public: // constructor/destructor/assignment
        /// @brief Creates an Either that is neither left nor right.
        /// @details From a conceptual point of view a standard constructor is not possible, but it allows a much more
        /// comfortable working with the class.
        Either() :
                _side{Side::undefined} {}

        /// @brief Creates an Either that has assigned a left value.
        /// @attention If both left and right have the same type, always the right value is assigned. You can use
        /// #setLeft instead.
        /// @tparam LeftP Used for perfect forwarding.
        /// @pre LeftP must be either the same as LeftT if it's convertible to RightT or must be at least convertible
        /// to LeftT if it isn't convertible to RightT (SFINAE).
        /// @pre Disables this constructor if LeftT and RightT are the same (CLASH).
        /// @param left The left value to be assigned.
        /// @note Only `noexcept` if LeftT is nothrow constructible from a forwarded LeftP.
        template<class LeftP,
                 DOXYLIE(ARG(class = std::enable_if_t<
                         (std::is_same<std::decay_t<LeftP>, LeftT>() && std::is_convertible<LeftT, RightT>()) ||
                         (std::is_convertible<LeftP, LeftT>() && !std::is_convertible<LeftT, RightT>())>), SFINAE),
                 DOXYLIE(ARG(class = std::enable_if_t<sizeof(LeftP) && !std::is_same<LeftT, RightT>()>), CLASH) >
        explicit Either(LeftP &&left) noexcept(DOXYLIE(noexcept(Optional<LeftT>{std::forward<LeftP>(left)}),)) :
                _left{std::forward<LeftP>(left)},
                _side{Side::left} {}

        /// @brief Creates an Either that has assigned a right value.
        /// @tparam RightP Used for perfect forwarding.
        /// @pre RightP must be either the same as RightT if it's convertible to LeftT or must be at least convertible
        /// to RightT if it isn't convertible to LeftT (SFINAE).
        /// @param right The right value to be assigned.
        /// @note Only `noexcept` if RightT is nothrow constructible from a forwarded RightP.
        template<class RightP,
                 DOXYLIE(ARG(class = std::enable_if_t<
                         (std::is_same<std::decay_t<RightP>, RightT>() && std::is_convertible<LeftT, RightT>()) ||
                         (std::is_convertible<RightP, RightT>() && !std::is_convertible<LeftT, RightT>())>), SFINAE) >
        explicit Either(RightP &&right) noexcept(DOXYLIE(noexcept(Optional<RightT>{std::forward<RightP>(right)}),)) :
                _right{std::forward<RightP>(right)},
                _side{Side::right} {}

        /// @brief Copy constructor that handles the left and right variant differently.
        /// @copyconstruct
        /// @note Only `noexcept` if LeftT and RightT are nothrow copy constructible.
        Either(Either const &other) noexcept(DOXYLIE(std::is_nothrow_copy_constructible<Optional<LeftT>>() &&
                                                     std::is_nothrow_copy_constructible<Optional<RightT>>(),)) :
                _side{other._side} {
            switch (_side) {
                case Side::left: {
                    new(&_left) Optional<LeftT>{other._left};
                    break;
                }
                case Side::right: {
                    new(&_right) Optional<RightT>{other._right};
                    break;
                }
                default:break;
            }
        }

        /// @brief Move constructor that handles the left and right variant differently.
        /// @moveconstruct
        /// @note Only `noexcept` if LeftT and RightT are nothrow move constructible, and #resetLeft and #resetRight
        /// don't throw.
        Either(Either &&other) noexcept(DOXYLIE(std::is_nothrow_move_constructible<Optional<LeftT>>() &&
                                                std::is_nothrow_move_constructible<Optional<RightT>>() &&
                                                noexcept(Either{}.resetLeft()) &&
                                                noexcept(Either{}.resetRight()),)):
                _side{other._side} {
            switch (other._side) {
                case Side::left: {
                    new(&_left) Optional<LeftT>{std::move(other._left)};
                    other.resetLeft();
                    break;
                }
                case Side::right: {
                    new(&_right) Optional<RightT>{std::move(other._right)};
                    other.resetRight();
                    break;
                }
                default:break;
            }
        }

        /// @brief Destructor that handles destruction of the left and right variant.
        /// @note Only `noexcept` if LeftT and RightT are nothrow destructible.
        ~Either() noexcept(DOXYLIE(std::is_nothrow_destructible<Optional<LeftT>>() &&
                                   std::is_nothrow_destructible<Optional<RightT>>(),)) override {
            resetLeft();
            resetRight();
        }

        /// @brief Copy assignment operator that handles the left and right variant differently.
        /// @copyassign
        /// @note Only `noexcept` if LeftT and RightT are nothrow copy constructible.
        Either &operator=(Either const &other)
        noexcept(DOXYLIE(std::is_nothrow_copy_constructible<Optional<LeftT>>() &&
                         std::is_nothrow_copy_constructible<Optional<RightT>>(),)) {
            if (this != &other) {
                _side = other._side;
                switch (_side) {
                    case Side::left: {
                        new(&_left) Optional<LeftT>{other._left};
                        break;
                    }
                    case Side::right: {
                        new(&_right) Optional<RightT>{other._right};
                        break;
                    }
                    default:break;
                }
            }
            return *this;
        }

        /// @brief Move assignment operator that handles the left and right variant differently.
        /// @moveassign
        /// @note Only `noexcept` if LeftT and RightT are nothrow move constructible, and #resetLeft and #resetRight
        /// don't throw.
        Either &operator=(Either &&other) noexcept(DOXYLIE(std::is_nothrow_move_constructible<Optional<LeftT>>() &&
                                                           std::is_nothrow_move_constructible<Optional<RightT>>() &&
                                                           noexcept(Either{}.resetLeft()) &&
                                                           noexcept(Either{}.resetRight()),)) {
            if (this != &other) {
                _side = other._side;
                switch (_side) {
                    case Side::left: {
                        new(&_left) Optional<LeftT>{std::move(other._left)};
                        other.resetLeft();
                        break;
                    }
                    case Side::right: {
                        new(&_right) Optional<RightT>{std::move(other._right)};
                        other.resetRight();
                        break;
                    }
                    default:break;
                }
            }
            return *this;
        }

    public: // getter
        /// @brief Returns the left value if there is one, otherwise throws an exception.
        /// @return The left value.
        /// @note Returns a const reference for non-scalar types of LeftT.
        /// @throw IsNotLeft (no hints, 0)
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<LeftT>>(), std::decay_t<LeftT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<LeftT>>>>), LeftT)
        left() const {
            if (!isLeft()) {
                IsNotLeft::create(0, "Either is not left.", CuteException::Severity::critical)->raise();
            }
            return _left.value();
        }

        /// @brief Returns a default left value instead of throwing an exception.
        /// @tparam LeftP Used for perfect forwarding.
        /// @param defaultLeft The default left value to return.
        /// @return The left value, or the provided default.
        /// @note Returns a const reference for non-scalar types of LeftT.
        template<class LeftP>
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<LeftT>>(), std::decay_t<LeftT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<LeftT>>>>), LeftT)
        left(LeftP &&defaultLeft) const noexcept {
            if (isLeft()) {
                return _left.value(std::forward<LeftP>(defaultLeft));
            } else {
                return std::forward<LeftP>(defaultLeft);
            }
        }

        /// @brief Used to test if it is safe to query the left value.
        /// @return True if the Either is left.
        bool isLeft() const noexcept {
            return _side == Side::left;
        }

        /// @brief Returns the right value if there is one, otherwise throws an exception.
        /// @return The right value.
        /// @note Returns a const reference for non-scalar types of RightT.
        /// @throw IsNotRight (no hints, 0)
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<RightT>>(), std::decay_t<RightT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<RightT>>>>), RightT)
        right() const {
            if (_side != Side::right) {
                IsNotRight::create(0, "Either is not right.", CuteException::Severity::critical)->raise();
            }
            return _right.value();
        }

        /// @brief Returns a default right value instead of throwing an exception.
        /// @tparam RightP Used for perfect forwarding.
        /// @param defaultRight The default right value to return.
        /// @return The value of the optional, or the provided default.
        /// @note Returns a const reference for non-scalar types of RightT.
        template<class RightP>
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<RightT>>(), std::decay_t<RightT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<RightT>>>>), RightT)
        right(RightP &&defaultRight) const noexcept {
            if (_side == Side::right) {
                return _right.value(std::forward<RightP>(defaultRight));
            } else {
                return std::forward<RightP>(defaultRight);
            }
        }

        /// @brief Used to test if it is safe to query the right value.
        /// @return True if the either is right.
        bool isRight() const noexcept {
            return _side == Side::right;
        }

        /// @copydoc #right
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<RightT>>(), std::decay_t<RightT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<RightT>>>>), RightT)
        operator*() const {
            return right();
        }

        /// @copydoc #isRight
        explicit operator bool() const noexcept {
            return isRight();
        }

    public: // setter
        /// @brief Assign a new left value to the Either, and remove its right value.
        /// @tparam LeftP Used for perfect forwarding.
        /// @param left The new left value to be set.
        /// @note Only `noexcept` if #resetRight doesn't throw and LeftT is nothrow constructible from a forwarded
        /// LeftP.
        template<class LeftP>
        void setLeft(LeftP &&left) noexcept(DOXYLIE(noexcept(Either{}.resetRight()) &&
                                                    noexcept(Optional<LeftT>{std::forward<LeftP>(left)}),)) {
            if (isLeft()) {
                _left.setValue(std::forward<LeftP>(left));
            } else {
                resetRight();
                new(&_left) Optional<LeftT>{std::forward<LeftP>(left)};
                _side = Side::left;
            }
        }

        /// @brief Assign a new right value to the Either, and remove its left value.
        /// @param right The new right value to be set.
        /// @note Only `noexcept` if #resetLeft doesn't throw and RightT is nothrow constructible from a forwarded
        /// RightP.
        template<class RightP>
        void setRight(RightP &&right) noexcept(DOXYLIE(noexcept(Either{}.resetLeft()) &&
                                                       noexcept(Optional<RightT>{std::forward<RightP>(right)}),)) {
            if (isRight()) {
                _right.setValue(std::forward<RightP>(right));
            } else {
                resetLeft();
                new(&_right) Optional<RightT>{std::forward<RightP>(right)};
                _side = Side::right;
            }
        }

    public: // reset
        /// @brief Resets the left value if there is one. If the right value is set, it remains unchanged.
        /// @note Only `noexcept` if LeftT is nothrow destructible.
        void resetLeft() noexcept(DOXYLIE(std::is_nothrow_destructible<Optional<LeftT>>(),)) {
            if (_side == Side::left) {
                _left.Optional<LeftT>::~Optional();
                _side = Side::undefined;
            }
        }

        /// @brief Resets the right value if there is one. If the left value is set, it remains unchanged.
        /// @note Only `noexcept` if RightT is nothrow destructible.
        void resetRight() noexcept(DOXYLIE(std::is_nothrow_destructible<Optional<RightT>>(),)) {
            if (_side == Side::right) {
                _right.Optional<RightT>::~Optional();
                _side = Side::undefined;
            }
        }

    public: // methods
        QSharedPointer<Cloneable> clone() const final {
            return QSharedPointer<Cloneable>{new Either{*this}};
        }

        bool equals(Either const &other) const noexcept final {
            return (!isLeft() && !other.isLeft() && !isRight() && !other.isRight()) ||
                   (isLeft() && other.isLeft() && left() == other.left()) ||
                   (isRight() && other.isRight() && right() == other.right());
        }

        QDataStream &serialize(QDataStream &stream) const final {
            stream << isLeft() << isRight();
            if (isLeft()) {
                stream << left();
            } else if (isRight()) {
                stream << right();
            }
            return stream;
        }

        QDataStream &deserialize(QDataStream &stream) final {
            bool isLeft{false}, isRight{false};
            stream >> isLeft >> isRight;
            if (isLeft) {
                LeftT left;
                stream >> left;
                setLeft(std::move(left));
            } else if (isRight) {
                RightT right;
                stream >> right;
                setRight(std::move(right));
            } else {
                resetLeft();
                resetRight();
            }
            return stream;
        }

    private: // types
        union {
            /// @private
            Optional<LeftT> _left;
            /// @private
            Optional<RightT> _right;
        };

        enum class Side :
                quint8 {
            undefined,
            left,
            right,
        } _side;
    };
}}

Q_DECLARE_METATYPE_TEMPLATE_2ARG_CUTE(CuteVR::Extension::Either)

#endif // CUTE_VR_EXTENSION_EITHER
