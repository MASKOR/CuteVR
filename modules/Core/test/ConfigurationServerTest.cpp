/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Internal/TestHelper.hpp>
#include <CuteVR/ConfigurationServer.hpp>

using namespace CuteVR;

class ConfigurationServerTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void isEnabled_nonRegisteredFeature_returnsFeatureNotRegistered() {
        auto result{ConfigurationServer::isEnabled(ConfigurationServer::Feature::undefined)};
        QVERIFY(result.isLeft());
        QVERIFY_EXCEPTION_THROWN(result.left()->raise(), ConfigurationServer::FeatureNotRegistered);
    }

    void isEnabled_registeredFeature_returnsFeatureState() {}

    void registerFeature_enumeratorPlacedInReservedRange_throwsFeatureRegistrationFailedException() {
        QVERIFY_EXCEPTION_THROWN(ConfigurationServer::instance().registerFeature(static_cast<ConfigurationServer::Feature>(0xDFFF),
                                                                           true, true, true),
                                 ConfigurationServer::FeatureRegistrationFailed);
    }

    void registerFeature_enumeratorOverridesGroupRangeEnumerator_throwsFeatureRegistrationFailedException() {
        QVERIFY_EXCEPTION_THROWN(ConfigurationServer::instance().registerFeature(
                static_cast<ConfigurationServer::Feature>(ConfigurationServer::generalCore), true, true, true),
                                 ConfigurationServer::FeatureRegistrationFailed);
    }

    void registerFeature_validationFailedForDefaultState_throwsFeatureRegistrationFailedException() {
        QVERIFY_EXCEPTION_THROWN(ConfigurationServer::instance().registerFeature(
                static_cast<ConfigurationServer::Feature>(ConfigurationServer::generalCore + 1), true, true, true, [](bool) {
                    return Extension::Optional<QSharedPointer<Extension::CuteException>>{
                            ConfigurationServer::FeatureRegistrationFailed::create()};
                }),
                                 ConfigurationServer::FeatureRegistrationFailed);
    }

    // TODO: add missing unit tests
};

QTEST_APPLESS_MAIN(ConfigurationServerTest)

#include "ConfigurationServerTest.moc"
