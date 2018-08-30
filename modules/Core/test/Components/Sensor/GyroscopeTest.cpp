/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Gyroscope.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Sensor::Gyroscope;

class GyroscopeTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Gyroscope gyroscope{};
        gyroscope.bias.setValue(QVector3D{3.0f, 2.0f, 1.0f});
        gyroscope.angularVelocity = {6.0f, 5.0f, 4.0f};
        QTest::addColumn<Gyroscope>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << gyroscope;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Gyroscope>(); }

    void equalityComparableInterface_data() {
        Gyroscope left{}, right{};
        QTest::addColumn<Gyroscope>("left");
        QTest::addColumn<Gyroscope>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("LeftHasNewBiasNow_ReturnsFalse") << left << right << false;
        right.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("RightHasNewBiasNow_ReturnsTrue") << left << right << true;
        left.angularVelocity = {4.0f, 5.0f, 6.0f};
        QTest::newRow("LeftHasNewAngularVelocityNow_ReturnsFalse") << left << right << false;
        right.angularVelocity = {4.0f, 5.0f, 6.0f};
        QTest::newRow("RightHasNewAngularVelocityNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Gyroscope>(); }

    void serializableInterface_data() {
        Gyroscope gyroscope{};
        gyroscope.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        gyroscope.angularVelocity = {4.0f, 5.0f, 6.0f};
        QTest::addColumn<Gyroscope>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << gyroscope;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Gyroscope>(); }
};

QTEST_APPLESS_MAIN(GyroscopeTest)

#include "Components/Sensor/GyroscopeTest.moc"
