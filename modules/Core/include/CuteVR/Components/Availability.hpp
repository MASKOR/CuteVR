/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_AVAILABILITY
#define CUTE_VR_COMPONENTS_AVAILABILITY

#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components {
    /// @brief An Availability is kind of a conceptual Component that provides information about the status of a Device.
    struct Availability final :
            public Component {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The device is connected to the VR system, whereby both wireless and wired connections are possible.
        Q_PROPERTY(CuteVR::Extension::Trilean connected MEMBER connected FINAL)
        /// @brief The device was detected via the tracking capabilities of the VR system.
        Q_PROPERTY(CuteVR::Extension::Trilean detected MEMBER detected FINAL)
        /// @brief The device was calibrated using the tracking capabilities of the VR system.
        Q_PROPERTY(CuteVR::Extension::Trilean calibrated MEMBER calibrated FINAL)
        /// @brief The device is located within the tracked area and is therefore visible to the VR system.
        Q_PROPERTY(CuteVR::Extension::Trilean visible MEMBER visible FINAL)

    public: // destructor
        ~Availability() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Trilean connected{Extension::maybe};
        CuteVR::Extension::Trilean detected{Extension::maybe};
        CuteVR::Extension::Trilean calibrated{Extension::maybe};
        CuteVR::Extension::Trilean visible{Extension::maybe};
    };
}}

Q_DECLARE_METATYPE(CuteVR::Components::Availability)

#endif // CUTE_VR_COMPONENTS_AVAILABILITY
