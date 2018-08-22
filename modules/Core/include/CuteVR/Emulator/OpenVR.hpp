/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen
/// @cond

#ifndef CUTE_VR_EMULATOR_OPEN_VR
#define CUTE_VR_EMULATOR_OPEN_VR

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

// The following define prevents that the symbols for instantiating the OpenVR interfaces are recreated. This allows us
// to define our own interface and thus allows reliable "offline" testing.
// Important Notes:
// 1. This define is undocumented and should be checked for existence and semantics with every update.
// 2. This file should never be included anywhere else than in tests that access functions of the OpenVR interface.
#ifdef _OPENVR_API
#error "This file must be included before openvr.h"
#endif // _OPENVR_API
#ifndef OPENVR_INTERFACE_INTERNAL
#define OPENVR_INTERFACE_INTERNAL
#endif // OPENVR_INTERFACE_INTERNAL

#include <cassert>
#include <cstring>
#include <openvr.h>

namespace CuteVR { namespace Emulator { namespace OpenVR {
    class VRSystem :
            public vr::IVRSystem {
    public:
        struct GetRecommendedRenderTargetSize_data {
            uint32_t pnWidth{};
            uint32_t pnHeight{};
        } getRecommendedRenderTargetSize_data{};

        void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
            *pnWidth = getRecommendedRenderTargetSize_data.pnWidth;
            *pnHeight = getRecommendedRenderTargetSize_data.pnHeight;
        }

        struct GetProjectionMatrix_data {
            vr::EVREye eEye{};
            float fNearZ{};
            float fFarZ{};
            vr::HmdMatrix44_t returns{};
        } getProjectionMatrix_data{};

        vr::HmdMatrix44_t GetProjectionMatrix(vr::EVREye eEye, float fNearZ, float fFarZ) override {
            assert(getProjectionMatrix_data.eEye == eEye);
            (void) eEye;
            assert(getProjectionMatrix_data.fNearZ == fNearZ);
            (void) fNearZ;
            assert(getProjectionMatrix_data.fFarZ == fFarZ);
            (void) fFarZ;
            return getProjectionMatrix_data.returns;
        }

        struct GetProjectionRaw_data {
            vr::EVREye eEye{};
            float pfLeft{};
            float pfRight{};
            float pfTop{};
            float pfBottom{};
        } getProjectionRaw_data{};

        void GetProjectionRaw(vr::EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) override {
            assert(getProjectionRaw_data.eEye == eEye);
            (void) eEye;
            *pfLeft = getProjectionRaw_data.pfLeft;
            *pfRight = getProjectionRaw_data.pfRight;
            *pfTop = getProjectionRaw_data.pfTop;
            *pfBottom = getProjectionRaw_data.pfBottom;
        }

        struct ComputeDistortion_data {
            vr::EVREye eEye{};
            float fU{};
            float fV{};
            vr::DistortionCoordinates_t pDistortionCoordinates{};
            bool returns{};
        } computeDistortion_data{};

        bool ComputeDistortion(vr::EVREye eEye, float fU, float fV,
                               vr::DistortionCoordinates_t *pDistortionCoordinates) override {
            assert(computeDistortion_data.eEye == eEye);
            (void) eEye;
            assert(computeDistortion_data.fU == fU);
            (void) fU;
            assert(computeDistortion_data.fV == fV);
            (void) fV;
            *pDistortionCoordinates = computeDistortion_data.pDistortionCoordinates;
            return computeDistortion_data.returns;
        }

        struct GetEyeToHeadTransform_data {
            vr::EVREye eEye{};
            vr::HmdMatrix34_t returns{};
        } getEyeToHeadTransform_data{};

        vr::HmdMatrix34_t GetEyeToHeadTransform(vr::EVREye eEye) override {
            assert(getEyeToHeadTransform_data.eEye == eEye);
            (void) eEye;
            return getEyeToHeadTransform_data.returns;
        }

