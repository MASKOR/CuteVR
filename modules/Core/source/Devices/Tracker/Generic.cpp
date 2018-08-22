/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Devices/Tracker/Generic.hpp>
#include <CuteVR/DeviceServer.hpp>

using namespace CuteVR;
using Components::Description;
using Configurations::Core::Feature;
using Configurations::feature;
using Devices::Tracker::Generic;

namespace {
    struct RegisterDevice {
        RegisterDevice() {
            DeviceServer::registerDevice(
                    [](Identifier const identifier) {
                        auto const enabled{ConfigurationServer::isEnabled(feature(Feature::trackerGeneric))};
                        return QSharedPointer<Device>{enabled ? new Generic{identifier} : nullptr};
                    }, QStringList{"Tracker_Generic_1_0_0", "Tracker_Generic_1_x"});
        }
    } registerDevice; // NOLINT
}

Generic::Generic(Identifier const identifier) :
        CategorizedDevice<Device::Category::tracker, TrackedDevice>(identifier) {
    Description cuteDeviceName{"Tracker_Generic_1_0_0"};
    cuteDeviceName.type = Description::Type::cuteDeviceName;
    cuteDeviceName.identifier = 1337;
    descriptions.insert(cuteDeviceName.identifier, cuteDeviceName);
}

#include "../../../include/CuteVR/Devices/Tracker/moc_Generic.cpp" // LEGACY: CMake 3.8 ignores include paths
