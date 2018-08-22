/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Peripheral/Battery.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Peripheral::Battery;
using Extension::Trilean;

class BatteryTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Battery battery{};
        battery.charging = Trilean::no;
        battery.level = 0.3f;
        QTest::addColumn<Battery>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << battery;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Battery>(); }

    void equalityComparableInterface_data() {
        Battery left{}, right{};
        QTest::addColumn<Battery>("left");
        QTest::addColumn<Battery>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.charging = Trilean::no;
        QTest::newRow("LeftHasNewChargingNow_ReturnsFalse") << left << right << false;
        right.charging = Trilean::no;
        QTest::newRow("RightHasNewChargingNow_ReturnsTrue") << left << right << true;
        left.level = 0.5f;
        QTest::newRow("LeftHasNewLevelNow_ReturnsFalse") << left << right << false;
        right.level = 0.5f;
        QTest::newRow("RightHasNewLevelNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Battery>(); }

    void serializableInterface_data() {
        Battery battery{};
        battery.charging = Trilean::yes;
        battery.level = 0.7f;
        QTest::addColumn<Battery>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << battery;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Battery>(); }
};

QTEST_APPLESS_MAIN(BatteryTest)

#include "Components/Peripheral/BatteryTest.moc"
