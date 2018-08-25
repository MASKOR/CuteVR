/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_PROPERTY
#define CUTE_VR_INTERNAL_PROPERTY

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <QtCore/QString>
#include <openvr.h>

#include <CuteVR/Internal/Matrix3x4.hpp>
#include <CuteVR/Internal/Matrix4x4.hpp>
#include <CuteVR/DriverServer.hpp>
#include <CuteVR/Identifier.hpp>

namespace CuteVR { namespace Internal {
    /// @internal@brief Query properties provided by OpenVR and, if necessary, convert them to appropriate Qt formats.
    namespace Property {
        /// @internal@brief Query a property from the OpenVR system.
        /// @tparam PropertyTypeT The return type expected from this property query.
        /// @pre PropertyTypeT can be `bool`, `float`, `qint32`, `quint64`, `QMatrix4x3`, `QMatrix4x4`, or `QString`.
        /// @param identifier The device index number.
        /// @param property The property to be queried.
        /// @param error If an error occurs, it is to be returned in this parameter.
        /// @return The queried property or a default value in case of an error.
        template<class PropertyTypeT>
        inline PropertyTypeT query(Identifier identifier, vr::ETrackedDeviceProperty property,
                                   vr::ETrackedPropertyError *error = nullptr);

        /// @internal@overload
        template<>
        inline bool query<bool>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                vr::ETrackedPropertyError *error) {
            bool value{};
            DriverServer::synchronized([&] {
                value = vr::VRSystem()->GetBoolTrackedDeviceProperty(identifier, property, error);
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline float query<float>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                  vr::ETrackedPropertyError *error) {
            float value{};
            DriverServer::synchronized([&] {
                value = vr::VRSystem()->GetFloatTrackedDeviceProperty(identifier, property, error);
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline qint32 query<qint32>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                    vr::ETrackedPropertyError *error) {
            qint32 value{};
            DriverServer::synchronized([&] {
                value = vr::VRSystem()->GetInt32TrackedDeviceProperty(identifier, property, error);
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline quint64 query<quint64>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                      vr::ETrackedPropertyError *error) {
            quint64 value{};
            DriverServer::synchronized([&] {
                value = vr::VRSystem()->GetUint64TrackedDeviceProperty(identifier, property, error);
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline QMatrix4x3 query<QMatrix4x3>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                            vr::ETrackedPropertyError *error) {
            QMatrix4x3 value{};
            DriverServer::synchronized([&] {
                value = Matrix3x4::from(vr::VRSystem()->GetMatrix34TrackedDeviceProperty(identifier, property, error));
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline QMatrix4x4 query<QMatrix4x4>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                            vr::ETrackedPropertyError *error) {
            QMatrix4x4 value{};
            DriverServer::synchronized([&] {
                value = Matrix4x4::from(vr::VRSystem()->GetMatrix34TrackedDeviceProperty(identifier, property, error));
            }, Extension::Trilean::yes);
            return value;
        }

        /// @internal@overload
        template<>
        inline QString query<QString>(Identifier const identifier, vr::ETrackedDeviceProperty const property,
                                      vr::ETrackedPropertyError *error) {
            QByteArray bytes{};
            DriverServer::synchronized([&] {
                bytes = QByteArray{static_cast<qint32>(vr::VRSystem()
                        ->GetStringTrackedDeviceProperty(identifier, property, nullptr, 0)), '\0'};
                vr::VRSystem()->GetStringTrackedDeviceProperty(identifier, property, bytes.data(),
                                                               static_cast<quint32>(bytes.size()), error);
            });
            return {bytes};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_PROPERTY
