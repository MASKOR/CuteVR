/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Input/Button.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Input::Button;
using Extension::Trilean;

class ButtonTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Button button{};
        button.touched = Trilean::no;
        button.pressed = Trilean::yes;
        QTest::addColumn<Button>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << button;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Button>(); }

    void equalityComparableInterface_data() {
        Button left{}, right{};
        QTest::addColumn<Button>("left");
        QTest::addColumn<Button>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.pressed = Trilean::no;
        QTest::newRow("LeftHasNewPressedNow_ReturnsFalse") << left << right << false;
        right.pressed = Trilean::no;
        QTest::newRow("RightHasNewPressedNow_ReturnsTrue") << left << right << true;
        left.touched = Trilean::yes;
        QTest::newRow("LeftHasNewTouchedNow_ReturnsFalse") << left << right << false;
        right.touched = Trilean::yes;
        QTest::newRow("RightHasNewTouchedNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Button>(); }

    void serializableInterface_data() {
        Button button{};
        button.touched = Trilean::yes;
        button.pressed = Trilean::no;
        QTest::addColumn<Button>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << button;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Button>(); }
};

QTEST_APPLESS_MAIN(ButtonTest)

#include "Components/Input/ButtonTest.moc"
