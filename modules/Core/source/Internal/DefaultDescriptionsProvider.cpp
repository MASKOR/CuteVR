/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QList>
#include <openvr.h>

#include <CuteVR/Internal/DefaultDescriptionsProvider.hpp>
#include <CuteVR/Internal/Property.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Description;
using Internal::DefaultDescriptionsProvider;
using Internal::Property::query;

class DefaultDescriptionsProvider::Private {
public: // constructor
    explicit Private(DefaultDescriptionsProvider *that, Identifier const device,
                     std::function<void(Description const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryDescriptions();
    }

public: // methods
    void queryDescriptions() {
        queryDescription(vr::Prop_ManufacturerName_String);
        queryDescription(vr::Prop_TrackingSystemName_String);
        // FIXME: currently unknown queryDescription(vr::Prop_HardwareName_String);
        queryDescription(vr::Prop_HardwareRevision_String);
        queryDescription(vr::Prop_ModelNumber_String);
        queryDescription(vr::Prop_SerialNumber_String);
    }

    void queryDescription(vr::ETrackedDeviceProperty const type) {
        QMap<vr::ETrackedDeviceProperty, Description::Type> const typeMapping{
                {vr::Prop_ManufacturerName_String,   Description::Type::manufacturerName},
                {vr::Prop_TrackingSystemName_String, Description::Type::trackingSystemName},
                // FIXME: currently unknown {vr::Prop_HardwareName_String, HardwareName};
                {vr::Prop_HardwareRevision_String,   Description::Type::hardwareRevision},
                {vr::Prop_ModelNumber_String,        Description::Type::modelNumber},
                {vr::Prop_SerialNumber_String,       Description::Type::serialNumber},
        };
        Description description{};
        description.identifier = static_cast<Identifier >(type);
        description.type = typeMapping.value(type, Description::Type::undefined);
        description.append(query<QString>(device, type));
        callback(description);
    }

public: // variables
    DefaultDescriptionsProvider *that;
    Identifier device{};
    std::function<void(Description const &)> callback{};
};

DefaultDescriptionsProvider::DefaultDescriptionsProvider(Identifier const device,
                                                         std::function<void(Description const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultDescriptionsProvider::~DefaultDescriptionsProvider() = default;

bool DefaultDescriptionsProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (!theEvent) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_PropertyChanged: {
            if (theEvent->trackedDeviceIndex == _private->device) {
                switch (theEvent->data.property.prop) {
                    case vr::Prop_ManufacturerName_String:
                    case vr::Prop_TrackingSystemName_String:
                        // FIXME: currently unknown vr::Prop_HardwareName_String:
                    case vr::Prop_HardwareRevision_String:
                    case vr::Prop_ModelNumber_String:
                    case vr::Prop_SerialNumber_String: {
                        _private->queryDescription(theEvent->data.property.prop);
                        return true;
                    }
                    default: return false;
                }
            } else {
                return false;
            }
        }
        default: return false;
    }
}