        struct GetTimeSinceLastVsync_data {
            float pfSecondsSinceLastVsync{};
            uint64_t pulFrameCounter{};
            bool returns{};
        } getTimeSinceLastVsync_data{};

        bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
            *pfSecondsSinceLastVsync = getTimeSinceLastVsync_data.pfSecondsSinceLastVsync;
            *pulFrameCounter = getTimeSinceLastVsync_data.pulFrameCounter;
            return getTimeSinceLastVsync_data.returns;
        }

        struct GetD3D9AdapterIndex_data {
            int32_t returns{};
        } getD3D9AdapterIndex_data{};

        int32_t GetD3D9AdapterIndex() override {
            return getD3D9AdapterIndex_data.returns;
        }

        struct GetDXGIOutputInfo_data {
            int32_t pnAdapterIndex{};
        } getDXGIOutputInfo_data{};

        void GetDXGIOutputInfo(int32_t *pnAdapterIndex) override {
            *pnAdapterIndex = getDXGIOutputInfo_data.pnAdapterIndex;
        }

        struct GetOutputDevice_data {
            uint64_t pnDevice{};
            vr::ETextureType textureType{};
            VkInstance_T *pInstance{nullptr};
        } getOutputDevice_data{};

        void GetOutputDevice(uint64_t *pnDevice, vr::ETextureType textureType, VkInstance_T *pInstance) override {
            assert(getOutputDevice_data.textureType == textureType);
            (void) textureType;
            assert(getOutputDevice_data.pInstance == pInstance);
            (void) pInstance;
            *pnDevice = getOutputDevice_data.pnDevice;
        }

        struct IsDisplayOnDesktop_data {
            bool returns{};
        } isDisplayOnDesktop_data{};

        bool IsDisplayOnDesktop() override {
            return isDisplayOnDesktop_data.returns;
        }

        struct SetDisplayVisibility_data {
            bool bIsVisibleOnDesktop{};
            bool returns{};
        } setDisplayVisibility_data{};

        bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override {
            assert(setDisplayVisibility_data.bIsVisibleOnDesktop == bIsVisibleOnDesktop);
            (void) bIsVisibleOnDesktop;
            return setDisplayVisibility_data.returns;
        }

        struct GetDeviceToAbsoluteTrackingPose_data {
            vr::ETrackingUniverseOrigin eOrigin{};
            float fPredictedSecondsToPhotonsFromNow{};
            vr::TrackedDevicePose_t *pTrackedDevicePoseArray{};
            uint32_t unTrackedDevicePoseArrayCount{};
        } getDeviceToAbsoluteTrackingPose_data{};

        void GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin eOrigin,
                                             float fPredictedSecondsToPhotonsFromNow,
                                             vr::TrackedDevicePose_t *pTrackedDevicePoseArray,
                                             uint32_t unTrackedDevicePoseArrayCount) override {
            assert(getDeviceToAbsoluteTrackingPose_data.eOrigin == eOrigin);
            (void) eOrigin;
            assert(getDeviceToAbsoluteTrackingPose_data.fPredictedSecondsToPhotonsFromNow ==
                   fPredictedSecondsToPhotonsFromNow);
            (void) fPredictedSecondsToPhotonsFromNow;
            assert(getDeviceToAbsoluteTrackingPose_data.unTrackedDevicePoseArrayCount ==
                   unTrackedDevicePoseArrayCount);
            (void) unTrackedDevicePoseArrayCount;
            for (uint32_t i = 0; i < unTrackedDevicePoseArrayCount; i++) {
                pTrackedDevicePoseArray[i] = getDeviceToAbsoluteTrackingPose_data.pTrackedDevicePoseArray[i];
            }
        }

        void ResetSeatedZeroPose() override {}

        struct GetSeatedZeroPoseToStandingAbsoluteTrackingPose_data {
            vr::HmdMatrix34_t returns{};
        } getSeatedZeroPoseToStandingAbsoluteTrackingPose_data{};

