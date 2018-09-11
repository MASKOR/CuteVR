/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Description.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Description;

class DescriptionTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Description description{"new"};
        description.type = Description::Type::cuteDeviceName;
        QTest::addColumn<Description>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << description;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Description>(); }

    void equalityComparableInterface_data() {
        Description left{}, right{};
        QTest::addColumn<Description>("left");
        QTest::addColumn<Description>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.append("new");
        QTest::newRow("LeftHasNewStringNow_ReturnsFalse") << left << right << false;
        right.append("new");
        QTest::newRow("RightHasNewStringNow_ReturnsTrue") << left << right << true;
        left.type = Description::Type::hardwareName;
        QTest::newRow("LeftHasNewTypeNow_ReturnsFalse") << left << right << false;
        right.type = Description::Type::hardwareName;
        QTest::newRow("RightHasNewTypeNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Description>(); }

    void serializableInterface_data() {
        Description description{"new"};
        description.type = Description::Type::hardwareRevision;
        QTest::addColumn<Description>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << description;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Description>(); }
};

QTEST_APPLESS_MAIN(DescriptionTest)

#include "Components/DescriptionTest.moc"
