/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Configurations/Core.hpp>
#include <CuteVR/Emulator/OpenVR.hpp>
#include <CuteVR/Internal/Property.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Configurations::Core::Feature;
using Configurations::feature;
using Emulator::OpenVR::vrSystem;
using Internal::Property::query;

class PropertyTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void initTestCase() {
        Emulator::OpenVR::invoke();
        vr::init_data.eApplicationType = vr::VRApplication_Background;
        ConfigurationServer::disable(feature(Feature::cell));
        DriverServer::instance().initialize();
    }

    void queryBool_PreparedSuccess_ReturnsExpected() {
        vrSystem.getBoolTrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getBoolTrackedDeviceProperty_data.prop = vr::Prop_NeverTracked_Bool;
        vrSystem.getBoolTrackedDeviceProperty_data.returns = true;
        QVERIFY(Internal::Property::query<bool>(5, vr::Prop_NeverTracked_Bool, nullptr));
    }

    void queryBool_PreparedError_ReturnsExpected() {
        vrSystem.getBoolTrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getBoolTrackedDeviceProperty_data.prop = vr::Prop_ContainsProximitySensor_Bool;
        vrSystem.getBoolTrackedDeviceProperty_data.pError = vr::TrackedProp_InvalidDevice;
        vrSystem.getBoolTrackedDeviceProperty_data.returns = false;
        vr::ETrackedPropertyError error{};
        QVERIFY(!Internal::Property::query<bool>(7, vr::Prop_ContainsProximitySensor_Bool, &error));
        QVERIFY(error == vr::TrackedProp_InvalidDevice);
    }

    void queryFloat_PreparedSuccess_ReturnsExpected() {
        vrSystem.getFloatTrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getFloatTrackedDeviceProperty_data.prop = vr::Prop_DisplayFrequency_Float;
        vrSystem.getFloatTrackedDeviceProperty_data.returns = 3.1415f;
        QVERIFY(Internal::Property::query<float>(5, vr::Prop_DisplayFrequency_Float, nullptr) == 3.1415f);
    }

    void queryFloat_PreparedError_ReturnsExpected() {
        vrSystem.getFloatTrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getFloatTrackedDeviceProperty_data.prop = vr::Prop_DeviceBatteryPercentage_Float;
        vrSystem.getFloatTrackedDeviceProperty_data.pError = vr::TrackedProp_UnknownProperty;
        vrSystem.getFloatTrackedDeviceProperty_data.returns = 0.0f;
        vr::ETrackedPropertyError error{};
        QVERIFY(Internal::Property::query<float>(7, vr::Prop_DeviceBatteryPercentage_Float, &error) == 0.0f);
        QVERIFY(error == vr::TrackedProp_UnknownProperty);
    }

    void queryQInt32_PreparedSuccess_ReturnsExpected() {
        vrSystem.getInt32TrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getInt32TrackedDeviceProperty_data.prop = vr::Prop_DeviceClass_Int32;
        vrSystem.getInt32TrackedDeviceProperty_data.returns = 123;
        QVERIFY(Internal::Property::query<qint32>(5, vr::Prop_DeviceClass_Int32, nullptr) == 123);
    }

    void queryQInt32_PreparedError_ReturnsExpected() {
        vrSystem.getInt32TrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getInt32TrackedDeviceProperty_data.prop = vr::Prop_NumCameras_Int32;
        vrSystem.getInt32TrackedDeviceProperty_data.pError = vr::TrackedProp_PermissionDenied;
        vrSystem.getInt32TrackedDeviceProperty_data.returns = 0;
        vr::ETrackedPropertyError error{};
        QVERIFY(Internal::Property::query<qint32>(7, vr::Prop_NumCameras_Int32, &error) == 0);
        QVERIFY(error == vr::TrackedProp_PermissionDenied);
    }

    void queryQUInt64_PreparedSuccess_ReturnsExpected() {
        vrSystem.getUint64TrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getUint64TrackedDeviceProperty_data.prop = vr::Prop_DongleVersion_Uint64;
        vrSystem.getUint64TrackedDeviceProperty_data.returns = 12345;
        QVERIFY(Internal::Property::query<quint64>(5, vr::Prop_DongleVersion_Uint64, nullptr) == 12345);
    }

    void queryQUInt64_PreparedError_ReturnsExpected() {
        vrSystem.getUint64TrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getUint64TrackedDeviceProperty_data.prop = vr::Prop_FirmwareVersion_Uint64;
        vrSystem.getUint64TrackedDeviceProperty_data.pError = vr::TrackedProp_NotYetAvailable;
        vrSystem.getUint64TrackedDeviceProperty_data.returns = 0;
        vr::ETrackedPropertyError error{};
        QVERIFY(Internal::Property::query<quint64>(7, vr::Prop_FirmwareVersion_Uint64, &error) == 0);
        QVERIFY(error == vr::TrackedProp_NotYetAvailable);
    }

    void queryQMatrix4x3_PreparedSuccess_ReturnsExpected() {
        vrSystem.getMatrix34TrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getMatrix34TrackedDeviceProperty_data.prop = vr::Prop_CameraToHeadTransform_Matrix34;
        vrSystem.getMatrix34TrackedDeviceProperty_data.returns = {{
                                                                          {0.0f, 1.0f, 2.0f, 3.0f},
                                                                          {3.0f, 4.0f, 5.0f, 6.0f},
                                                                          {6.0f, 7.0f, 8.0f, 9.0f}}};
        QVERIFY(Internal::Property::query<QMatrix4x3>(5, vr::Prop_CameraToHeadTransform_Matrix34, nullptr) ==
                Internal::Matrix3x4::from(vrSystem.getMatrix34TrackedDeviceProperty_data.returns));
    }

    void queryQMatrix4x3_PreparedError_ReturnsExpected() {
        vrSystem.getMatrix34TrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getMatrix34TrackedDeviceProperty_data.prop = vr::Prop_ImuToHeadTransform_Matrix34;
        vrSystem.getMatrix34TrackedDeviceProperty_data.pError = vr::TrackedProp_ValueNotProvidedByDevice;
        vrSystem.getMatrix34TrackedDeviceProperty_data.returns = {{
                                                                          {9.0f, 8.0f, 7.0f, 6.0f},
                                                                          {6.0f, 5.0f, 4.0f, 3.0f},
                                                                          {3.0f, 2.0f, 1.0f, 0.0f}}};
        vr::ETrackedPropertyError error{};
        QVERIFY(Internal::Property::query<QMatrix4x3>(7, vr::Prop_ImuToHeadTransform_Matrix34, &error) ==
                Internal::Matrix3x4::from(vrSystem.getMatrix34TrackedDeviceProperty_data.returns));
        QVERIFY(error == vr::TrackedProp_ValueNotProvidedByDevice);
    }

    void queryQMatrix4x4_PreparedSuccess_ReturnsExpected() {
        vrSystem.getMatrix34TrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getMatrix34TrackedDeviceProperty_data.prop = vr::Prop_NeverTracked_Bool;
        vrSystem.getMatrix34TrackedDeviceProperty_data.returns = {{
                                                                          {0.0f, 2.0f, 4.0f, 6.0f},
                                                                          {1.0f, 3.0f, 5.0f, 7.0f},
                                                                          {2.0f, 4.0f, 6.0f, 8.0f}}};
        QVERIFY(Internal::Property::query<QMatrix4x4>(5, vr::Prop_NeverTracked_Bool, nullptr) ==
                Internal::Matrix4x4::from(vrSystem.getMatrix34TrackedDeviceProperty_data.returns));
    }

    void queryQMatrix4x4_PreparedError_ReturnsExpected() {
        vrSystem.getMatrix34TrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getMatrix34TrackedDeviceProperty_data.prop = vr::Prop_ContainsProximitySensor_Bool;
        vrSystem.getMatrix34TrackedDeviceProperty_data.pError = vr::TrackedProp_InvalidDevice;
        vrSystem.getMatrix34TrackedDeviceProperty_data.returns = {{
                                                                          {8.0f, 6.0f, 4.0f, 2.0f},
                                                                          {7.0f, 5.0f, 3.0f, 1.0f},
                                                                          {6.0f, 4.0f, 2.0f, 0.0f}}};
        vr::ETrackedPropertyError error;
        QVERIFY(Internal::Property::query<QMatrix4x4>(7, vr::Prop_ContainsProximitySensor_Bool, &error) ==
                Internal::Matrix4x4::from(vrSystem.getMatrix34TrackedDeviceProperty_data.returns));
        QVERIFY(error == vr::TrackedProp_InvalidDevice);
    }

    void queryQString_PreparedSuccess_ReturnsExpected() {
        vrSystem.getStringTrackedDeviceProperty_data.unDeviceIndex = 5;
        vrSystem.getStringTrackedDeviceProperty_data.prop = vr::Prop_ControllerType_String;
        vrSystem.getStringTrackedDeviceProperty_data.pchValue = "yes\0";
        vrSystem.getStringTrackedDeviceProperty_data.unBufferSize = 4;
        vrSystem.getStringTrackedDeviceProperty_data.returns = 4;
        QVERIFY(Internal::Property::query<QString>(5, vr::Prop_ControllerType_String, nullptr) == QString{"yes\0"});
    }

    void queryQString_PreparedError_ReturnsExpected() {
        vrSystem.getStringTrackedDeviceProperty_data.unDeviceIndex = 7;
        vrSystem.getStringTrackedDeviceProperty_data.prop = vr::Prop_DriverVersion_String;
        vrSystem.getStringTrackedDeviceProperty_data.pchValue = "no\0";
        vrSystem.getStringTrackedDeviceProperty_data.unBufferSize = 3;
        vrSystem.getStringTrackedDeviceProperty_data.pError = vr::TrackedProp_BufferTooSmall;
        vrSystem.getStringTrackedDeviceProperty_data.returns = 3;
        vr::ETrackedPropertyError error;
        QVERIFY(Internal::Property::query<QString>(7, vr::Prop_DriverVersion_String, &error) == QString{"no\0"});
        QVERIFY(error == vr::TrackedProp_BufferTooSmall);
    }
};

QTEST_APPLESS_MAIN(PropertyTest)

#include "Internal/PropertyTest.moc"

#endif // CUTE_VR_OPEN_VR