        vr::HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() override {
            return getSeatedZeroPoseToStandingAbsoluteTrackingPose_data.returns;
        }

        struct GetRawZeroPoseToStandingAbsoluteTrackingPose_data {
            vr::HmdMatrix34_t returns{};
        } getRawZeroPoseToStandingAbsoluteTrackingPose_data{};

        vr::HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() override {
            return getRawZeroPoseToStandingAbsoluteTrackingPose_data.returns;
        }

        struct GetSortedTrackedDeviceIndicesOfClass_data {
            vr::ETrackedDeviceClass eTrackedDeviceClass{};
            vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray{};
            uint32_t unTrackedDeviceIndexArrayCount{};
            vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex{vr::k_unTrackedDeviceIndex_Hmd};
            uint32_t returns{};
        } getSortedTrackedDeviceIndicesOfClass_data{};

        uint32_t GetSortedTrackedDeviceIndicesOfClass(vr::ETrackedDeviceClass eTrackedDeviceClass,
                                                      vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray,
                                                      uint32_t unTrackedDeviceIndexArrayCount,
                                                      vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex) override {
            assert(getSortedTrackedDeviceIndicesOfClass_data.eTrackedDeviceClass == eTrackedDeviceClass);
            (void) eTrackedDeviceClass;
            assert(getSortedTrackedDeviceIndicesOfClass_data.unTrackedDeviceIndexArrayCount ==
                   unTrackedDeviceIndexArrayCount);
            (void) unTrackedDeviceIndexArrayCount;
            assert(getSortedTrackedDeviceIndicesOfClass_data.unRelativeToTrackedDeviceIndex ==
                   unRelativeToTrackedDeviceIndex);
            (void) unRelativeToTrackedDeviceIndex;
            for (uint32_t i = 0; i < unTrackedDeviceIndexArrayCount; i++) {
                punTrackedDeviceIndexArray[i] = getSortedTrackedDeviceIndicesOfClass_data.punTrackedDeviceIndexArray[i];
            }
            return getSortedTrackedDeviceIndicesOfClass_data.returns;
        }

        struct GetTrackedDeviceActivityLevel_data {
            vr::TrackedDeviceIndex_t unDeviceId{};
            vr::EDeviceActivityLevel returns{};
        } getTrackedDeviceActivityLevel_data{};

        vr::EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override {
            assert(getTrackedDeviceActivityLevel_data.unDeviceId == unDeviceId);
            (void) unDeviceId;
            return getTrackedDeviceActivityLevel_data.returns;
        }

        struct ApplyTransform_data {
            vr::TrackedDevicePose_t pOutputPose{};
            vr::TrackedDevicePose_t pTrackedDevicePose{};
            vr::HmdMatrix34_t pTransform{};
        } applyTransform_data;

        void ApplyTransform(vr::TrackedDevicePose_t *pOutputPose, const vr::TrackedDevicePose_t *pTrackedDevicePose,
                            const vr::HmdMatrix34_t *pTransform) override {
            //assert(applyTransform_data.pTrackedDevicePose == *pTrackedDevicePose); // TODO: == not implemented
            (void) pTrackedDevicePose;
            //assert(applyTransform_data.pTransform == *pTransform); // TODO: == not implemented
            (void) pTransform;
            *pOutputPose = applyTransform_data.pOutputPose;
        }

        struct GetTrackedDeviceIndexForControllerRole_data {
            vr::ETrackedControllerRole unDeviceType{};
            vr::TrackedDeviceIndex_t returns{};
        } getTrackedDeviceIndexForControllerRole_data{};

        vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(
                vr::ETrackedControllerRole unDeviceType) override {
            assert(getTrackedDeviceIndexForControllerRole_data.unDeviceType == unDeviceType);
            (void) unDeviceType;
            return getTrackedDeviceIndexForControllerRole_data.returns;
        }

