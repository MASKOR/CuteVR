/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DEVICE
#define CUTE_VR_DEVICE

#include <type_traits>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include <CuteVR/Components/Description.hpp>
#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Interface/Destroyable.hpp>
#include <CuteVR/Interface/EqualityComparable.hpp>
#include <CuteVR/Interface/Initializable.hpp>
#include <CuteVR/Interface/OrderComparable.hpp>
#include <CuteVR/Interface/Serializable.hpp>
#include <CuteVR/Interface/Updatable.hpp>
#include <CuteVR/Identifier.hpp>
#include <CuteVR/Macros.hpp>

namespace CuteVR {
    /// @brief The abstract base class for all devices in the Devices namespace.
    class Device :
            public QObject,
            public Interface::Destroyable,
            public Interface::EqualityComparable<Device>,
            public Interface::Initializable,
            public Interface::OrderComparable<Device>,
            public Interface::Serializable,
            public Interface::Updatable {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief Identifier to distinguish this device from the others, duplicates are unusual.
        Q_PROPERTY(CuteVR::Identifier identifier MEMBER identifier CONSTANT FINAL)
        /// @brief Categories help to group the different devices.
        Q_PROPERTY(CuteVR::Device::Category category READ category CONSTANT FINAL)
        /// @brief Contains various descriptions of the respective device.
        /// @details There is an additional signal which only emits the actually changed description.
        Q_PROPERTY(ARG(QMap<CuteVR::Identifier, CuteVR::Components::Description>) descriptions
                   MEMBER descriptions NOTIFY descriptionsChanged FINAL)

    public: // types
        /// @brief Serves to determine the category of a device.
        /// @details User-defined categories should be greater than Category::user.
        enum class Category :
                quint8 {
            undefined, ///< Default if no value has been given.
            trackingReference, ///< The device describes a reference that is used to track other devices.
            headMountedDisplay, ///< The device describes a head-mounted display.
            headMountedAudio, ///< The device describes a head-mounted audio setup.
            controller, ///< The device describes a controller.
            tracker, ///< The device describes a tracker.
            user = 100, ///< User-defined categories start after this enumerator.
        };

        Q_ENUM(Category)

    public: // constructor/destructor
        /// @brief Create a new device object which has the given identifier for its whole lifetime.
        /// @param identifier The identifier of the device.
        explicit Device(Identifier identifier = invalidIdentifier);

        ~Device() override = 0;

        Q_DISABLE_COPY(Device)

    public: // getter
        /// @brief Query the identifier of a description to which the specified type is assigned.
        /// @param type The type the description must have.
        /// @return The identifier of a description that is of the given type or nothing.
        Extension::Optional<Identifier> description(Components::Description::Type type) const noexcept;

    public: // methods
        /// @brief Queries the category of the device.
        /// @return The category of the device or undefined.
        virtual Category category() const noexcept;

        void destroy() override;

        bool isDestroyed() const noexcept override;

        bool equals(Device const &other) const noexcept override;

        void initialize() override;

        bool isInitialized() const noexcept override;

        qint32 orders(Device const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

        void update() override;

        bool isCurrent() const noexcept override;

    public: // variables
        CuteVR::Identifier const identifier{};
        QMap<CuteVR::Identifier, CuteVR::Components::Description> descriptions{};

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{map of descriptions}
        void descriptionsChanged(QMap<CuteVR::Identifier, CuteVR::Components::Description>);

        /// @signal{individual description}
        void descriptionChanged(CuteVR::Identifier, CuteVR::Components::Description);
    };

    /// @brief Adds a categorized hierarchy layer to a Device to enable polymorphism within a specific domain.
    /// @details The class inherits from itself with the `void` specialization, that serves as a kind of tag. This
    /// allows polymorphism within a domain even if the base classes are not the same.
    /// @extends Device
    /// @tparam CategoryE The category which the device belongs to.
    /// @tparam BaseT The class which will be inherited from.
    /// @pre BaseT extends Device.
    template<Device::Category CategoryE, class BaseT = void>
    class CategorizedDevice :
            public BaseT
            DOXYLIE(ARG(, public CategorizedDevice<CategoryE>),) {
        static_assert(std::is_base_of<Device, BaseT>(), "Base class must be a device.");
    public: // constructor/destructor
        using BaseT::BaseT;

        ~CategorizedDevice() override = default;

    public: // methods
        constexpr Device::Category category() const noexcept final {
            return CategoryE;
        }
    };

    /// @private
    template<Device::Category CategoryE>
    class CategorizedDevice<CategoryE, void> {};

    /// @ostream{device category}
    QDataStream &operator<<(QDataStream &stream, Device::Category category);

    /// @istream{device category}
    QDataStream &operator>>(QDataStream &stream, Device::Category &category);

    /// @ostream{shared pointer of a device}
    QDataStream &operator<<(QDataStream &stream, QSharedPointer<Device> const &serializable);

    /// @istream{shared pointer of a device}
    QDataStream &operator>>(QDataStream &stream, QSharedPointer<Device> &serializable);
}

Q_DECLARE_METATYPE(CuteVR::Device::Category)

Q_DECLARE_METATYPE(QSharedPointer<CuteVR::Device>)

#endif // CUTE_VR_DEVICE
