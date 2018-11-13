/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Configurations/Core.hpp>

using namespace CuteVR;
using Configurations::Core::Feature;
using Configurations::Core::Parameter;
using Configurations::feature;
using Configurations::parameter;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Feature>();
            qRegisterMetaType<Parameter>();
        }
    } registerMetaTypes; // NOLINT

    struct RegisterCoreConfiguration {
        RegisterCoreConfiguration() {
            // TODO: add validators
            // general features
            ConfigurationServer::registerFeature(feature(Feature::tracking), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::events), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::eventTracking), false, true, true);
            ConfigurationServer::registerFeature(feature(Feature::cell), true, true, false);
            ConfigurationServer::registerFeature(feature(Feature::multiCell), false, false, false);
            ConfigurationServer::registerFeature(feature(Feature::equipment), true, true, false);
            ConfigurationServer::registerFeature(feature(Feature::multiEquipment), false, false, false);
            ConfigurationServer::registerFeature(feature(Feature::smartFocus), false, false, false);
            ConfigurationServer::registerFeature(feature(Feature::forcedFocus), false, false, false);
            ConfigurationServer::registerFeature(feature(Feature::linearVelocity), false, true, true);
            ConfigurationServer::registerFeature(feature(Feature::linearAcceleration), false, true, false);
            ConfigurationServer::registerFeature(feature(Feature::angularVelocity), false, true, true);
            ConfigurationServer::registerFeature(feature(Feature::angularAcceleration), false, true, false);
            // device features
            ConfigurationServer::registerFeature(feature(Feature::inhibitDeviceRegistration), false, true, false);
            ConfigurationServer::registerFeature(feature(Feature::trackingReferenceGeneric), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::headMountedDisplayGeneric), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::headMountedAudioGeneric), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::controllerGeneric), true, true, true);
            ConfigurationServer::registerFeature(feature(Feature::trackerGeneric), true, true, true);
            // render features
            ConfigurationServer::registerFeature(feature(Feature::drawing), false, true, true);
            // general parameters
            ConfigurationServer::registerParameter(parameter(Parameter::driverLockWarn), {5000}, QVariant::UInt);
            ConfigurationServer::registerParameter(parameter(Parameter::driverLockAbort), {5000}, QVariant::UInt);
            // render parameters
            ConfigurationServer::registerParameter(parameter(Parameter::zNear), {0.01}, QVariant::Double);
            ConfigurationServer::registerParameter(parameter(Parameter::zFar), {1000.0}, QVariant::Double);
        }
    } registerCoreConfiguration; // NOLINT
}

namespace CuteVR { namespace Configurations { namespace Core {
    QDataStream &operator<<(QDataStream &stream, Feature coreFeature) {
        return stream << static_cast<quint8>(coreFeature);
    }

    QDataStream &operator>>(QDataStream &stream, Feature &coreFeature) {
        return stream >> reinterpret_cast<quint8 &>(coreFeature);
    }

    QDataStream &operator<<(QDataStream &stream, Parameter coreParameter) {
        return stream << static_cast<quint8>(coreParameter);
    }

    QDataStream &operator>>(QDataStream &stream, Parameter &coreParameter) {
        return stream >> reinterpret_cast<quint8 &>(coreParameter);
    }
}}}

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0) // LEGACY: Qt 5.7 has no Q_NAMESPACE/Q_ENUM_NS prevents MOC note
#include "../../include/CuteVR/Configurations/moc_Core.cpp" // LEGACY: CMake 3.8 ignores include paths
#endif
