/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <openvr.h>

#include <CuteVR/Internal/DefaultButtonsProvider.hpp>
#include <CuteVR/Internal/Property.hpp>

using namespace CuteVR;
using Components::Input::Button;
using Extension::Trilean;
using Internal::DefaultButtonsProvider;
using Internal::Property::query;

class DefaultButtonsProvider::Private {
public: // constructor
    Private(DefaultButtonsProvider *that, Identifier const device, std::function<void(Button const &)> callback) :
            that{that},
            device{device},
            callback{std::move(callback)} {
        queryButtonIdentifiers();
        queryButtons();
    }

public: // methods
    void queryButtonIdentifiers() {
        auto const supportedButtons{query<quint64>(device, vr::Prop_SupportedButtons_Uint64)};
        for (Identifier bit = 0; bit < 64; bit++) {
            if ((supportedButtons & (1ULL << bit))) {
                buttonIdentifiers.append(bit);
            }
        }
    }

    void queryButtons() {
        for (auto const buttonIdentifier : buttonIdentifiers) {
            queryButton(buttonIdentifier);
        }
    }

    void queryButton(Identifier identifier) {
        vr::VRControllerState_t state{};
        DriverServer::synchronized([&] {
            vr::VRSystem()->GetControllerState(device, &state, (sizeof(vr::VRControllerState_t)));
        }, Trilean::yes);

        Button button{};
        button.identifier = identifier;
        button.pressed = static_cast<Trilean>((state.ulButtonPressed &
                                               vr::ButtonMaskFromId((vr::EVRButtonId) identifier)) > 0);
        button.touched = static_cast<Trilean>((state.ulButtonTouched &
                                               vr::ButtonMaskFromId((vr::EVRButtonId) identifier)) > 0);
        callback(button);
    }

public: // variables
    DefaultButtonsProvider *that{nullptr};
    Identifier device{};
    std::function<void(Button const &)> callback{};
    QList<Identifier> buttonIdentifiers{};
};

DefaultButtonsProvider::DefaultButtonsProvider(Identifier const device, std::function<void(Button const &)> callback) :
        _private{new Private{this, device, std::move(callback)}} {}

DefaultButtonsProvider::~DefaultButtonsProvider() = default;

bool DefaultButtonsProvider::handleEvent(void const *event, void const *) {
    auto const *theEvent(static_cast<vr::VREvent_t const *>(event));
    if (theEvent == nullptr) {
        return false;
    }
    switch (theEvent->eventType) {
        case vr::VREvent_ButtonPress:
        case vr::VREvent_ButtonUnpress:
        case vr::VREvent_ButtonTouch:
        case vr::VREvent_ButtonUntouch: {
            _private->queryButton(theEvent->data.controller.button);
            return true;
        }
        default: return false;
    }
}