        struct GetControllerRoleForTrackedDeviceIndex_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedControllerRole returns{};
        } getControllerRoleForTrackedDeviceIndex_data{};

        vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(
                vr::TrackedDeviceIndex_t unDeviceIndex) override {
            assert(getControllerRoleForTrackedDeviceIndex_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            return getControllerRoleForTrackedDeviceIndex_data.returns;
        }

        struct GetTrackedDeviceClass_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceClass returns{};
        } getTrackedDeviceClass_data{};

        vr::ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
            assert(getTrackedDeviceClass_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            return getTrackedDeviceClass_data.returns;
        }

        struct IsTrackedDeviceConnected_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            bool returns{};
        } isTrackedDeviceConnected_data{};

        bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
            assert(isTrackedDeviceConnected_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            return isTrackedDeviceConnected_data.returns;
        }

        struct GetBoolTrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::ETrackedPropertyError pError{};
            bool returns{};
        } getBoolTrackedDeviceProperty_data;

        bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                          vr::ETrackedPropertyError *pError) override {
            assert(getBoolTrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getBoolTrackedDeviceProperty_data.prop == prop);
            (void) prop;
            if (pError) {
                *pError = getBoolTrackedDeviceProperty_data.pError;
            }
            return getBoolTrackedDeviceProperty_data.returns;
        }

        struct GetFloatTrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::ETrackedPropertyError pError{};
            float returns{};
        } getFloatTrackedDeviceProperty_data;

        float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                            vr::ETrackedPropertyError *pError) override {
            assert(getFloatTrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getFloatTrackedDeviceProperty_data.prop == prop);
            (void) prop;
            if (pError) {
                *pError = getFloatTrackedDeviceProperty_data.pError;
            }
            return getFloatTrackedDeviceProperty_data.returns;
        }

        struct GetInt32TrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::ETrackedPropertyError pError{};
            int32_t returns{};
        } getInt32TrackedDeviceProperty_data;

        int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                              vr::ETrackedPropertyError *pError) override {
            assert(getInt32TrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getInt32TrackedDeviceProperty_data.prop == prop);
            (void) prop;
            if (pError) {
                *pError = getInt32TrackedDeviceProperty_data.pError;
            }
            return getInt32TrackedDeviceProperty_data.returns;
        }

        struct GetUint64TrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::ETrackedPropertyError pError{};
            uint64_t returns{};
        } getUint64TrackedDeviceProperty_data;

        uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                                vr::ETrackedPropertyError *pError) override {
            assert(getUint64TrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getUint64TrackedDeviceProperty_data.prop == prop);
            (void) prop;
            if (pError) {
                *pError = getUint64TrackedDeviceProperty_data.pError;
            }
            return getUint64TrackedDeviceProperty_data.returns;
        }

        struct GetMatrix34TrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::ETrackedPropertyError pError{};
            vr::HmdMatrix34_t returns{};
        } getMatrix34TrackedDeviceProperty_data;

        vr::HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex,
                                                           vr::ETrackedDeviceProperty prop,
                                                           vr::ETrackedPropertyError *pError) override {
            assert(getMatrix34TrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getMatrix34TrackedDeviceProperty_data.prop == prop);
            (void) prop;
            if (pError) {
                *pError = getMatrix34TrackedDeviceProperty_data.pError;
            }
            return getMatrix34TrackedDeviceProperty_data.returns;
        }

        struct GetArrayTrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            vr::PropertyTypeTag_t propType{};
            void *pBuffer{};
            uint32_t unBufferSize{};
            vr::ETrackedPropertyError pError{};
            uint32_t returns{};
        } getArrayTrackedDeviceProperty_data;

        uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                               vr::PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize,
                                               vr::ETrackedPropertyError *pError) override {
            assert(getArrayTrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getArrayTrackedDeviceProperty_data.prop == prop);
            (void) prop;
            assert(getArrayTrackedDeviceProperty_data.propType == propType);
            (void) propType;
            assert(getArrayTrackedDeviceProperty_data.unBufferSize == unBufferSize || unBufferSize == 0);
            (void) unBufferSize;
            if (pError) {
                *pError = getArrayTrackedDeviceProperty_data.pError;
            }
            if (pBuffer) {
                std::memcpy(pBuffer, getArrayTrackedDeviceProperty_data.pBuffer, unBufferSize);
            }
            return getArrayTrackedDeviceProperty_data.returns;
        }

        struct GetStringTrackedDeviceProperty_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::ETrackedDeviceProperty prop{};
            const char *pchValue{};
            uint32_t unBufferSize{};
            vr::ETrackedPropertyError pError{};
            uint32_t returns{};
        } getStringTrackedDeviceProperty_data;

        uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop,
                                                char *pchValue, uint32_t unBufferSize,
                                                vr::ETrackedPropertyError *pError) override {
            assert(getStringTrackedDeviceProperty_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(getStringTrackedDeviceProperty_data.prop == prop);
            (void) prop;
            assert(getStringTrackedDeviceProperty_data.unBufferSize == unBufferSize || unBufferSize == 0);
            (void) unBufferSize;
            if (pError) {
                *pError = getStringTrackedDeviceProperty_data.pError;
            }
            if (pchValue) {
                std::memcpy(pchValue, getStringTrackedDeviceProperty_data.pchValue, unBufferSize);
            }
            return getStringTrackedDeviceProperty_data.returns;
        }

        struct GetPropErrorNameFromEnum_data {
            vr::ETrackedPropertyError error{};
            const char *returns{};
        } getPropErrorNameFromEnum_data;

        const char *GetPropErrorNameFromEnum(vr::ETrackedPropertyError error) override {
            assert(getPropErrorNameFromEnum_data.error == error);
            (void) error;
            return getPropErrorNameFromEnum_data.returns;
        }

        struct PollNextEvent_data {
            vr::VREvent_t pEvent{};
            bool returns{};
        } pollNextEvent_data{};

        bool PollNextEvent(vr::VREvent_t *pEvent, uint32_t uncbVREvent) override {
            assert(uncbVREvent == sizeof(vr::VREvent_t)); // sic!
            (void) uncbVREvent;
            *pEvent = pollNextEvent_data.pEvent;
            return pollNextEvent_data.returns;
        }

        struct PollNextEventWithPose_data {
            vr::ETrackingUniverseOrigin eOrigin{};
            vr::VREvent_t pEvent{};
            vr::TrackedDevicePose_t pTrackedDevicePose{};
            bool returns{};
        } pollNextEventWithPose_data{};

        bool PollNextEventWithPose(vr::ETrackingUniverseOrigin eOrigin, vr::VREvent_t *pEvent, uint32_t uncbVREvent,
                                   vr::TrackedDevicePose_t *pTrackedDevicePose) override {
            assert(pollNextEventWithPose_data.eOrigin == eOrigin);
            (void) eOrigin;
            assert(uncbVREvent == sizeof(vr::VREvent_t)); // sic!
            (void) uncbVREvent;
            *pEvent = pollNextEventWithPose_data.pEvent;
            *pTrackedDevicePose = pollNextEventWithPose_data.pTrackedDevicePose;
            return pollNextEventWithPose_data.returns;
        }

        struct GetEventTypeNameFromEnum_data {
            vr::EVREventType eType{};
            const char *returns{};
        } getEventTypeNameFromEnum_data{};

        const char *GetEventTypeNameFromEnum(vr::EVREventType eType) override {
            assert(getEventTypeNameFromEnum_data.eType == eType);
            (void) eType;
            return getEventTypeNameFromEnum_data.returns;
        }

        struct GetHiddenAreaMesh_data {
            vr::EVREye eEye{};
            vr::EHiddenAreaMeshType type{};
            vr::HiddenAreaMesh_t returns{};
        } getHiddenAreaMesh_data{};

        vr::HiddenAreaMesh_t GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type) override {
            assert(getHiddenAreaMesh_data.eEye == eEye);
            (void) eEye;
            assert(getHiddenAreaMesh_data.type == type);
            (void) type;
            return getHiddenAreaMesh_data.returns;
        }

        struct GetControllerState_data {
            vr::TrackedDeviceIndex_t unControllerDeviceIndex{};
            vr::VRControllerState_t pControllerState{};
            uint32_t unControllerStateSize{};
            bool returns{};
        } getControllerState_data{};

        bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex,
                                vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) override {
            assert(getControllerState_data.unControllerDeviceIndex == unControllerDeviceIndex);
            (void) unControllerDeviceIndex;
            assert(getControllerState_data.unControllerStateSize == unControllerStateSize);
            (void) unControllerStateSize;
            *pControllerState = getControllerState_data.pControllerState;
            return getControllerState_data.returns;
        }

        struct GetControllerStateWithPose_data {
            vr::ETrackingUniverseOrigin eOrigin{};
            vr::TrackedDeviceIndex_t unControllerDeviceIndex{};
            vr::VRControllerState_t pControllerState{};
            vr::TrackedDevicePose_t pTrackedDevicePose{};
            uint32_t unControllerStateSize{};
            bool returns{};
        } getControllerStateWithPose_data{};

        bool GetControllerStateWithPose(vr::ETrackingUniverseOrigin eOrigin,
                                        vr::TrackedDeviceIndex_t unControllerDeviceIndex,
                                        vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize,
                                        vr::TrackedDevicePose_t *pTrackedDevicePose) override {
            assert(getControllerStateWithPose_data.eOrigin == eOrigin);
            (void) eOrigin;
            assert(getControllerStateWithPose_data.unControllerDeviceIndex == unControllerDeviceIndex);
            (void) unControllerDeviceIndex;
            assert(getControllerStateWithPose_data.unControllerStateSize == unControllerStateSize);
            (void) unControllerStateSize;
            *pControllerState = getControllerStateWithPose_data.pControllerState;
            *pTrackedDevicePose = getControllerStateWithPose_data.pTrackedDevicePose;
            return getControllerStateWithPose_data.returns;
        }

        struct TriggerHapticPulse_data {
            vr::TrackedDeviceIndex_t unControllerDeviceIndex{};
            uint32_t unAxisId{};
            unsigned short usDurationMicroSec{};
        } triggerHapticPulse_data{};

        void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId,
                                unsigned short usDurationMicroSec) override {
            assert(triggerHapticPulse_data.unControllerDeviceIndex == unControllerDeviceIndex);
            (void) unControllerDeviceIndex;
            assert(triggerHapticPulse_data.unAxisId == unAxisId);
            (void) unAxisId;
            assert(triggerHapticPulse_data.usDurationMicroSec == usDurationMicroSec);
            (void) usDurationMicroSec;
        }

        struct GetButtonIdNameFromEnum_data {
            vr::EVRButtonId eButtonId{};
            const char *returns{};
        } getButtonIdNameFromEnum_data{};

        const char *GetButtonIdNameFromEnum(vr::EVRButtonId eButtonId) override {
            assert(getButtonIdNameFromEnum_data.eButtonId == eButtonId);
            (void) eButtonId;
            return getButtonIdNameFromEnum_data.returns;
        }

        struct GetControllerAxisTypeNameFromEnum_data {
            vr::EVRControllerAxisType eAxisType{};
            const char *returns{};
        } getControllerAxisTypeNameFromEnum_data{};

        const char *GetControllerAxisTypeNameFromEnum(vr::EVRControllerAxisType eAxisType) override {
            assert(getControllerAxisTypeNameFromEnum_data.eAxisType == eAxisType);
            (void) eAxisType;
            return getControllerAxisTypeNameFromEnum_data.returns;
        }

        struct IsInputAvailable_data {
            bool returns{};
        } isInputAvailable_data{};

        bool IsInputAvailable() override {
            return isInputAvailable_data.returns;
        }

        struct IsSteamVRDrawingControllers_data {
            bool returns{};
        } isSteamVRDrawingControllers_data{};

        bool IsSteamVRDrawingControllers() override {
            return isSteamVRDrawingControllers_data.returns;
        }

        struct ShouldApplicationPause_data {
            bool returns{};
        } shouldApplicationPause_data{};

        bool ShouldApplicationPause() override {
            return shouldApplicationPause_data.returns;
        }

        struct ShouldApplicationReduceRenderingWork_data {
            bool returns{};
        } shouldApplicationReduceRenderingWork_data{};

        bool ShouldApplicationReduceRenderingWork() override {
            return shouldApplicationReduceRenderingWork_data.returns;
        }

        struct DriverDebugRequest_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            const char *pchRequest{};
            char *pchResponseBuffer{};
            uint32_t unResponseBufferSize{};
            uint32_t returns{};
        } driverDebugRequest_data{};

        uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest,
                                    char *pchResponseBuffer, uint32_t unResponseBufferSize) override {
            assert(driverDebugRequest_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            assert(driverDebugRequest_data.unResponseBufferSize == unResponseBufferSize || unResponseBufferSize == 0);
            (void) unResponseBufferSize;
            assert(std::strcmp(pchRequest, driverDebugRequest_data.pchRequest) == 0);
            (void) pchRequest;
            if (pchResponseBuffer) { // TODO: verify this method works like GetStringTrackedDeviceProperty
                memcpy(pchResponseBuffer, driverDebugRequest_data.pchResponseBuffer, unResponseBufferSize);
            }
            return driverDebugRequest_data.returns;
        }

        struct PerformFirmwareUpdate_data {
            vr::TrackedDeviceIndex_t unDeviceIndex{};
            vr::EVRFirmwareError returns{};
        } performFirmwareUpdate_data{};

        vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override {
            assert(performFirmwareUpdate_data.unDeviceIndex == unDeviceIndex);
            (void) unDeviceIndex;
            return performFirmwareUpdate_data.returns;
        }

        void AcknowledgeQuit_Exiting() override {}

        void AcknowledgeQuit_UserPrompt() override {}
    } vrSystem{};
}}}

