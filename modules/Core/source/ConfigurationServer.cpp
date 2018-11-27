/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QReadWriteLock>

#include <CuteVR/ConfigurationServer.hpp>

using namespace CuteVR;
using Extension::CuteException;
using Extension::Either;
using Extension::Optional;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<ConfigurationServer::Feature>();
            qRegisterMetaType<ConfigurationServer::Parameter>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR {
    // As stated here http://doc.qt.io/qt-5/qhash.html#the-qhash-hashing-function:
    // "A QHash's key type has additional requirements other than being an assignable data type: [...] there must also
    // be a qHash() function in the type's namespace that returns a hash value for an argument of the key's type."
    /// @private
    inline uint qHash(ConfigurationServer::Feature key) {
        return ::qHash(static_cast<quint16>(key));
    }

    /// @private
    inline uint qHash(ConfigurationServer::Parameter key) {
        return ::qHash(static_cast<quint16>(key));
    }

    QDataStream &operator<<(QDataStream &stream, ConfigurationServer::Feature feature) {
        return stream << static_cast<quint16>(feature);
    }

    QDataStream &operator>>(QDataStream &stream, ConfigurationServer::Feature &feature) {
        return stream >> reinterpret_cast<quint16 &>(feature);
    }

    QDataStream &operator<<(QDataStream &stream, ConfigurationServer::Parameter parameter) {
        return stream << static_cast<quint16>(parameter);
    }

    QDataStream &operator>>(QDataStream &stream, ConfigurationServer::Parameter &parameter) {
        return stream >> reinterpret_cast<quint16 &>(parameter);
    }
}

class ConfigurationServer::Private {
public: // types
    struct FeatureSetup {
        bool enabled{false};
        bool enabledByDefault{false};
        bool supported{false};
        bool supportedNative{false};
        std::function<Optional<QSharedPointer<CuteException>>(bool)> validator{};
    };

    struct ParameterSetup {
        QVariant value{};
        QVariant valueByDefault{};
        QVariant::Type valueType{QVariant::Type::Invalid};
        std::function<Optional<QSharedPointer<CuteException>>(QVariant)> validator{};
    };

public: // constructor
    explicit Private(ConfigurationServer *that) :
            that{that} {}

public: // methods
    Optional<QSharedPointer<CuteException>> setFeatureState(Feature const feature, bool const state) noexcept {
        QWriteLocker{&mutex};
        if (!featureSetups.contains(feature)) {
            return Optional<QSharedPointer<CuteException>>{
                    FeatureNotRegistered::create(0, "Feature is not registered.", CuteException::Severity::warning)};
        } else if (!featureSetups.value(feature).supported) {
            return Optional<QSharedPointer<CuteException>>{
                    FeatureNotSupported::create(0, "Feature is not supported.", CuteException::Severity::warning)};
        } else if (featureSetups.value(feature).validator) {
            auto exception{featureSetups.value(feature).validator(state)};
            if (exception.hasValue()) {
                return exception;
            }
        }
        if (featureSetups.value(feature).enabled != state) {
            featureSetups[feature].enabled = state;
            emit that->featureChanged(feature, state);
        }
        return {};
    }

