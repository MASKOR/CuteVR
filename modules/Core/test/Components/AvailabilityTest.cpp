/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Availability.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Availability;
using Extension::Trilean;

class AvailabilityTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Availability availability{};
        availability.connected = Trilean::yes;
        availability.detected = Trilean::no;
        availability.calibrated = Trilean::yes;
        availability.visible = Trilean::no;
        QTest::addColumn<Availability>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << availability;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Availability>(); }

    void equalityComparableInterface_data() {
        Availability left{}, right{};
        QTest::addColumn<Availability>("left");
        QTest::addColumn<Availability>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.connected = Trilean::yes;
        QTest::newRow("LeftHasNewConnectedNow_ReturnsFalse") << left << right << false;
        right.connected = Trilean::yes;
        QTest::newRow("RightHasNewConnectedNow_ReturnsTrue") << left << right << true;
        left.detected = Trilean::no;
        QTest::newRow("LeftHasNewDetectedNow_ReturnsFalse") << left << right << false;
        right.detected = Trilean::no;
        QTest::newRow("RightHasNewDetectedNow_ReturnsTrue") << left << right << true;
        left.calibrated = Trilean::yes;
        QTest::newRow("LeftHasNewCalibratedNow_ReturnsFalse") << left << right << false;
        right.calibrated = Trilean::yes;
        QTest::newRow("RightHasNewCalibratedNow_ReturnsTrue") << left << right << true;
        left.visible = Trilean::no;
        QTest::newRow("LeftHasNewVisibleNow_ReturnsFalse") << left << right << false;
        right.visible = Trilean::no;
        QTest::newRow("RightHasNewVisibleNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Availability>(); }

    void serializableInterface_data() {
        Availability availability{};
        availability.connected = Trilean::no;
        availability.detected = Trilean::yes;
        availability.calibrated = Trilean::no;
        availability.visible = Trilean::yes;
        QTest::addColumn<Availability>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << availability;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Availability>(); }
};

QTEST_APPLESS_MAIN(AvailabilityTest)

#include "Components/AvailabilityTest.moc"
