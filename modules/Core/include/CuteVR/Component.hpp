/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENT
#define CUTE_VR_COMPONENT

#include <type_traits>
#include <QtCore/QObject>

#include <CuteVR/Interface/Cloneable.hpp>
#include <CuteVR/Interface/EqualityComparable.hpp>
#include <CuteVR/Interface/OrderComparable.hpp>
#include <CuteVR/Interface/Serializable.hpp>
#include <CuteVR/Identifier.hpp>
#include <CuteVR/Macros.hpp>

namespace CuteVR {
    /// @brief The abstract base class for all components in the Components namespace.
    struct Component :
            public Interface::Cloneable,
            public Interface::EqualityComparable<Component>,
            public Interface::OrderComparable<Component>,
            public Interface::Serializable {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief Identifier to distinguish this component from the others, duplicates may occur.
        Q_PROPERTY(CuteVR::Identifier identifier MEMBER identifier FINAL)
        /// @brief Categories help to group the different components.
        Q_PROPERTY(CuteVR::Component::Category category READ category CONSTANT FINAL)

    public: // types
        /// @brief Serves to determine the category of a component.
        /// @details User-defined categories should be greater than Category::user.
        enum class Category :
                quint8 {
            undefined, ///< Default if no value has been given.
            geometry, ///< The component is the description of a geometric form.
            input, ///< The component is the abstraction of a hardware that is used for user input.
            interaction, ///< The component describes an interaction opportunity with the user himself.
            output, ///< The component is the abstraction of a hardware that is used for user output.
            peripheral, ///< The component describes a peripheral device that does not interact with the user directly.
            sensor, ///< The component describes a sensor.
            user = 100, ///< User-defined categories start after this enumerator.
        };

        Q_ENUM(Category)

    public: // destructor
        ~Component() override = 0;

    public: // methods
        /// @brief Queries the category of the component.
        /// @return The category of the component or Category::undefined.
        virtual Category category() const noexcept;

        bool equals(Component const &other) const noexcept override;

        qint32 orders(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Identifier identifier{invalidIdentifier};
    };

    inline Component::~Component() = default;

    /// @brief Adds a categorized hierarchy layer to a component to enable polymorphism within a specific domain.
    /// @details The class inherits from itself with the `void` specialization, that serves as a kind of tag. This
    /// allows polymorphism within a domain even if the base classes are not the same.
    /// @extends Component
    /// @tparam CategoryE The category which the component belongs to.
    /// @tparam BaseT The class which will be inherited from.
    /// @pre BaseT extends Component.
    template<Component::Category CategoryE, class BaseT = void>
    struct CategorizedComponent :
            public BaseT
            DOXYLIE(ARG(, public CategorizedComponent<CategoryE>),) {
        static_assert(std::is_base_of<Component, BaseT>(), "Base class must be a component.");

        constexpr Component::Category category() const noexcept final {
            return CategoryE;
        }
    };

    /// @private
    template<Component::Category CategoryE>
    struct CategorizedComponent<CategoryE, void> {
        virtual ~CategorizedComponent() = default;
    };

    /// @ostream{component category}
    QDataStream &operator<<(QDataStream &stream, Component::Category category);

    /// @istream{component category}
    QDataStream &operator>>(QDataStream &stream, Component::Category &category);
}

Q_DECLARE_METATYPE(CuteVR::Component::Category)

#endif // CUTE_VR_COMPONENT
