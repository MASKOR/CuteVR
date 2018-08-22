/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Input/Axis.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Input::Axis;
using Extension::Trilean;

class AxisTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Axis axis{};
        axis.used = Trilean::no;
        axis.position = 0.5f;
        QTest::addColumn<Axis>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << axis;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Axis>(); }

    void equalityComparableInterface_data() {
        Axis left{}, right{};
        QTest::addColumn<Axis>("left");
        QTest::addColumn<Axis>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.used = Trilean::yes;
        QTest::newRow("LeftHasNewUsedNow_ReturnsFalse") << left << right << false;
        right.used = Trilean::yes;
        QTest::newRow("RightHasNewUsedNow_ReturnsTrue") << left << right << true;
        left.position = 1.0f;
        QTest::newRow("LeftHasNewPositionNow_ReturnsFalse") << left << right << false;
        right.position = 1.0f;
        QTest::newRow("RightHasNewPositionNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Axis>(); }

    void serializableInterface_data() {
        Axis axis{};
        axis.used = Trilean::yes;
        axis.position = 1.0f;
        QTest::addColumn<Axis>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << axis;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Axis>(); }
};

QTEST_APPLESS_MAIN(AxisTest)

#include "Components/Input/AxisTest.moc"
