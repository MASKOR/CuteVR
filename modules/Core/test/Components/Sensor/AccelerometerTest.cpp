/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Accelerometer.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Sensor::Accelerometer;

class AccelerometerTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Accelerometer accelerometer{};
        accelerometer.bias.setValue(QVector3D{3.0f, 2.0f, 1.0f});
        accelerometer.linearAcceleration = {6.0f, 5.0f, 4.0f};
        QTest::addColumn<Accelerometer>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << accelerometer;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Accelerometer>(); }

    void equalityComparableInterface_data() {
        Accelerometer left{}, right{};
        QTest::addColumn<Accelerometer>("left");
        QTest::addColumn<Accelerometer>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("LeftHasNewBiasNow_ReturnsFalse") << left << right << false;
        right.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("RightHasNewBiasNow_ReturnsTrue") << left << right << true;
        left.linearAcceleration = {4.0f, 5.0f, 6.0f};
        QTest::newRow("LeftHasNewLinearAccelerationNow_ReturnsFalse") << left << right << false;
        right.linearAcceleration = {4.0f, 5.0f, 6.0f};
        QTest::newRow("RightHasNewLinearAccelerationNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Accelerometer>(); }

    void serializableInterface_data() {
        Accelerometer accelerometer{};
        accelerometer.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        accelerometer.linearAcceleration = {4.0f, 5.0f, 6.0f};
        QTest::addColumn<Accelerometer>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << accelerometer;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Accelerometer>(); }
};

QTEST_APPLESS_MAIN(AccelerometerTest)

#include "Components/Sensor/AccelerometerTest.moc"
