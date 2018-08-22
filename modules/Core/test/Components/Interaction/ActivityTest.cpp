/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Activity.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Interaction::Activity;

class ActivityTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Activity activity{};
        activity.usage = Activity::Usage::now;
        QTest::addColumn<Activity>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << activity;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Activity>(); }

    void equalityComparableInterface_data() {
        Activity left{}, right{};
        QTest::addColumn<Activity>("left");
        QTest::addColumn<Activity>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.usage = Activity::Usage::recent;
        QTest::newRow("LeftHasNewUsageNow_ReturnsFalse") << left << right << false;
        right.usage = Activity::Usage::recent;
        QTest::newRow("RightHasNewUsageNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Activity>(); }

    void serializableInterface_data() {
        Activity activity{};
        activity.usage = Activity::Usage::standby;
        QTest::addColumn<Activity>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << activity;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Activity>(); }
};

QTEST_APPLESS_MAIN(ActivityTest)

#include "Components/Interaction/ActivityTest.moc"
