/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DEVICES_TRACKED_DEVICE
#define CUTE_VR_DEVICES_TRACKED_DEVICE

#include <CuteVR/Components/Availability.hpp>
#include <CuteVR/Components/Pose.hpp>
#include <CuteVR/Device.hpp>

namespace CuteVR { namespace Devices {
    /// @brief This abstract class provides both a 3D pose and availability information and can
    /// thus be used as a base class for all kinds of tracked devices.
    class TrackedDevice :
            public Device {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief A set of availability information about this tracked device.
        Q_PROPERTY(CuteVR::Components::Availability availability MEMBER availability NOTIFY availabilityChanged FINAL)
        /// @brief The 3D pose of this tracked device including speed and acceleration vectors.
        /// @details Information about angular and linear velocity and acceleration is only available if enabled in the
        /// configuration.
        Q_PROPERTY(CuteVR::Components::Pose pose MEMBER pose NOTIFY poseChanged FINAL)

    public: // constructor/destructor
        explicit TrackedDevice(Identifier identifier);

        ~TrackedDevice() override = 0;

        Q_DISABLE_COPY(TrackedDevice)

    public: // methods
        void destroy() override;

        bool isDestroyed() const noexcept override;

        void initialize() override;

        bool isInitialized() const noexcept override;

        void update() override;

        bool isCurrent() const noexcept override;

    public: // variables
        CuteVR::Components::Availability availability;
        CuteVR::Components::Pose pose;

    private: // types
        class Private;

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{availability}
        void availabilityChanged(CuteVR::Components::Availability);

        /// @signal{pose}
        void poseChanged(CuteVR::Components::Pose);
    };
}}

#endif // CUTE_VR_DEVICES_TRACKED_DEVICE