    Optional<QSharedPointer<CuteException>>
    setParameterValue(Parameter const parameter, QVariant const &value) noexcept {
        QWriteLocker{&mutex};
        if (!parameterSetups.contains(parameter)) {
            return Optional<QSharedPointer<CuteException>>{
                    ParameterNotRegistered::create(0, "Parameter is not registered.",
                                                   CuteException::Severity::warning)};
        } else if (parameterSetups.value(parameter).validator) {
            auto exception{parameterSetups.value(parameter).validator(value)};
            if (exception.hasValue()) {
                return exception;
            }
        }
        if (parameterSetups.value(parameter).value != value) {
            parameterSetups[parameter].value = value;
            emit that->parameterChanged(parameter, value);
        }
        return {};
    }

public: // variables
    ConfigurationServer *that{nullptr};
    QReadWriteLock mutex{QReadWriteLock::RecursionMode::Recursive};
    QHash<Feature, FeatureSetup> featureSetups{};
    QHash<Parameter, ParameterSetup> parameterSetups{};
    QSet<quint16> forbiddenEnumerators{
            generalCore, generalDriver, generalZeta,
            deviceCore, deviceUser, deviceZeta,
            renderCore, renderUser, renderZeta,
            audioCore, audioUser, audioZeta,
            videoCore, videoUser, videoZeta,
            inputCore, inputUser, inputZeta,
    };

private: // friends
    friend QDataStream &operator<<(QDataStream &stream, FeatureSetup const &featureSetup) {
        return stream << featureSetup.enabled;
    }

    friend QDataStream &operator>>(QDataStream &stream, FeatureSetup &featureSetup) {
        return stream >> featureSetup.enabled;
    }

    friend QDataStream &operator<<(QDataStream &stream, ParameterSetup const &parameterSetup) {
        return stream << parameterSetup.value;
    }

    friend QDataStream &operator>>(QDataStream &stream, ParameterSetup &parameterSetup) {
        return stream >> parameterSetup.value;
    }
};

ConfigurationServer::~ConfigurationServer() = default;

