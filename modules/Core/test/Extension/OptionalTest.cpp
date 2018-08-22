/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Extension::Optional;
using TestType = Optional<QString>;

class OptionalTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void constructor_OriginalValueGiven_NewHasOriginalValue() {
        TestType optionalWithValue{originalValue};
        QVERIFY(optionalWithValue.value() == originalValue);
    }

    void constructor_NoValueGiven_NewHasNoValue() {
        TestType optionalWithNoValue{};
        QVERIFY(!optionalWithNoValue.hasValue());
    }

    void copyConstructor_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasOriginalValue_NewAndOtherHaveOriginalValue") << TestType{originalValue};
        QTest::newRow("OtherHasNoValue_NewAndOtherHaveNoValue") << TestType{};
    }

    void copyConstructor() { Internal::copyConstructorTestHelper<TestType>(); }

    void moveConstructor_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasOriginalValue_OriginalValueHasMovedFromOtherToNew") << TestType{originalValue};
        QTest::newRow("OtherHasNoValue_NewAndOtherHaveNoValue") << TestType{};
    }

    void moveConstructor() { Internal::moveConstructorTestHelper<TestType>(); }

    void copyAssignmentOperator_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasOriginalValue_NewAndOtherHaveOriginalValue") << TestType{originalValue};
        QTest::newRow("OtherHasNoValue_NewAndOtherHaveNoValue") << TestType{};
    }

    void copyAssignmentOperator() { Internal::copyAssignmentOperatorTestHelper<TestType>(); }

    void moveAssignmentOperator_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasOriginalValue_OriginalValueHasMovedFromOtherToNew") << TestType{originalValue};
        QTest::newRow("OtherHasNoValue_NewAndOtherHaveNoValue") << TestType{};
    }

    void moveAssignmentOperator() { Internal::moveAssignmentOperatorTestHelper<TestType>(); }

    void value_HasOriginalValue_ReturnsOriginalValue() {
        TestType optionalWithValue{originalValue};
        QVERIFY(optionalWithValue.value() == originalValue);
    }

    void value_HasOriginalValueAndDefaultValueGiven_ReturnsOriginalValue() {
        TestType optionalWithValue{originalValue};
        QVERIFY(optionalWithValue.value(defaultValue) == originalValue);
    }

    void value_HasNoValue_ThrowsHasNoValueException() {
        TestType optionalWithNoValue{};
        QVERIFY_EXCEPTION_THROWN(optionalWithNoValue.value(), TestType::HasNoValue);
    }

    void value_HasNoValueAndDefaultValueGiven_ReturnsDefaultValue() {
        TestType optionalWithNoValue{};
        QVERIFY(optionalWithNoValue.value(defaultValue) == defaultValue);
    }

    void setValue_data() {
        QTest::addColumn<TestType>("optional");
        QTest::newRow("HasOriginalValueAndSetNewValue_HasNewValue") << TestType{originalValue};
        QTest::newRow("HasNoValueAndSetNewValue_HasNewValue") << TestType{};
    }

    void setValue() {
        QFETCH(TestType, optional);
        optional.setValue(newValue);
        QVERIFY(optional.value() == newValue);
    }

    void resetValue_data() {
        QTest::addColumn<TestType>("optional");
        QTest::newRow("HasOriginalValue_HasNoValue") << TestType{originalValue};
        QTest::newRow("HasNoValue_HasNoValue") << TestType{};
    }

    void resetValue() {
        QFETCH(TestType, optional);
        optional.resetValue();
        QVERIFY(!optional.hasValue());
    }

    void hasValue_data() {
        QTest::addColumn<TestType>("optional");
        QTest::addColumn<bool>("result");
        QTest::newRow("HasOriginalValue_ReturnsTrue") << TestType{originalValue} << true;
        QTest::newRow("HasNoValue_ReturnsFalse") << TestType{} << false;
    }

    void hasValue() {
        QFETCH(TestType, optional);
        QFETCH(bool, result);
        QVERIFY(optional.hasValue() == result);
    }

    void cloneableInterface_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("HasOriginalValue_StreamedHasOriginalValue") << TestType{originalValue};
        QTest::newRow("HasNoValue_StreamedHasNoValue") << TestType{};
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<TestType>(); }

    void equalityComparableInterface_data() {
        TestType left{}, right{};
        QTest::addColumn<TestType>("left");
        QTest::addColumn<TestType>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.setValue(newValue);
        QTest::newRow("LeftHasNewValueNow_ReturnsFalse") << left << right << false;
        right.setValue(newValue);
        QTest::newRow("RightHasNewValueNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<TestType>(); }

    void serializableInterface_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("HasOriginalValue_StreamedHasOriginalValue") << TestType{originalValue};
        QTest::newRow("HasNoValue_StreamedHasNoValue") << TestType{};
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<TestType>(); }

private: // constants
    QString const defaultValue{"default"};
    QString const originalValue{"original"};
    QString const newValue{"new"};
};

QTEST_APPLESS_MAIN(OptionalTest)

#include "Extension/OptionalTest.moc"
