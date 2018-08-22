/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Extension/Either.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Extension::Either;
using TestType = Either<qint32, QString>;

class EitherTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void constructor_NothingGiven_NewHasNeitherLeftNorRight() {
        TestType eitherWithNothing{};
        QVERIFY(!eitherWithNothing.isLeft());
        QVERIFY(!eitherWithNothing.isRight());
    }

    void constructor_OriginalLeftGiven_NewHasOriginalLeft() {
        TestType eitherWithLeft{originalLeft};
        QVERIFY(eitherWithLeft.left() == originalLeft);
    }

    void constructor_OriginalRightGiven_NewHasOriginalRight() {
        TestType eitherWithRight{originalRight};
        QVERIFY(eitherWithRight.right() == originalRight);
    }

    void constructor_LeftAndRightOfSameTypeAndOriginalRightGiven_NewHasOriginalRight() {
        Either<QString, QString> eitherWithRight{originalRight};
        QVERIFY(eitherWithRight.right() == originalRight);
    }

    void constructor_LeftAndRightOfConvertibleTypeAndOriginalLeftGiven_NewHasOriginalLeft() {
        Either<qint32, qint16> eitherWithLeft{originalLeft};
        QVERIFY(eitherWithLeft.left() == originalLeft);
    }

    void copyConstructor_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasNothing_NewAndOtherHaveNothing") << TestType{};
        QTest::newRow("OtherHasOriginalLeft_NewAndOtherHaveOriginalLeft") << TestType{originalLeft};
        QTest::newRow("OtherHasOriginalRight_NewAndOtherHaveOriginalRight") << TestType{originalRight};
    }

    void copyConstructor() { Internal::copyConstructorTestHelper<TestType>(); }

    void moveConstructor_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasNothing_NewAndOtherHaveNothing") << TestType{};
        QTest::newRow("OtherHasOriginalLeft_OriginalLeftHasMovedFromOtherToNew") << TestType{originalLeft};
        QTest::newRow("OtherHasOriginalRight_OriginalRightHasMovedFromOtherToNew") << TestType{originalRight};
    }

    void moveConstructor() { Internal::moveConstructorTestHelper<TestType>(); }

    void copyAssignmentOperator_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasNothing_NewAndOtherHaveNothing") << TestType{};
        QTest::newRow("OtherHasOriginalLeft_NewAndOtherHaveOriginalLeft") << TestType{originalLeft};
        QTest::newRow("OtherHasOriginalRight_NewAndOtherHaveOriginalRight") << TestType{originalRight};
    }

    void copyAssignmentOperator() { Internal::copyAssignmentOperatorTestHelper<TestType>(); }

    void moveAssignmentOperator_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("OtherHasNothing_NewAndOtherHaveNothing") << TestType{};
        QTest::newRow("OtherHasOriginalLeft_OriginalLeftHasMovedFromOtherToNew") << TestType{originalLeft};
        QTest::newRow("OtherHasOriginalRight_OriginalRightHasMovedFromOtherToNew") << TestType{originalRight};
    }

    void moveAssignmentOperator() { Internal::moveAssignmentOperatorTestHelper<TestType>(); }

    void left_HasNothing_ThrowsIsNotLeftException() {
        TestType eitherWithNothing{};
        QVERIFY_EXCEPTION_THROWN(eitherWithNothing.left(), TestType::IsNotLeft);
    }

    void left_HasNothingAndDefaultLeftGiven_ReturnsDefaultLeft() {
        TestType eitherWithNothing{};
        QVERIFY(eitherWithNothing.left(defaultLeft) == defaultLeft);
    }

    void left_HasOriginalLeft_ReturnsOriginalLeft() {
        TestType eitherWithLeft{originalLeft};
        QVERIFY(eitherWithLeft.left() == originalLeft);
    }

    void left_HasOriginalLeftAndDefaultLeftGiven_ReturnsOriginalLeft() {
        TestType eitherWithLeft{originalLeft};
        QVERIFY(eitherWithLeft.left(defaultLeft) == originalLeft);
    }

    void left_HasOriginalRight_ThrowsIsNotLeftException() {
        TestType eitherWithRight{originalRight};
        QVERIFY_EXCEPTION_THROWN(eitherWithRight.left(), TestType::IsNotLeft);
    }

    void left_HasOriginalRightAndDefaultLeftGiven_ReturnsDefaultLeft() {
        TestType eitherWithRight{originalRight};
        QVERIFY(eitherWithRight.left(defaultLeft) == defaultLeft);
    }

    void setLeft_data() {
        QTest::addColumn<TestType>("either");
        QTest::newRow("HasNothingAndSetNewLeft_HasNewLeft") << TestType{};
        QTest::newRow("HasOriginalLeftAndSetNewLeft_HasNewLeft") << TestType{originalLeft};
        QTest::newRow("HasOriginalRightAndSetNewLeft_HasNewLeft") << TestType{originalRight};
    }

    void setLeft() {
        QFETCH(TestType, either);
        either.setLeft(newLeft);
        QVERIFY(either.left() == newLeft);
    }

    void resetLeft_HasNothing_HasNothing() {
        TestType eitherWithNothing{};
        eitherWithNothing.resetLeft();
        QVERIFY(!eitherWithNothing.isLeft() && !eitherWithNothing.isRight());
    }

    void resetLeft_HasOriginalLeft_HasNothing() {
        TestType eitherWithLeft{originalLeft};
        eitherWithLeft.resetLeft();
        QVERIFY(!eitherWithLeft.isLeft() && !eitherWithLeft.isRight());
    }

    void resetLeft_HasOriginalRight_HasOriginalRight() {
        TestType eitherWithRight{originalRight};
        eitherWithRight.resetLeft();
        QVERIFY(eitherWithRight.isRight() && eitherWithRight.right() == originalRight);
    }

    void isLeft_data() {
        QTest::addColumn<TestType>("either");
        QTest::addColumn<bool>("result");
        QTest::newRow("HasNothing_ReturnsFalse") << TestType{} << false;
        QTest::newRow("HasOriginalLeft_ReturnsTrue") << TestType{originalLeft} << true;
        QTest::newRow("HasOriginalRight_ReturnsFalse") << TestType{originalRight} << false;
    }

    void isLeft() {
        QFETCH(TestType, either);
        QFETCH(bool, result);
        QVERIFY(either.isLeft() == result);
    }

    void right_HasNothing_ThrowsIsNotRightException() {
        TestType eitherWithNothing{};
        QVERIFY_EXCEPTION_THROWN(eitherWithNothing.right(), TestType::IsNotRight);
    }

    void right_HasNothingAndDefaultRightGiven_ReturnsDefaultRight() {
        TestType eitherWithNothing{};
        QVERIFY(eitherWithNothing.right(defaultRight) == defaultRight);
    }

    void right_HasOriginalLeft_ThrowsIsNotRightException() {
        TestType eitherWithLeft{originalLeft};
        QVERIFY_EXCEPTION_THROWN(eitherWithLeft.right(), TestType::IsNotRight);
    }

    void right_HasOriginalLeftAndDefaultRightGiven_ReturnsDefaultRight() {
        TestType eitherWithLeft{originalLeft};
        QVERIFY(eitherWithLeft.right(defaultRight) == defaultRight);
    }

    void right_HasOriginalRight_ReturnsOriginalRight() {
        TestType eitherWithRight{originalRight};
        QVERIFY(eitherWithRight.right() == originalRight);
    }

    void right_HasOriginalRightAndDefaultRightGiven_ReturnsOriginalRight() {
        TestType eitherWithRight{originalRight};
        QVERIFY(eitherWithRight.right(defaultRight) == originalRight);
    }

    void setRight_data() {
        QTest::addColumn<TestType>("either");
        QTest::newRow("HasNothingAndSetNewRight_HasNewRight") << TestType{};
        QTest::newRow("HasOriginalLeftAndSetNewRight_HasNewRight") << TestType{originalLeft};
        QTest::newRow("HasOriginalRightAndSetNewRight_HasNewRight") << TestType{originalRight};
    }

    void setRight() {
        QFETCH(TestType, either);
        either.setRight(newRight);
        QVERIFY(either.right() == newRight);
    }

    void resetRight_HasNothing_HasNothing() {
        TestType eitherWithNothing{};
        eitherWithNothing.resetRight();
        QVERIFY(!eitherWithNothing.isLeft() && !eitherWithNothing.isRight());
    }

    void resetRight_HasOriginalLeft_HasOriginalLeft() {
        TestType eitherWithLeft{originalLeft};
        eitherWithLeft.resetRight();
        QVERIFY(eitherWithLeft.isLeft() && eitherWithLeft.left() == originalLeft);
    }

    void resetRight_HasOriginalRight_HasNothing() {
        TestType eitherWithRight{originalRight};
        eitherWithRight.resetRight();
        QVERIFY(!eitherWithRight.isLeft() && !eitherWithRight.isRight());
    }

    void isRight_data() {
        QTest::addColumn<TestType>("either");
        QTest::addColumn<bool>("result");
        QTest::newRow("HasNothing_ReturnsFalse") << TestType{} << false;
        QTest::newRow("HasOriginalLeft_ReturnsFalse") << TestType{originalLeft} << false;
        QTest::newRow("HasOriginalRight_ReturnsTrue") << TestType{originalRight} << true;
    }

    void isRight() {
        QFETCH(TestType, either);
        QFETCH(bool, result);
        QVERIFY(either.isRight() == result);
    }

    void cloneableInterface_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("HasOriginalLeft_StreamedHasOriginalLeft") << TestType{originalLeft};
        QTest::newRow("HasOriginalRight_StreamedHasOriginalRight") << TestType{originalRight};
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<TestType>(); }

    void equalityComparableInterface_data() {
        TestType left{}, right{};
        QTest::addColumn<TestType>("left");
        QTest::addColumn<TestType>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.setLeft(newLeft);
        QTest::newRow("LeftHasNewLeftNow_ReturnsFalse") << left << right << false;
        right.setLeft(newLeft);
        QTest::newRow("RightHasNewLeftNow_ReturnsTrue") << left << right << true;
        left.setRight(newRight);
        QTest::newRow("LeftHasNewRightNow_ReturnsFalse") << left << right << false;
        right.setRight(newRight);
        QTest::newRow("RightHasNewRightNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<TestType>(); }

    void serializableInterface_data() {
        QTest::addColumn<TestType>("object");
        QTest::newRow("HasNothing_StreamedHasNothing") << TestType{};
        QTest::newRow("HasOriginalLeft_StreamedHasOriginalLeft") << TestType{originalLeft};
        QTest::newRow("HasOriginalRight_StreamedHasOriginalRight") << TestType{originalRight};
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<TestType>(); }

private: // constants
    qint32 const defaultLeft{11111};
    qint32 const originalLeft{22222};
    qint32 const newLeft{33333};
    QString const defaultRight{"default"};
    QString const originalRight{"original"};
    QString const newRight{"new"};
};

QTEST_APPLESS_MAIN(EitherTest)

#include "Extension/EitherTest.moc"