namespace vr {
    struct System_data {
        IVRSystem *returns{nullptr};
    } system_data{};

    inline IVRSystem *VRSystem() { return system_data.returns; }

    struct Init_data {
        EVRInitError peError{VRInitError_None};
        EVRApplicationType eApplicationType{};
        const char *pStartupInfo{nullptr};
    } init_data{};

    inline IVRSystem *VR_Init(EVRInitError *peError, EVRApplicationType eApplicationType,
                              const char *pStartupInfo = nullptr) {
        assert(init_data.eApplicationType == eApplicationType);
        assert(init_data.pStartupInfo == pStartupInfo);
        *peError = init_data.peError;
        if (init_data.peError == VRInitError_None) {
            system_data.returns = &CuteVR::Emulator::OpenVR::vrSystem;
        }
        return system_data.returns;
    }

    struct Shutdown_data {
    } shutdown_data{};

    inline void VR_Shutdown() {
        system_data.returns = nullptr;
    }

    struct IsInterfaceVersionValid_data {
        bool returns{true};
    } isInterfaceVersionValid_data{};

    VR_INTERFACE bool VR_CALLTYPE VR_IsInterfaceVersionValid(const char *) {
        return isInterfaceVersionValid_data.returns;
    }
}

namespace CuteVR { namespace Emulator { namespace OpenVR {
    void invoke() {
        vr::init_data.eApplicationType = vr::VRApplication_Bootstrapper;
        vr::EVRInitError error;
        vr::VR_Init(&error, vr::VRApplication_Bootstrapper);
        vr::VR_Shutdown();
    }
}}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_EMULATOR_OPEN_VR

/// @endcond
