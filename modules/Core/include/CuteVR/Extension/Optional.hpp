/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_EXTENSION_OPTIONAL
#define CUTE_VR_EXTENSION_OPTIONAL

#include <type_traits>
#include <utility>
#include <QtCore/QMetaType>

#include <CuteVR/Extension/CuteException.hpp>
#include <CuteVR/Interface/Cloneable.hpp>
#include <CuteVR/Macros.hpp>

namespace CuteVR { namespace Extension {
    /// @brief Allows optional values in a readable form without using pointers and such.
    /// @attention This class may be replaced by std::optional as soon as C++17 is used. Furthermore, this
    /// implementation is very rudimentary and presumably not as good for all applications as the C++ standard
    /// implementation. Also the class can not handle raw pointers appropriately.
    /// @tparam ValueT The type of the optional value.
    /// @pre ValueT is copy and move constructible, destructible and not a pointer.
    template<class ValueT>
    class Optional final :
            public Interface::Cloneable,
            public Interface::EqualityComparable<Extension::Optional<ValueT>>,
            public Interface::Serializable {
        static_assert(std::is_copy_constructible<ValueT>(), "Values wrapped in Optional must be copy constructible.");
        static_assert(std::is_move_constructible<ValueT>(), "Values wrapped in Optional must be move constructible.");
        static_assert(std::is_destructible<ValueT>(), "Values wrapped in Optional must be destructible.");
        static_assert(!std::is_pointer<ValueT>(), "Optional cannot handle raw pointers appropriately.");

    public: // types
        /// @brief Optional has no value set but the getter method was called without providing a default value.
        struct HasNoValue final :
                public CuriousCuteException<HasNoValue> {
        };

    public: // constructor/destructor/assignment
        /// @default
        Optional() noexcept :
                _nothing{true},
                _hasValue{false} {}

        /// @brief Initializes an Optional that already has a value set.
        /// @tparam ValueP Used for perfect forwarding.
        /// @pre ValueP must be convertible to ValueT (SFINAE).
        /// @param value The value to be set.
        /// @note Only `noexcept` if ValueT is nothrow constructible from a forwarded ValueP.
        template<class ValueP,
                 DOXYLIE(ARG(class = std::enable_if_t<sizeof(ValueP) && std::is_convertible<ValueP, ValueT>()>),
                         SFINAE) >
        explicit Optional(ValueP &&value) noexcept(DOXYLIE(noexcept(ValueT{std::forward<ValueP>(value)}),)):
                _value{std::forward<ValueP>(value)},
                _hasValue{true} {}

        /// @brief Copy constructor that handles set and non-set values differently.
        /// @copyconstruct
        /// @note Only `noexcept` if #setValue doesn't throw.
        Optional(Optional const &other) noexcept(DOXYLIE(noexcept(Optional{}.setValue(ValueT{})),)) {
            if (other._hasValue) {
                setValue(other._value);
            }
        }

        /// @brief Move constructor that handles set and non-set values differently.
        /// @moveconstruct
        /// @note Only `noexcept` if #resetValue and #setValue don't throw.
        Optional(Optional &&other) noexcept(DOXYLIE(noexcept(Optional{}.resetValue()) &&
                                                    noexcept(Optional{}.setValue(std::move(ValueT{}))),)) {
            if (other._hasValue) {
                setValue(std::move(other._value));
            }
            other.resetValue();
        }

        /// @brief Destructor that handles destruction of set and non-set values.
        /// @note Only `noexcept` if ValueT is nothrow destructible.
        ~Optional() noexcept(DOXYLIE(std::is_nothrow_destructible<ValueT>(),)) override {
            resetValue();
        };

        /// @brief Copy assignment operator that handles set and non-set values differently.
        /// @copyassign
        /// @note Only `noexcept` if #resetValue and #setValue don't throw.
        Optional &operator=(Optional const &other) noexcept(DOXYLIE(noexcept(Optional{}.resetValue()) &&
                                                                    noexcept(Optional{}.setValue(ValueT{})),)) {
            if (this != &other) {
                resetValue();
                if (other._hasValue) {
                    setValue(other._value);
                }
            }
            return *this;
        }

        /// @brief Move assignment operator that handles set and non-set values differently.
        /// @moveassign
        /// @note Only `noexcept` if #resetValue and #setValue don't throw.
        Optional &operator=(Optional &&other) noexcept(DOXYLIE(noexcept(Optional{}.resetValue()) &&
                                                               noexcept(Optional{}.setValue(std::move(ValueT{}))),)) {
            if (this != &other) {
                resetValue();
                if (other._hasValue) {
                    setValue(std::move(other._value));
                }
                other.resetValue();
            }
            return *this;
        }

    public: // getter
        /// @brief Returns the value of the Optional if there is one, otherwise throws an exception.
        /// @return The current value if there is one.
        /// @note Returns a const reference for non-scalar types of ValueT.
        /// @throw HasNoValue (no hints, 0)
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<ValueT>>(), std::decay_t<ValueT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<ValueT>>>>), ValueT)
        value() const {
            if (!_hasValue) {
                HasNoValue::create(0, "Optional has no value.", CuteException::Severity::critical)->raise();
            }
            return _value;
        }

