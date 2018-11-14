/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_DRIVER_SERVER
#define CUTE_VR_DRIVER_SERVER

#include <functional>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include <CuteVR/Extension/CuteException.hpp>
#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Interface/CyclicHandler.hpp>
#include <CuteVR/Interface/Destroyable.hpp>
#include <CuteVR/Interface/EventHandler.hpp>
#include <CuteVR/Interface/Initializable.hpp>
#include <CuteVR/Interface/TrackingHandler.hpp>
#include <CuteVR/Identifier.hpp>

namespace CuteVR {
    /// @brief This class implements a singleton pattern that initializes and destroys the underlying driver and helps
    /// to synchronize driver calls.
    class DriverServer :
            public QObject,
            public Interface::Destroyable,
            public Interface::Initializable {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")

    public: // types
        /// @brief Used to find out which library version has been compiled with.
        struct Version {
            quint32 const current{0x01000000}; ///< 2 byte "major", 2 byte "minor", 4 byte "patch"
        };

        /// @brief The driver instance cannot call the underlying driver due to the fact that access is locked.
        class CallLocked final :
                public Extension::CuriousCuteException<CallLocked> {};

        /// @brief The underlying driver is not installed or cannot be found.
        class UnderlyingDriverNotInstalled final :
                public Extension::CuriousCuteException<UnderlyingDriverNotInstalled> {};

        /// @brief There is no VR hardware setup present.
        class HardwareSetupNotPresent final :
                public Extension::CuriousCuteException<UnderlyingDriverNotInstalled> {};

        /// @brief The driver cannot be initialized twice.
        class AlreadyInitialized final :
                public Extension::CuriousCuteException<AlreadyInitialized> {};

        /// @brief The driver cannot be initialized properly.
        class InitializationFailed final :
                public Extension::CuriousCuteException<InitializationFailed> {};

        /// @brief To perform certain actions it is required that the driver is initialized.
        class NotInitialized final :
                public Extension::CuriousCuteException<NotInitialized> {};

        /// @brief The driver cannot be terminated twice.
        class AlreadyTerminated final :
                public Extension::CuriousCuteException<AlreadyTerminated> {};

        /// @brief The driver cannot be terminated properly.
        class TerminationFailed final :
                public Extension::CuriousCuteException<TerminationFailed> {};

        /// @brief To perform certain actions it is required that the driver is terminated.
        class NotTerminated final :
                public Extension::CuriousCuteException<NotTerminated> {};

        /// @brief The version of the shared library and the linked version in the executable have diverged.
        class VersionDiverged final :
                public Extension::CuriousCuteException<VersionDiverged> {};

        /// @brief The interface initialization of the underlying driver interface failed.
        class InterfaceInitializationFailed final :
                public Extension::CuriousCuteException<InterfaceInitializationFailed> {};

        /// @brief The interface termination of the underlying driver interface failed.
        class InterfaceTerminationFailed final :
                public Extension::CuriousCuteException<InterfaceTerminationFailed> {};

        /// @brief The interface version used to compile the library is invalid to the underlying driver.
        class InterfaceVersionInvalid final :
                public Extension::CuriousCuteException<InterfaceVersionInvalid> {};

        /// @brief The underlying driver cannot be initialized properly.
        class RuntimeInitializationFailed final :
                public Extension::CuriousCuteException<RuntimeInitializationFailed> {};

        /// @brief The underlying driver cannot be terminated properly.
        class RuntimeTerminationFailed final :
                public Extension::CuriousCuteException<RuntimeTerminationFailed> {};

        /// @brief The version of the underlying driver is invalid.
        class RuntimeVersionInvalid final :
                public Extension::CuriousCuteException<RuntimeVersionInvalid> {};

    public: // constructor/destructor
        ~DriverServer() override;

        Q_DISABLE_COPY(DriverServer)

    public: // methods
        /// @return Singleton instance of this class.
        static DriverServer &instance() noexcept;

        /// @brief Functors that are executed with this method have a guarantee that the library will neither start up
        /// nor shut down the underlying driver.
        /// @param functor The functor being executed.
        /// @param initialized `yes` and the driver must be initialized, `no` and it must be terminated, or `maybe` and
        /// it doesn't matter.
        /// @throw NotInitialized
        /// @throw NotTerminated
        static void synchronized(std::function<void(void)> const &functor,
                                 Extension::Trilean initialized = Extension::maybe);

        /// @brief Adds a callback to the event handling loop of the #pollEvents method.
        /// @param eventHandler The event handler that will be called if a new event occurs.
        /// @param devices A list of device identifiers for which the handler wants to receive events. An empty list
        /// subscribes for all devices.
        /// @param events A list of events which the handler wants to receive. An empty list subscribes for all events.
        static void announce(QWeakPointer<Interface::EventHandler> eventHandler,
                             QSet<Identifier> const &devices,
                             QSet<qint64> const &events) noexcept;

        /// @brief Polls new events from the virtual reality system and delegates them.
        /// @details Calls the event handlers that were #announce%d. The concrete behavior depends manly on the state
        /// of Configurations::Core::Feature::eventsEnabled and Configurations::Core::Feature::eventTrackingEnabled.
        /// @return Nothing, or an exception.
        /// @throw NotInitialized
        static Extension::Optional<QSharedPointer<Extension::CuteException>> pollEvents();

        /// @brief Adds a callback to the tracking handling loop of the #pollTracking method.
        /// @param trackingHandler The tracking handler that will be called if new tracking information is available.
        /// @param devices A list of device identifiers for which the handler wants to receive tracking information. An
        /// empty list subscribes to all devices.
        static void announce(QWeakPointer<Interface::TrackingHandler> trackingHandler,
                             QSet<Identifier> const &devices) noexcept;

        /// @brief Polls new tracking information from the virtual reality system and delegates them.
        /// @details Calls the tracking handlers that were #announce%d. The concrete behavior depends manly on the
        /// state of Configuration::Core::Feature::trackingEnabled.
        /// @return Nothing, or an exception.
        /// @throw NotInitialized
        static Extension::Optional<QSharedPointer<Extension::CuteException>> pollTracking();

        /// @brief Adds a callback to the cyclic loop of the #runCycle method.
        /// @param cyclicHandler The cyclic handler that will be called on every cycle.
        /// @param dataProvider The data generated by this function is sent to the cyclic handler.
        static void announce(QWeakPointer<Interface::CyclicHandler> cyclicHandler,
                             std::function<void *(void)> dataProvider = {}) noexcept;

        /// @brief Runs through a new cycle in which all cyclic handlers are called.
        /// @details Calls the cyclic handler that were #announce%d and sends the possibly generated data to them.
        /// @return Nothing, or an exception.
        static Extension::Optional<QSharedPointer<Extension::CuteException>> runCycle();

        /// @brief Checks whether all preconditions for a successful initialization have been met.
        /// @throw VersionDiverged
        /// @throw UnderlyingDriverNotInstalled
        /// @throw HardwareSetupNotPresent
        Extension::Optional<QSharedPointer<Extension::CuteException>> preInitialize(Version);

        /// @throw CallLocked
        /// @throw AlreadyTerminated
        /// @throw TerminationFailed
        /// @throw InterfaceTerminationFailed
        /// @throw RuntimeTerminationFailed
        /// @deprecated This will just call destroy2 and omit the exceptions.
        void destroy() override;

        /// @see destroy
        /// @return Nothing, or an exception.
        Extension::Optional<QSharedPointer<Extension::CuteException>> destroy2();

        bool isDestroyed() const noexcept override;

        /// @throw CallLocked
        /// @throw AlreadyInitialized
        /// @throw InitializationFailed
        /// @throw InterfaceInitializationFailed
        /// @throw InterfaceVersionInvalid
        /// @throw RuntimeInitializationFailed
        /// @throw RuntimeVersionInvalid
        /// @deprecated This will just call initialize2 and omit the exceptions.
        void initialize() override;

        /// @see initialize
        /// @return Nothing, or an exception.
        Extension::Optional<QSharedPointer<Extension::CuteException>> initialize2();

        bool isInitialized() const noexcept override;

    private: // types
        class Private;

    private: // constructor
        DriverServer();

    private: // variables
        QScopedPointer<Private> _private;
    };
}

#endif // CUTE_VR_DRIVER_SERVER
