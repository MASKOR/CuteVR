/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Extension::Trilean;

class TrileanTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void andOperator_data() {
        QTest::addColumn<Trilean>("left");
        QTest::addColumn<Trilean>("right");
        QTest::addColumn<Trilean>("result");
        QTest::newRow("LeftIsNoAndRightIsNo_ReturnsNo") << Trilean::no << Trilean::no << Trilean::no;
        QTest::newRow("LeftIsNoAndRightIsMaybe_ReturnsNo") << Trilean::no << Trilean::maybe << Trilean::no;
        QTest::newRow("LeftIsNoAndRightIsYes_ReturnsNo") << Trilean::no << Trilean::yes << Trilean::no;
        QTest::newRow("LeftIsMaybeAndRightIsNo_ReturnsNo") << Trilean::maybe << Trilean::no << Trilean::no;
        QTest::newRow("LeftIsMaybeAndRightIsMaybe_ReturnsMaybe") << Trilean::maybe << Trilean::maybe << Trilean::maybe;
        QTest::newRow("LeftIsMaybeAndRightIsYes_ReturnsMaybe") << Trilean::maybe << Trilean::yes << Trilean::maybe;
        QTest::newRow("LeftIsYesAndRightIsNo_ReturnsNo") << Trilean::yes << Trilean::no << Trilean::no;
        QTest::newRow("LeftIsYesAndRightIsMaybe_ReturnsMaybe") << Trilean::yes << Trilean::maybe << Trilean::maybe;
        QTest::newRow("LeftIsYesAndRightIsYes_ReturnsYes") << Trilean::yes << Trilean::yes << Trilean::yes;
    }

    void andOperator() {
        QFETCH(Trilean, left);
        QFETCH(Trilean, right);
        QFETCH(Trilean, result);
        QVERIFY((left && right) == result);
    }

    void orOperator_data() {
        QTest::addColumn<Trilean>("left");
        QTest::addColumn<Trilean>("right");
        QTest::addColumn<Trilean>("result");
        QTest::newRow("LeftIsNoAndRightIsNo_ReturnsNo") << Trilean::no << Trilean::no << Trilean::no;
        QTest::newRow("LeftIsNoAndRightIsMaybe_ReturnsMaybe") << Trilean::no << Trilean::maybe << Trilean::maybe;
        QTest::newRow("LeftIsNoAndRightIsYes_ReturnsYes") << Trilean::no << Trilean::yes << Trilean::yes;
        QTest::newRow("LeftIsMaybeAndRightIsNo_ReturnsMaybe") << Trilean::maybe << Trilean::no << Trilean::maybe;
        QTest::newRow("LeftIsMaybeAndRightIsMaybe_ReturnsMaybe") << Trilean::maybe << Trilean::maybe << Trilean::maybe;
        QTest::newRow("LeftIsMaybeAndRightIsYes_ReturnsYes") << Trilean::maybe << Trilean::yes << Trilean::yes;
        QTest::newRow("LeftIsYesAndRightIsNo_ReturnsYes") << Trilean::yes << Trilean::no << Trilean::yes;
        QTest::newRow("LeftIsYesAndRightIsMaybe_ReturnsYes") << Trilean::yes << Trilean::maybe << Trilean::yes;
        QTest::newRow("LeftIsYesAndRightIsYes_ReturnsYes") << Trilean::yes << Trilean::yes << Trilean::yes;
    }

    void orOperator() {
        QFETCH(Trilean, left);
        QFETCH(Trilean, right);
        QFETCH(Trilean, result);
        QVERIFY((left || right) == result);
    }

    void notOperator_data() {
        QTest::addColumn<Trilean>("left");
        QTest::addColumn<Trilean>("result");
        QTest::newRow("IsNo_ReturnsYes") << Trilean::no << Trilean::yes;
        QTest::newRow("IsMaybe_ReturnsMaybe") << Trilean::maybe << Trilean::maybe;
        QTest::newRow("IsYes_ReturnsNo") << Trilean::yes << Trilean::no;
    }

    void notOperator() {
        QFETCH(Trilean, left);
        QFETCH(Trilean, result);
        QVERIFY((!left) == result);
    }

    void serializableInterface_data() {
        QTest::addColumn<Trilean>("object");
        QTest::newRow("IsYes_StreamedIsYes") << Trilean::yes;
        QTest::newRow("IsNo_StreamedIsNo") << Trilean::no;
        QTest::newRow("IsMaybe_StreamedIsMaybe") << Trilean::maybe;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Trilean>(); }
};

QTEST_APPLESS_MAIN(TrileanTest)

#include "Extension/TrileanTest.moc"
