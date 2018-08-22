/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <cstring>

#include <CuteVR/Extension/CuteException.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Extension::CuteException;
using Extension::CuriousCuteException;

class TestException :
        public CuriousCuteException<TestException> {
};

Q_DECLARE_METATYPE(TestException)

class CuteExceptionTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void what_HasMessageGiven_ReturnsMessageAsCharConstPointer() {
        auto testException{TestException::create(0, "what")};
        QVERIFY(std::strcmp(testException->what(), "what") == 0);
    }

    void create_HasHintAndMessageAndSeverityGiven_NewHasSameHintAndMessageAndSeverity() {
        auto testException{TestException::create(12345, "clone", CuteException::Severity::warning)};
        QVERIFY(testException->hint == 12345);
        QVERIFY(testException->message == "clone");
        QVERIFY(testException->severity == CuteException::Severity::warning);
    }

    void raise_ExceptionIsTestException_ThrownExceptionIsTestException() {
        auto testException{TestException::create()};
        QVERIFY_EXCEPTION_THROWN(testException->raise(), TestException);
    }

    void clone_OriginHasHintAndMessageAndSeverityGiven_CloneHasSameDataButAnotherAddress() {
        auto testException{TestException::create(23456, "clone", CuteException::Severity::critical)};
        auto testExceptionClone{testException->clone()};
        QVERIFY(testExceptionClone != testException.data());
        QVERIFY(*dynamic_cast<TestException *>(testExceptionClone) == *testException);
        delete testExceptionClone;
    }

    void equalityComparableInterface_data() {
        TestException left{}, right{};
        QTest::addColumn<TestException>("left");
        QTest::addColumn<TestException>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.hint = 5;
        QTest::newRow("LeftHasNewHintNow_ReturnsFalse") << left << right << false;
        right.hint = 5;
        QTest::newRow("RightHasNewHintNow_ReturnsTrue") << left << right << true;
        left.message = "new";
        QTest::newRow("LeftHasNewMessageNow_ReturnsFalse") << left << right << false;
        right.message = "new";
        QTest::newRow("RightHasNewMessageNow_ReturnsTrue") << left << right << true;
        left.severity = CuteException::Severity::informative;
        QTest::newRow("LeftHasNewSeverityNow_ReturnsFalse") << left << right << false;
        right.severity = CuteException::Severity::informative;
        QTest::newRow("RightHasNewSeverityNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<TestException>(); }

    void serializableInterface_data() {
        TestException testException{};
        testException.hint = 3;
        testException.message = "new";
        testException.severity = CuteException::Severity::critical;
        QTest::addColumn<TestException>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << testException;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<TestException>(); }
};

QTEST_APPLESS_MAIN(CuteExceptionTest)

#include "Extension/CuteExceptionTest.moc"
