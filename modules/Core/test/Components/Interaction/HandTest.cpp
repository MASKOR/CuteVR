/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Hand.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Interaction::Hand;

class HandTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Hand hand{};
        hand.type = Hand::Type::varying;
        QTest::addColumn<Hand>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << hand;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Hand>(); }

    void equalityComparableInterface_data() {
        Hand left{}, right{};
        QTest::addColumn<Hand>("left");
        QTest::addColumn<Hand>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.type = Hand::Type::left;
        QTest::newRow("LeftHasNewTypeNow_ReturnsFalse") << left << right << false;
        right.type = Hand::Type::left;
        QTest::newRow("RightHasNewTypeNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Hand>(); }

    void serializableInterface_data() {
        Hand hand{};
        hand.type = Hand::Type::both;
        QTest::addColumn<Hand>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << hand;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Hand>(); }
};

QTEST_APPLESS_MAIN(HandTest)

#include "Components/Interaction/HandTest.moc"
