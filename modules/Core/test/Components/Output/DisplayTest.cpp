/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Output/Display.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Output::Display;

class DisplayTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Display display{};
        display.resolution = {300, 400};
        display.refreshRate = 5.0f;
        QTest::addColumn<Display>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << display;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Display>(); }

    void equalityComparableInterface_data() {
        Display left{}, right{};
        QTest::addColumn<Display>("left");
        QTest::addColumn<Display>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.resolution = {100, 200};
        QTest::newRow("LeftHasNewResolutionNow_ReturnsFalse") << left << right << false;
        right.resolution = {100, 200};
        QTest::newRow("RightHasNewResolutionNow_ReturnsTrue") << left << right << true;
        left.refreshRate = 3.0f;
        QTest::newRow("LeftHasNewFrequencyNow_ReturnsFalse") << left << right << false;
        right.refreshRate = 3.0f;
        QTest::newRow("RightHasNewFrequencyNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Display>(); }

    void serializableInterface_data() {
        Display display{};
        display.resolution = {100, 200};
        display.refreshRate = 3.0f;
        QTest::addColumn<Display>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << display;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Display>(); }
};

QTEST_APPLESS_MAIN(DisplayTest)

#include "Components/Output/DisplayTest.moc"
