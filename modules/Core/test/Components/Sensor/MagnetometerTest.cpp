/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Magnetometer.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Sensor::Magnetometer;

class MagnetometerTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Magnetometer magnetometer{};
        magnetometer.bias.setValue(QVector3D{3.0f, 2.0f, 1.0f});
        magnetometer.fieldStrength = {6.0f, 5.0f, 4.0f};
        QTest::addColumn<Magnetometer>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << magnetometer;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Magnetometer>(); }

    void equalityComparableInterface_data() {
        Magnetometer left{}, right{};
        QTest::addColumn<Magnetometer>("left");
        QTest::addColumn<Magnetometer>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("LeftHasNewBiasNow_ReturnsFalse") << left << right << false;
        right.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("RightHasNewBiasNow_ReturnsTrue") << left << right << true;
        left.fieldStrength = {4.0f, 5.0f, 6.0f};
        QTest::newRow("LeftHasNewFieldStrengthNow_ReturnsFalse") << left << right << false;
        right.fieldStrength = {4.0f, 5.0f, 6.0f};
        QTest::newRow("RightHasNewFieldStrengthNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Magnetometer>(); }

    void serializableInterface_data() {
        Magnetometer magnetometer{};
        magnetometer.bias.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        magnetometer.fieldStrength = {4.0f, 5.0f, 6.0f};
        QTest::addColumn<Magnetometer>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << magnetometer;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Magnetometer>(); }
};

QTEST_APPLESS_MAIN(MagnetometerTest)

#include "Components/Sensor/MagnetometerTest.moc"
