/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Sensor/Proximity.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Sensor::Proximity;

class ProximityTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Proximity proximity{};
        proximity.bias.setValue(0.5f);
        proximity.nearestDistance = 4.0f;
        QTest::addColumn<Proximity>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << proximity;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Proximity>(); }

    void equalityComparableInterface_data() {
        Proximity left{}, right{};
        QTest::addColumn<Proximity>("left");
        QTest::addColumn<Proximity>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.bias.setValue(1.0f);
        QTest::newRow("LeftHasNewBiasNow_ReturnsFalse") << left << right << false;
        right.bias.setValue(1.0f);
        QTest::newRow("RightHasNewBiasNow_ReturnsTrue") << left << right << true;
        left.nearestDistance = 2.0f;
        QTest::newRow("LeftHasNewLinearAccelerationNow_ReturnsFalse") << left << right << false;
        right.nearestDistance = 2.0f;
        QTest::newRow("RightHasNewLinearAccelerationNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Proximity>(); }

    void serializableInterface_data() {
        Proximity proximity{};
        proximity.bias.setValue(1.0f);
        proximity.nearestDistance = 2.0f;
        QTest::addColumn<Proximity>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << proximity;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Proximity>(); }
};

QTEST_APPLESS_MAIN(ProximityTest)

#include "Components/Sensor/ProximityTest.moc"
