/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Internal/TestDevice.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using TestDevice = Internal::TestDevice<>;

Q_DECLARE_METATYPE(QSharedPointer<TestDevice>)

class DeviceTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void equalityComparableInterface_data() {
        QSharedPointer<TestDevice> left{new TestDevice{0}}, right{new TestDevice{0}};
        QTest::addColumn<QSharedPointer<TestDevice>>("left");
        QTest::addColumn<QSharedPointer<TestDevice>>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.reset(new TestDevice{23456});
        QTest::newRow("LeftHasNewIdentifierNow_ReturnsFalse") << left << right << false;
        right.reset(new TestDevice{23456});
        QTest::newRow("RightHasNewIdentifierNow_ReturnsTrue") << left << right << true;
        left.reset(new TestDevice{0});
        left->dummyCategory = Device::Category::user;
        QTest::newRow("LeftHasNewCategoryNow_ReturnsFalse") << left << right << false;
        right.reset(new TestDevice{0});
        right->dummyCategory = Device::Category::user;
        QTest::newRow("RightHasNewCategoryNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<QSharedPointer<TestDevice>>(); }

    void orderComparableInterface_data() {
        QSharedPointer<TestDevice> left{new TestDevice{23456}}, right{new TestDevice{12345}};
        QTest::addColumn<QSharedPointer<TestDevice>>("left");
        QTest::addColumn<QSharedPointer<TestDevice>>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftIsGreaterThanRight_LeftGreaterRightReturnsTrue") << left << right << true;
        right.reset(new TestDevice{23456});
        QTest::newRow("LeftAndRightAreSame_LeftGreaterRightReturnsFalse") << left << right << false;
        right.reset(new TestDevice{34567});
        QTest::newRow("LeftIsSmallerThanRight_LeftGreaterRightReturnsFalse") << left << right << false;
    }

    void orderComparableInterface() { Internal::orderComparableInterfaceTestHelper<QSharedPointer<TestDevice>>(); }

    void serializableInterface_data() {
        QSharedPointer<TestDevice> device{new TestDevice{}};
        device->dummyCategory = Device::Category::user;
        QTest::addColumn<QSharedPointer<TestDevice>>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << device;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<QSharedPointer<TestDevice>>(); }
};

QTEST_APPLESS_MAIN(DeviceTest)

#include "DeviceTest.moc"
