/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_CONFIGURATION
#define CUTE_VR_CONFIGURATION

#include <functional>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <CuteVR/Extension/CuteException.hpp>
#include <CuteVR/Extension/Either.hpp>
#include <CuteVR/Interface/Serializable.hpp>

namespace CuteVR {
    /// @brief This class implements a singleton that provides the configuration of the VR system and makes it
    /// available to the user.
    /// @details The class defines general conditions for registering "features", checking their current state and
    /// support, and enabling or disabling them. In addition, it is possible to set parameters that can be used beyond
    /// the boundaries of modules. These parameters allow various types, whereas features are limited to boolean.
    class ConfigurationServer :
            public QObject,
            public Interface::Serializable {
    Q_OBJECT
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")

    public: // types
        /// @brief This anonymous enumeration contains the start numbers of the value ranges which are used by the
        /// different modules.
        /// @details The first enumerator of a module group always starts **after** the specified value.
        enum :
                quint16 {
            generalCore = 0x0100, ///< General enumerators of the core module.
            generalDriver = 0x0400, ///< Driver-specific enumerators.
            generalZeta = 0x0F00, ///< General enumerators of zeta modules.
            deviceCore = 0x1000, ///< Device-related enumerators of the core module.
            deviceUser = 0x1100, ///< Device-related enumerators of the device module.
            deviceZeta = 0x7F00, ///< Device-related enumerators of the device zeta module.
            renderCore = 0x8000, ///< Render-related enumerators of the core module.
            renderUser = 0x8100, ///< Render-related enumerators of the render module.
            renderZeta = 0x8F00, ///< Render-related enumerators of the render zeta module.
            audioCore = 0x9000, ///< Audio-related enumerators of the core module.
            audioUser = 0x9100, ///< Audio-related enumerators of the audio module.
            audioZeta = 0x9F00, ///< Audio-related enumerators of the audio zeta module.
            videoCore = 0xA000, ///< Video-related enumerators of the core module.
            videoUser = 0xA100, ///< Video-related enumerators of the video module.
            videoZeta = 0xAF00, ///< Video-related enumerators of the video zeta module.
            inputCore = 0xB000, ///< Input-related enumerators of the core module.
            inputUser = 0xB100, ///< Input-related enumerators of the input module.
            inputZeta = 0xBF00, ///< Input-related enumerators of the input zeta module.
            reserved = 0xC000, ///< This range is reserved for future modules only.
        };

        /// @brief This is a basic enumeration for features, which only serves to specify the function parameter types
        /// for any feature-related method in this this class.
        /// @details The actually relevant enumerations can be found in the respective module configuration headers,
        /// e.g. CoreFeature. User-defined features should be greater than Feature::user.
        enum class Feature :
                quint16 {
            undefined, ///< Default if no value has been given.
            user = 0xF000, ///< User-defined features.
        };

        Q_ENUM(Feature)

        /// @brief This is a basic enumeration for parameters, which only serves to specify the function parameter
        /// types for any parameter-related method in this this class.
        /// @details The actually relevant enumerations can be found in the respective module configuration headers,
        /// e.g. CoreParameters. User-defined parameters should be greater than Parameter::user.
        enum class Parameter :
                quint16 {
            undefined, ///< Default if no value has been given.
            user = 0xF000, ///< User-defined parameters.
        };

        Q_ENUM(Parameter)

        /// @brief The given feature-enumerator has not been registered yet.
        struct FeatureNotRegistered final :
                public Extension::CuriousCuteException<FeatureNotRegistered> {};

        /// @brief Tried to enable a feature that is not supported.
        struct FeatureNotSupported final :
                public Extension::CuriousCuteException<FeatureNotSupported> {};

        /// @brief The validator failed while configuring the feature.
        struct FeatureValidationFailed final :
                public Extension::CuriousCuteException<FeatureValidationFailed> {};

        /// @brief The registration of the feature failed.
        /// @details
        /// | Hint | Problem                                                                                           |
        /// | ---- | ------------------------------------------------------------------------------------------------- |
        /// | Severity: **Warning**                                                                                   ||
        /// | 4000 | Inconsistent because native support but no support in general. Assuming the feature is supported. |
        /// | 4001 | Inconsistent because enabled by default but not supported. Assuming the feature is disabled.      |
        /// | 4002 | Validation failed for current state. Assume the default state withstands validation.              |
        /// | Severity: **Critical**                                                                                  ||
        /// | 8000 | Enumerator value is placed in a reserved range.                                                   |
        /// | 8001 | Enumerator value overrides a group range enumerator.                                              |
        /// | 8002 | Validation failed for default state.
        struct FeatureRegistrationFailed final :
                public Extension::CuriousCuteException<FeatureRegistrationFailed> {};

        /// @brief The given parameter-enumerator has not been registered yet.
        struct ParameterNotRegistered final :
                public Extension::CuriousCuteException<ParameterNotRegistered> {};

        /// @brief The validator failed while configuring the parameter.
        struct ParameterValidationFailed final :
                public Extension::CuriousCuteException<ParameterValidationFailed> {};

        /// @brief The registration of the parameter failed.
        /// @details
        /// | Hint | Problem                                                                                           |
        /// | ---- | ------------------------------------------------------------------------------------------------- |
        /// | Severity: **Warning**                                                                                   ||
        /// | 4002 | Validation failed for current value. Assume the default value withstands validation.              |
        /// | Severity: **Critical**                                                                                  ||
        /// | 8000 | Enumerator value is placed in a reserved range.                                                   |
        /// | 8001 | Enumerator value overrides a group range enumerator.                                              |
        /// | 8002 | Validation failed for default value.                                                              |
        struct ParameterRegistrationFailed final :
                public Extension::CuriousCuteException<ParameterRegistrationFailed> {};

    public: // constructor/destructor
        ~ConfigurationServer() override;

        Q_DISABLE_COPY(ConfigurationServer)

    public: // getter
        /// @brief Checks whether the given feature is enabled.
        /// @param feature The feature to test on.
        /// @return `true` (`false`) if the feature is (not) enabled, or FeatureNotRegistered (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, bool>
        isEnabled(Feature feature) noexcept;

        /// @brief Checks whether the given feature is enabled by default.
        /// @param feature The feature to test on.
        /// @return `true` (`false`) if the feature is (not) enabled by default, or FeatureNotRegistered (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, bool>
        isEnabledByDefault(Feature feature) noexcept;

        /// @brief Checks whether the given feature is supported, either native or emulated.
        /// @param feature The feature to test on.
        /// @return `true` (`false`) if the feature is (not) supported, or FeatureNotRegistered (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, bool>
        isSupported(Feature feature) noexcept;

        /// @brief Checks whether the given feature is supported native.
        /// @param feature The feature to test on.
        /// @return `true` (`false`) if the feature is (not) supported native, or FeatureNotRegistered (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, bool>
        isSupportedNative(Feature feature) noexcept;

        /// @brief Queries the value for the given parameter.
        /// @param parameter The parameter to fetch from.
        /// @return The value as a variant -- use #valueType to determine the type, or ParameterNotRegistered (no
        /// hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, QVariant>
        value(Parameter parameter) noexcept;

        /// @brief Queries the default value for the given parameter.
        /// @param parameter The parameter to fetch from.
        /// @return The default value as a variant -- use #valueType to determine the type, or ParameterNotRegistered
        /// (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, QVariant>
        valueByDefault(Parameter parameter) noexcept;

        /// @brief Queries the value type for the given parameter.
        /// @param parameter The parameter to fetch from.
        /// @return The type of the variant, or ParameterNotRegistered (no hints, 0).
        static Extension::Either<QSharedPointer<Extension::CuteException>, QVariant::Type>
        valueType(Parameter parameter) noexcept;

    public: // setter
        /// @brief Enables the given feature, if it is supported.
        /// @param feature The feature to change.
        /// @return Nothing, or FeatureNotRegistered (no hints, 0), FeatureNotSupported (no hints, 0), or
        /// FeatureValidationFailed (see corresponding module documentation for hints).
        static Extension::Optional<QSharedPointer<Extension::CuteException>> enable(Feature feature) noexcept;

        /// @brief Disables the given feature.
        /// @param feature The feature to change.
        /// @return Nothing, or FeatureNotRegistered (no hints, 0), or FeatureValidationFailed (see corresponding module
        /// documentation for hints)
        static Extension::Optional<QSharedPointer<Extension::CuteException>> disable(Feature feature) noexcept;

        /// @brief Sets a new value for the given parameter.
        /// @param parameter The parameter to change.
        /// @param value The new value for the parameter, use #valueType for the type.
        /// @return Nothing, or ParameterNotRegistered (no hints, 0), or ParameterValidationFailed (see corresponding
        /// module documentation for hints).
        static Extension::Optional<QSharedPointer<Extension::CuteException>>
        setValue(Parameter parameter, QVariant value) noexcept;

    public: // reset
        /// @brief Resets the given feature to its default value.
        /// @param feature The feature to be reset.
        /// @return Nothing, or FeatureNotRegistered (no hints, 0), or FeatureValidationFailed (see corresponding module
        /// documentation for hints).
        static Extension::Optional<QSharedPointer<Extension::CuteException>> reset(Feature feature) noexcept;

        /// @brief Resets the given parameter to its default value.
        /// @param parameter The parameter to be reset.
        /// @return Nothing, or ParameterNotRegistered (no hints, 0), or ParameterValidationFailed (see corresponding
        /// module documentation for hints).
        static Extension::Optional<QSharedPointer<Extension::CuteException>> resetValue(Parameter parameter) noexcept;

    public: // methods
        /// @return Singleton instance of this class.
        static ConfigurationServer &instance() noexcept;

        /// @brief Registers a new feature, which can then be accessed via the getters and setters. Already registered
        /// features will be updated accordingly.
        /// @param feature The new feature or the one to be updated.
        /// @param enabledByDefault The feature will be enabled by default, or on a reset.
        /// @param supported The feature is marked as supported, either native or emulated.
        /// @param supportedNative The feature is marked as supported native.
        /// @param validator A validator-functions, that will be called before enabling or disabling a feature.
        /// @return Nothing, or FeatureRegistrationFailed (hints: 4000, 4001, 4002)
        /// @throw FeatureRegistrationFailed (hints: 8000, 8001, 8002)
        static Extension::Optional<QSharedPointer<Extension::CuteException>>
        registerFeature(Feature feature, bool enabledByDefault, bool supported, bool supportedNative, std::function<
                Extension::Optional<QSharedPointer<Extension::CuteException>>(bool)> validator = {});

        /// @brief Registers a new parameter, which can then be accessed via the getters and setters. Already
        /// registered parameters will be updated accordingly.
        /// @param parameter The new parameter or the one to be updated.
        /// @param valueByDefault The parameter will have this value by default, or after a reset.
        /// @param valueType The expected type of the parameters value.
        /// @param validator A validator-functions, that will be called before setting a parameter.
        /// @return Nothing, or FeatureRegistrationFailed (hints: 4002)
        /// @throw FeatureRegistrationFailed (hints: 8000, 8001, 8002)
        static Extension::Optional<QSharedPointer<Extension::CuteException>>
        registerParameter(Parameter parameter, QVariant valueByDefault, QVariant::Type valueType, std::function<
                Extension::Optional<QSharedPointer<Extension::CuteException>>(QVariant)> validator = {});

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    private: // types
        class Private;

    private: // constructor
        ConfigurationServer();

    private: // variables
        QScopedPointer<Private> _private;

    signals:
        /// @signal{feature}
        void featureChanged(CuteVR::ConfigurationServer::Feature, bool);

        /// @signal{parameter}
        void parameterChanged(CuteVR::ConfigurationServer::Parameter, QVariant);
    };

    /// @ostream{configuration feature}
    QDataStream &operator<<(QDataStream &stream, ConfigurationServer::Feature feature);

    /// @istream{configuration feature}
    QDataStream &operator>>(QDataStream &stream, ConfigurationServer::Feature &feature);

    /// @ostream{configuration parameter}
    QDataStream &operator<<(QDataStream &stream, ConfigurationServer::Parameter parameter);

    /// @istream{configuration parameter}
    QDataStream &operator>>(QDataStream &stream, ConfigurationServer::Parameter &parameter);
}

Q_DECLARE_METATYPE(CuteVR::ConfigurationServer::Feature)

Q_DECLARE_METATYPE(CuteVR::ConfigurationServer::Parameter)

#endif // CUTE_VR_CONFIGURATION
