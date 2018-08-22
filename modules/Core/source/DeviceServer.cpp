/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QReadWriteLock>

#include <CuteVR/Internal/Property.hpp>
#include <CuteVR/DeviceServer.hpp>

using namespace CuteVR;
using Extension::CuteException;
using Extension::Either;
using Extension::Optional;
using Internal::Property::query;

class DeviceServer::Private {
public: // constructor
    explicit Private(DeviceServer *that) :
            that{that} {}

public: // variables
    DeviceServer *that{nullptr};
    QReadWriteLock registerLock{};
    QMultiHash<QString, std::function<QSharedPointer<Device>(Identifier)>> factories;
};

DeviceServer &DeviceServer::instance() noexcept {
    static DeviceServer instance;
    return instance;
}

DeviceServer::~DeviceServer() = default;

Either<QSharedPointer<CuteException>, QSharedPointer<Device>>
DeviceServer::create(Identifier identifier) {
    // create generic names for the new device
    QString genericName;
    QString moreGenericName;
    vr::ETrackedDeviceClass deviceClass;
    DriverServer::synchronized([&] {
        deviceClass = vr::VRSystem()->GetTrackedDeviceClass(identifier);
    });

    switch (deviceClass) {
        case vr::TrackedDeviceClass_HMD: {
            genericName = "HeadMountedDisplay_Generic_1_0_0";
            moreGenericName = "HeadMountedDisplay_Generic_1_x";
            break;
        }
        case vr::TrackedDeviceClass_Controller: {
            genericName = "Controller_Generic_1_0_0";
            moreGenericName = "Controller_Generic_1_x";
            break;
        }
        case vr::TrackedDeviceClass_TrackingReference: {
            genericName = "TrackingReference_Generic_1_0_0";
            moreGenericName = "TrackingReference_Generic_1_x";
            break;
        }
        case vr::TrackedDeviceClass_GenericTracker: {
            genericName = "Tracker_Generic_1_0_0";
            moreGenericName = "Tracker_Generic_1_x";
            break;
        }
        default: {
            genericName = "Other_Generic_1_0_0";
            moreGenericName = "Other_Generic_1_x";
            break;
        }
    }

    auto &_private{instance()._private};
    QReadLocker{&_private->registerLock};

    // create specific hardware name from vendor, device name, and revision
    // TODO: GetPropErrorNameFromEnum
    auto manufacturerName{query<QString>(identifier, vr::Prop_ManufacturerName_String)};
    auto trackingSystemName{query<QString>(identifier, vr::Prop_TrackingSystemName_String)};
    auto hardwareRevision{query<QString>(identifier, vr::Prop_HardwareRevision_String)};
    QString name{QString{manufacturerName.isEmpty() ? "unknownManufacturer" : manufacturerName} + "_" +
                 QString{trackingSystemName.isEmpty() ? "unknownTrackingSystem" : trackingSystemName} + "_" +
                 QString{hardwareRevision.isEmpty() ? "unknownHardwareRevision" : hardwareRevision}};

    // search for factories that can handle the hardware, generic is fallback
    QSharedPointer<Device> device;
    for (auto const &factory : _private->factories.values(name)) {
        if ((device = factory(identifier))) {
            break;
        }
    }
    if (!device) {
        for (auto const &factory : _private->factories.values(genericName)) {
            if ((device = factory(identifier))) {
                qInfo("Only generic factory available for tracked device class '%s'.", qPrintable(name));
                break;
            }
        }
    }
    if (!device) {
        for (auto const &factory : _private->factories.values(moreGenericName)) {
            if ((device = factory(identifier))) {
                qInfo("Only major generic factory available for tracked device class '%s'.", qPrintable(name));
                break;
            }
        }
    }
    if (!device) {
        qInfo("There is no factory available for tracked device class '%s'.", qPrintable(name));
    }

    return device ? Either<QSharedPointer<CuteException>, QSharedPointer<Device>>{device}
                  : Either<QSharedPointer<CuteException>, QSharedPointer<Device>>{DeviceNotSupported::create()};
}

Optional<QSharedPointer<CuteException>>
DeviceServer::registerDevice(std::function<QSharedPointer<Device>(Identifier)> factory,
                             QStringList const &signatures) {
    if (!factory) {
        return Optional<QSharedPointer<CuteException>>{
                DeviceRegistrationFailed::create(CuteException::Severity::warning, 4000,
                                                 "Factory callback is a null pointer.")};
    }
    if (signatures.empty()) {
        return Optional<QSharedPointer<CuteException>>{
                DeviceRegistrationFailed::create(CuteException::Severity::warning, 4001,
                                                 "The factory you want to register lacks device names.")};
    }
    auto &_private{instance()._private};
    QWriteLocker{&_private->registerLock};
    for (auto const &signature : signatures) {
        // FIXME: this is some overhead
        _private->factories.insert(signature, factory);
    }
    return {};
}

DeviceServer::DeviceServer() :
        _private{new Private{this}} {}

#include "../include/CuteVR/moc_DeviceServer.cpp" // LEGACY: CMake 3.8 ignores include paths
