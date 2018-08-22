/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Input/Axis.hpp>
#include <CuteVR/Internal/DefaultDisplaysProvider.hpp>
#include <CuteVR/Internal/Property.hpp>

using namespace CuteVR;
using Components::Output::Display;
using Extension::Trilean;
using Internal::DefaultDisplaysProvider;
using Internal::Property::query;

class DefaultDisplaysProvider::Private {
public: // constructor
    explicit Private(DefaultDisplaysProvider *that, Identifier const device,
                     std::function<void(Display const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryDisplays();
    }

public: // methods
    void queryDisplays() {
        Display display{};
        display.refreshRate = query<float>(device, vr::Prop_DisplayFrequency_Float);
        DriverServer::synchronized([&] {
            vr::VRSystem()->GetRecommendedRenderTargetSize((quint32 *) &display.resolution.rwidth(),
                                                           (quint32 *) &display.resolution.rheight());
        }, Trilean::yes);
        callback(display);
    }

public: // variables
    DefaultDisplaysProvider *that{nullptr};
    Identifier device{};
    std::function<void(Display const &)> callback{};
};

DefaultDisplaysProvider::DefaultDisplaysProvider(Identifier const device,
                                                 std::function<void(Display const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {
}

DefaultDisplaysProvider::~DefaultDisplaysProvider() = default;

bool DefaultDisplaysProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (!theEvent) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_PropertyChanged: {
            if (theEvent->trackedDeviceIndex == _private->device) {
                switch (theEvent->data.property.prop) {
                    case vr::Prop_DisplayFrequency_Float: {
                        _private->queryDisplays();
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