        /// @brief Returns a default value instead of throwing an exception.
        /// @tparam ValueP Used for perfect forwarding.
        /// @param defaultValue The default value to return if there is no value.
        /// @return The value, or the provided default.
        /// @note Returns a const reference for non-scalar types of ValueT.
        template<class ValueP>
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<ValueT>>(), std::decay_t<ValueT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<ValueT>>>>), ValueT)
        value(ValueP &&defaultValue) const noexcept {
            if (!_hasValue) {
                return std::forward<ValueP>(defaultValue);
            }
            return _value;
        }

        /// @brief Used to test if it's safe to query a value from an Optional.
        /// @return True if a value is set.
        bool hasValue() const noexcept {
            return _hasValue;
        }

        /// @copydoc #value
        DOXYLIE(ARG(std::conditional_t<!!std::is_scalar<std::decay_t<ValueT>>(), std::decay_t<ValueT>,
                                       std::add_lvalue_reference_t<std::add_const_t<std::decay_t<ValueT>>>>), ValueT)
        operator*() const {
            return value();
        }

        /// @copydoc #hasValue
        explicit operator bool() const noexcept {
            return hasValue();
        }

    public: // setter
        /// @setter{value}
        /// @tparam ValueP Used for perfect forwarding.
        /// @note Only `noexcept` if #resetValue doesn't throw and ValueT is nothrow constructible from a forwarded
        /// ValueP.
        template<class ValueP>
        void setValue(ValueP &&value) noexcept(DOXYLIE(noexcept(Optional{}.resetValue()) &&
                                                       noexcept(ValueT{std::forward<ValueP>(value)}),)) {
            resetValue();
            new(&_value) ValueT{std::forward<ValueP>(value)};
            _hasValue = true;
        }

    public: // reset
        /// @reset{value}
        /// @note Only `noexcept` if ValueT is nothrow destructible.
        void resetValue() noexcept(DOXYLIE(std::is_nothrow_destructible<ValueT>(),)) {
            if (_hasValue) {
                _value.ValueT::~ValueT();
            }
            _nothing = true;
            _hasValue = false;
        }

    public: // methods
        QSharedPointer<Cloneable> clone() const final {
            return QSharedPointer<Cloneable>{new Optional{*this}};
        }

        bool equals(Optional const &other) const noexcept final {
            return (!hasValue() && !other.hasValue()) ||
                   (hasValue() && other.hasValue() && value() == other.value());
        }

        QDataStream &serialize(QDataStream &stream) const final {
            stream << hasValue();
            if (hasValue()) {
                stream << value();
            }
            return stream;
        }

        QDataStream &deserialize(QDataStream &stream) final {
            bool hasValue{false};
            stream >> hasValue;
            if (hasValue) {
                ValueT value;
                stream >> value;
                setValue(std::move(value));
            } else {
                resetValue();
            }
            return stream;
        }

    private: // types
        union {
            /// @private
            bool _nothing{true}; // If this is used, it is only to make the code more meaningful.
            /// @private
            ValueT _value;
        };

    private: // variables
        bool _hasValue{false};
    };
}}

Q_DECLARE_METATYPE_TEMPLATE_1ARG_CUTE(CuteVR::Extension::Optional)

#endif // CUTE_VR_EXTENSION_OPTIONAL