Either<QSharedPointer<CuteException>, bool> ConfigurationServer::isEnabled(Feature const feature) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->featureSetups.contains(feature)) {
        return Either<QSharedPointer<CuteException>, bool>{
                FeatureNotRegistered::create(CuteException::Severity::warning, 0, "Feature is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, bool>{_private->featureSetups.value(feature).enabled};
}

Either<QSharedPointer<CuteException>, bool> ConfigurationServer::isEnabledByDefault(Feature const feature) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->featureSetups.contains(feature)) {
        return Either<QSharedPointer<CuteException>, bool>{
                FeatureNotRegistered::create(CuteException::Severity::warning, 0, "Feature is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, bool>{_private->featureSetups.value(feature).enabledByDefault};
}

Either<QSharedPointer<CuteException>, bool> ConfigurationServer::isSupported(Feature const feature) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->featureSetups.contains(feature)) {
        return Either<QSharedPointer<CuteException>, bool>{
                FeatureNotRegistered::create(CuteException::Severity::warning, 0, "Feature is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, bool>{_private->featureSetups.value(feature).supported};
}

Either<QSharedPointer<CuteException>, bool> ConfigurationServer::isSupportedNative(Feature const feature) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->featureSetups.contains(feature)) {
        return Either<QSharedPointer<CuteException>, bool>{
                FeatureNotRegistered::create(CuteException::Severity::warning, 0, "Feature is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, bool>{_private->featureSetups.value(feature).supportedNative};
}

Either<QSharedPointer<CuteException>, QVariant> ConfigurationServer::value(Parameter const parameter) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->parameterSetups.contains(parameter)) {
        return Either<QSharedPointer<CuteException>, QVariant>{
                ParameterNotRegistered::create(CuteException::Severity::warning, 0, "Parameter is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, QVariant>{_private->parameterSetups.value(parameter).value};
}

Either<QSharedPointer<CuteException>, QVariant>
ConfigurationServer::valueByDefault(Parameter const parameter) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->parameterSetups.contains(parameter)) {
        return Either<QSharedPointer<CuteException>, QVariant>{
                ParameterNotRegistered::create(CuteException::Severity::warning, 0, "Parameter is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, QVariant>{_private->parameterSetups.value(parameter).valueByDefault};
}

Either<QSharedPointer<CuteException>, QVariant::Type>
ConfigurationServer::valueType(Parameter const parameter) noexcept {
    auto const &_private{instance()._private};
    QReadLocker{&_private->mutex};
    if (!_private->parameterSetups.contains(parameter)) {
        return Either<QSharedPointer<CuteException>, QVariant::Type>{
                ParameterNotRegistered::create(CuteException::Severity::warning, 0, "Parameter is not registered.")};
    }
    return Either<QSharedPointer<CuteException>, QVariant::Type>{_private->parameterSetups.value(parameter).valueType};
}

Optional<QSharedPointer<CuteException>> ConfigurationServer::enable(Feature const feature) noexcept {
    auto &_private{instance()._private};
    return _private->setFeatureState(feature, true);
}

Optional<QSharedPointer<CuteException>> ConfigurationServer::disable(Feature const feature) noexcept {
    auto &_private{instance()._private};
    return _private->setFeatureState(feature, false);
}

Optional<QSharedPointer<CuteException>>
ConfigurationServer::setValue(ConfigurationServer::Parameter const parameter, QVariant value) noexcept {
    auto &_private{instance()._private};
    return _private->setParameterValue(parameter, value);
}

Optional<QSharedPointer<CuteException>>
CuteVR::ConfigurationServer::reset(ConfigurationServer::Feature const feature) noexcept {
    // FIXME: not thread-safe
    auto &_private{instance()._private};
    auto either{isEnabledByDefault(feature)};
    return either.isLeft() ? Optional<QSharedPointer<CuteException>>{either.left()} :
           _private->setFeatureState(feature, either.right());
}

Optional<QSharedPointer<CuteException>>
ConfigurationServer::resetValue(ConfigurationServer::Parameter const parameter) noexcept {
    // FIXME: not thread-safe
    auto &_private{instance()._private};
    auto either{valueByDefault(parameter)};
    return either.isLeft() ? Optional<QSharedPointer<CuteException>>{either.left()} :
           _private->setParameterValue(parameter, either.right());
}

ConfigurationServer &ConfigurationServer::instance() noexcept {
    static ConfigurationServer instance{};
    return instance;
}

ConfigurationServer::ConfigurationServer() :
        _private{new Private{this}} {}

Optional<QSharedPointer<CuteException>>
CuteVR::ConfigurationServer::registerFeature(ConfigurationServer::Feature const feature, bool const enabledByDefault,
                                             bool const supported, bool const supportedNative,
                                             std::function<Optional<QSharedPointer<CuteException>>(bool)> validator) {
    auto const &_private{instance()._private};
    QWriteLocker{&_private->mutex};
    Private::FeatureSetup featureSetup{
            _private->featureSetups.contains(feature) ? _private->featureSetups.value(feature).enabled
                                                      : enabledByDefault,
            enabledByDefault, supported, supportedNative, std::move(validator)};
    Optional<QSharedPointer<CuteException>> exception{};
    if (static_cast<quint16 >(feature) >= reserved && static_cast<quint16 >(feature) <= 0xF000) {
        FeatureRegistrationFailed::create(CuteException::Severity::critical, 8000,
                                          "Enumerator value is placed in a reserved range.")->raise();
    }
    if (_private->forbiddenEnumerators.contains(static_cast<quint16>(feature))) {
        FeatureRegistrationFailed::create(CuteException::Severity::critical, 8001,
                                          "Enumerator value overrides a group range enumerator.")->raise();
    }
    if (featureSetup.supportedNative && !featureSetup.supported) {
        exception.setValue(FeatureRegistrationFailed::create(CuteException::Severity::warning, 4000,
                                                             "Inconsistent because native support but no support in "
                                                             "general. Assuming the feature is supported.",
                                                             exception.value(QSharedPointer<CuteException>{})));
        featureSetup.supported = true;
    }
    if (!featureSetup.supported && featureSetup.enabledByDefault) {
        exception.setValue(FeatureRegistrationFailed::create(CuteException::Severity::warning, 4001,
                                                             "Inconsistent because enabled by default but not "
                                                             "supported. Assuming the feature is disabled.",
                                                             exception.value(QSharedPointer<CuteException>{})));
        featureSetup.enabledByDefault = false;
    }
    if (featureSetup.validator) {
        Optional<QSharedPointer<CuteException>> result{};
        if (_private->featureSetups.contains(feature)) {
            result = featureSetup.validator(featureSetup.enabled);
            if (result.hasValue()) {
                result.value()->previous = exception.value(QSharedPointer<CuteException>{});
                exception.setValue(FeatureRegistrationFailed::create(CuteException::Severity::warning, 4002,
                                                                     "Validation failed for current state. Assume the "
                                                                     "default state withstands validation.",
                                                                     result.value()));
                featureSetup.enabled = featureSetup.enabledByDefault;
            }
        }
        result = featureSetup.validator(featureSetup.enabledByDefault);
        if (result.hasValue()) {
            result.value()->previous = exception.value(QSharedPointer<CuteException>{});
            FeatureRegistrationFailed::create(CuteException::Severity::critical, 8002,
                                              "Validation failed for default state.",
                                              result.value())->raise();
        }
    }
    _private->featureSetups.insert(feature, featureSetup);
    return {};
}

Optional<QSharedPointer<CuteException>>
CuteVR::ConfigurationServer::registerParameter(ConfigurationServer::Parameter const parameter,
                                               QVariant const valueByDefault, QVariant::Type const valueType,
                                               std::function<
                                                       Optional<QSharedPointer<CuteException>>(QVariant)> validator) {
    auto const &_private{instance()._private};
    QWriteLocker{&_private->mutex};
    Private::ParameterSetup parameterSetup{
            _private->parameterSetups.contains(parameter) ? _private->parameterSetups.value(parameter).value
                                                          : valueByDefault,
            valueByDefault, valueType, std::move(validator)};
    Optional<QSharedPointer<CuteException>> exception{};
    if (static_cast<quint16 >(parameter) >= reserved && static_cast<quint16 >(parameter) <= 0xF000) {
        ParameterRegistrationFailed::create(CuteException::Severity::critical, 8000,
                                            "Enumerator value is placed in a reserved range.")->raise();
    }
    if (_private->forbiddenEnumerators.contains(static_cast<quint16>(parameter))) {
        ParameterRegistrationFailed::create(CuteException::Severity::critical, 8001,
                                            "Enumerator value overrides a group range enumerator.")->raise();
    }
    if (parameterSetup.validator) {
        Optional<QSharedPointer<CuteException>> result{};
        if (_private->parameterSetups.contains(parameter)) {
            result = parameterSetup.validator(parameterSetup.value);
            if (result.hasValue()) {
                result.value()->previous = exception.value(QSharedPointer<CuteException>{});
                exception.setValue(ParameterRegistrationFailed::create(CuteException::Severity::warning, 4002,
                                                                       "Validation failed for current value. Assume the "
                                                                       "default value withstands validation.",
                                                                       result.value()));
                parameterSetup.value = parameterSetup.valueByDefault;
            }
        }
        result = parameterSetup.validator(parameterSetup.valueByDefault);
        if (result.hasValue()) {
            result.value()->previous = exception.value(QSharedPointer<CuteException>{});
            ParameterRegistrationFailed::create(CuteException::Severity::critical, 8002,
                                                "Validation failed for default value.",
                                                result.value())->raise();
        }
    }
    _private->parameterSetups.insert(parameter, parameterSetup);
    return {};
}

QDataStream &ConfigurationServer::serialize(QDataStream &stream) const {
    QReadLocker{&_private->mutex};
    return stream << _private->featureSetups << _private->parameterSetups;
}

QDataStream &ConfigurationServer::deserialize(QDataStream &stream) {
    QWriteLocker{&_private->mutex};
    return stream >> _private->featureSetups >> _private->parameterSetups;
}

#include "../include/CuteVR/moc_ConfigurationServer.cpp"
