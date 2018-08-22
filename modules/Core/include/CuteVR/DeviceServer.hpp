/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DEVICE_SERVER
#define CUTE_VR_DEVICE_SERVER

#include <functional>
#include <QtCore/QList>

#include <CuteVR/Extension/CuteException.hpp>
#include <CuteVR/Extension/Either.hpp>
#include <CuteVR/Device.hpp>

namespace CuteVR {
    /// @brief This class implements a singleton pattern that helps to register and create different devices.
    class DeviceServer :
            public QObject {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")

    public: // types
        /// @brief The device has not been registered yet.
        struct DeviceNotRegistered final :
                public Extension::CuriousCuteException<DeviceNotRegistered> {};

        /// @brief Tried to create a device that is not supported.
        struct DeviceNotSupported final :
                public Extension::CuriousCuteException<DeviceNotSupported> {};

        /// @brief The validator failed while configuring the device.
        struct DeviceValidationFailed final :
                public Extension::CuriousCuteException<DeviceValidationFailed> {};

        /// @brief The registration of the device failed.
        struct DeviceRegistrationFailed final :
                public Extension::CuriousCuteException<DeviceRegistrationFailed> {};

    public: // methods
        /// @return Singleton instance of this class.
        static DeviceServer &instance() noexcept;

        /// @brief Create a new device with the given identifier.
        /// @param identifier The identifier for the device.
        /// @return The newly created devices, or DeviceNotRegistered, DeviceNotSupported, or DeviceValidationFailed.
        static Extension::Either<QSharedPointer<Extension::CuteException>, QSharedPointer<Device>>
        create(Identifier identifier);

        /// @brief Part of an automatic registration process which is mainly relevant to device class developers.
        /// @note [tl;dr] Some magic happens here, one does not really have to understand it. Only relevant to device
        /// class developers.
        /// @param factory Method that is called to create an object that matches the connected device by name.
        /// @param signatures Names of devices that the class supports.
        /// @return Nothing, or DeviceRegistrationFailed.
        /// @attention [DEV] This method is part of an automatic registration process, which requires that tracked
        /// device classes register themselves. This procedure relieves the user by excluding a potential source of
        /// error, namely an unregistered or incorrectly registered device class. However, the chosen approach is not
        /// particularly transparent and is therefore explained by way of an example.
        /// @n The following example code is an explicit assignment of a (static) variable in an anonymous namespace,
        /// which forces the call of the method and completes the registration. Each device class should only register
        /// itself in its own source file. The names are composed of vendor, device name and revision.
        /// @code
        /// // File: ./source/Devices/[Category]/[Vendor_HardwareName].cpp
        ///
        /// namespace {
        ///     struct RegisterDevice {
        ///         RegisterDevice() {
        ///             CuteVR::DeviceServer::registerDevice(
        ///                     [](CuteVR::Identifier const identifier) {
        ///                         // Maybe add some tests here, e.g. check if creation is allowed by configuration.
        ///                         return QSharedPointer<CuteVR::Device>{
        ///                                 new CuteVR::Devices::Tracker::Magic_Marker{identifier}};
        ///                     }, QStringList{"Magic_Marker_1_1", "Magic_MarkerPro_1_2"});
        ///         }
        ///     } registerDevice; // NOLINT
        /// }
        /// @endcode
        /// With this procedure the registration should also take place if the respective device library is just
        /// linked, even if no header of has not been included.
        /// @n The easiest way to find out the concatenated name is to connect the device and wait for a message from
        /// the system, such as `Factory for tracked device class 'HolyNewVendor_TheNewDevice_3_14' not available.`.
        /// If a class supports multiple devices or device revisions, alternative signatures can be specified.
        static Extension::Optional<QSharedPointer<Extension::CuteException>>
        registerDevice(std::function<QSharedPointer<Device>(Identifier)> factory, QStringList const &signatures);

    public: // constructor/destructor
        ~DeviceServer() override;

        Q_DISABLE_COPY(DeviceServer)

    private: // types
        class Private;

    private: // constructor/destructor
        DeviceServer();

    private: // variables
        QScopedPointer<Private> _private;
    };
}

#endif // CUTE_VR_DEVICE_SERVER
