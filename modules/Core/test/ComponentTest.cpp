/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Internal/TestComponent.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using TestComponent = Internal::TestComponent<>;

Q_DECLARE_METATYPE(TestComponent)

class ComponentTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        TestComponent component{};
        component.identifier = 12345;
        component.dummyCategory = Component::Category::user;
        QTest::addColumn<TestComponent>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << component;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<TestComponent>(); }

    void equalityComparableInterface_data() {
        TestComponent left{}, right{};
        QTest::addColumn<TestComponent>("left");
        QTest::addColumn<TestComponent>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.identifier = 23456;
        QTest::newRow("LeftHasNewIdentifierNow_ReturnsFalse") << left << right << false;
        right.identifier = 23456;
        QTest::newRow("RightHasNewIdentifierNow_ReturnsTrue") << left << right << true;
        left.dummyCategory = Component::Category::user;
        QTest::newRow("LeftHasNewCategoryNow_ReturnsFalse") << left << right << false;
        right.dummyCategory = Component::Category::user;
        QTest::newRow("RightHasNewCategoryNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<TestComponent>(); }

    void orderComparableInterface_data() {
        TestComponent left{}, right{};
        QTest::addColumn<TestComponent>("left");
        QTest::addColumn<TestComponent>("right");
        QTest::addColumn<bool>("result");
        left.identifier = 23456;
        right.identifier = 12345;
        QTest::newRow("LeftIsGreaterThanRight_LeftGreaterRightReturnsTrue") << left << right << true;
        right.identifier = 23456;
        QTest::newRow("LeftAndRightAreSame_LeftGreaterRightReturnsFalse") << left << right << false;
        right.identifier = 34567;
        QTest::newRow("LeftIsSmallerThanRight_LeftGreaterRightReturnsFalse") << left << right << false;
    }

    void orderComparableInterface() { Internal::orderComparableInterfaceTestHelper<TestComponent>(); }

    void serializableInterface_data() {
        TestComponent component{};
        component.identifier = 45678;
        component.dummyCategory = Component::Category::user;
        QTest::addColumn<TestComponent>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << component;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<TestComponent>(); }
};

QTEST_APPLESS_MAIN(ComponentTest)

#include "ComponentTest.moc"
