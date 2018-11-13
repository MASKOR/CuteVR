/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_CONFIGURATIONS_CORE
#define CUTE_VR_CONFIGURATIONS_CORE

#include <CuteVR/ConfigurationServer.hpp>

namespace CuteVR { namespace Configurations {
    /// @brief The configuration and initialization routine of the core module.
    namespace Core {
        /// @brief Used to describe all features that are included in the core module and can be configured.
        enum class Feature :
                quint16 {
            undefined = ///< Default if no value has been given.
                    static_cast<quint16>(ConfigurationServer::Feature::undefined),
            tracking = ///< Tracking information can be polled, queried and evaluated.
                    ConfigurationServer::generalCore + 1,
            events, ///< Events can be polled, queried and evaluated.
            eventTracking, ///< A reliable, time-accurate pose per event is available. Requires events and tracking.
            cell, ///< Reliable cell information is provided.
            multiCell, ///< Reliable information for setups with multiple cells is available. Requires cell.
            equipment, ///< Reliable information about equipment is provided.
            multiEquipment, ///< Reliable information for multi-equipment setups is available. Requires equipment.
            smartFocus, ///< It can be smartly decided if focusing e.g. on events is reasonable. Conflicts forced focus.
            forcedFocus, ///< It can be forced to keep the focus on events. Conflicts smart focus.
            linearVelocity, ///< Tracking information is enriched with data about the linear acceleration.
            linearAcceleration, ///< Tracking information is enriched with data about the linear acceleration.
            angularVelocity, ///< Tracking information is enriched with data about the angular velocity.
            angularAcceleration, ///< Tracking information is enriched with data about the angular acceleration.
            inhibitDeviceRegistration = ///< All devices of this module will no longer register automatically.
                    ConfigurationServer::deviceCore + 1,
            trackingReferenceGeneric, ///< Generic tracking reference implementation.
            headMountedDisplayGeneric, ///< Generic head mounted display implementation.
            headMountedAudioGeneric, ///< Generic head mounted audio implementation.
            controllerGeneric, ///< Generic controller implementation.
            trackerGeneric, ///< Generic tracker implementation.
            drawing = ///< It is possible to send rendered frames to connected head-mounted displays.
                    ConfigurationServer::renderCore + 1,
        };

        /// @brief Used to describe all parameters that are included in the core module and can be configured.
        enum class Parameter :
                quint16 {
            undefined = ///< Default if no value has been given.
                    static_cast<quint16>(ConfigurationServer::Feature::undefined),
            driverLockWarn = ///< The time in milliseconds until the driver lock attempt generates a warning.
                    ConfigurationServer::generalCore + 1,
            driverLockAbort, ///< The time in milliseconds until the driver lock attempt throws a critical exception.
            zNear = ///< The minimum viewing distance of the eyes that is used in the projection matrix.
                    ConfigurationServer::renderCore + 1,
            zFar, ///< The maximum viewing distance of the eyes that is used in the projection matrix.
        };

        /// @ostream{core feature}
        QDataStream &operator<<(QDataStream &stream, Feature feature);

        /// @istream{core feature}
        QDataStream &operator>>(QDataStream &stream, Feature &feature);

        /// @ostream{core parameter}
        QDataStream &operator<<(QDataStream &stream, Parameter parameter);

        /// @istream{core parameter}
        QDataStream &operator>>(QDataStream &stream, Parameter &parameter);

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0) // LEGACY: Qt 5.7 has no Q_NAMESPACE/Q_ENUM_NS
        Q_NAMESPACE

        Q_ENUM_NS(Feature)

        Q_ENUM_NS(Parameter)
#endif
    }

    /// @brief Converts a Core::Feature to a ConfigurationServer::Feature.
    constexpr ConfigurationServer::Feature feature(Core::Feature feature) noexcept {
        return static_cast<ConfigurationServer::Feature>(feature);
    }


    /// @brief Converts a Core::Parameter to a ConfigurationServer::Parameter.
    constexpr ConfigurationServer::Parameter parameter(Core::Parameter parameter) noexcept {
        return static_cast<ConfigurationServer::Parameter >(parameter);
    }
}}

Q_DECLARE_METATYPE(CuteVR::Configurations::Core::Feature)

Q_DECLARE_METATYPE(CuteVR::Configurations::Core::Parameter)

#endif // CUTE_VR_CONFIGURATIONS_CORE
