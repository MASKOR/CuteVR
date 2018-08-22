/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_DESCRIPTION
#define CUTE_VR_COMPONENTS_DESCRIPTION

#include <QtCore/QString>

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components {
    /// @brief A Description is a conceptual Component used to describe a Device in more detail.
    struct Description final :
            public Component,
            public QString,
            public Interface::EqualityComparable<Components::Description> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The type that this description has.
        Q_PROPERTY(CuteVR::Components::Description::Type type MEMBER type FINAL)

    public: // types
        /// @brief Serves to specify the type of the description.
        /// @details User-defined types should be greater than Type::user.
        enum class Type :
                quint8 {
            undefined, ///< Default if no value has been given.
            cuteDeviceName, ///< The name of the CuteVR device type used to represent the hardware.
            manufacturerName, ///< The name of the manufacturer.
            trackingSystemName, ///< The name of the tracking system in general.
            hardwareName, ///< The name of the hardware device.
            hardwareRevision, ///< The revision of the hardware device.
            modelNumber, ///< The model number of the device.
            serialNumber, ///< The serial number of the device.
            user = 100, ///< User-defined types start after this enumerator.
        };

        Q_ENUM(Type)

    public: // constructor/destructor
        using QString::QString;

        ~Description() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        bool equals(Description const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Components::Description::Type type{Type::undefined};
    };

    /// @ostream{description}
    QDataStream &operator<<(QDataStream &stream, Description const &description);

    /// @istream{description}
    QDataStream &operator>>(QDataStream &stream, Description &description);

    /// @ostream{description type}
    QDataStream &operator<<(QDataStream &stream, Description::Type type);

    /// @istream{description type}
    QDataStream &operator>>(QDataStream &stream, Description::Type &type);
}}

Q_DECLARE_METATYPE(CuteVR::Components::Description)

Q_DECLARE_METATYPE(CuteVR::Components::Description::Type)

#endif // CUTE_VR_COMPONENTS_DESCRIPTION
