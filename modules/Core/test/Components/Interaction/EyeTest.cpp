/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Interaction/Eye.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Interaction::Eye;

class EyeTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Eye eye{};
        eye.type = Eye::Type::right;
        eye.perspectiveProjection.scale(0.5f);
        eye.perspectiveProjection.scale(0.25f);
        QTest::addColumn<Eye>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << eye;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Eye>(); }

    void equalityComparableInterface_data() {
        Eye left{}, right{};
        QTest::addColumn<Eye>("left");
        QTest::addColumn<Eye>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.type = Eye::Type::left;
        QTest::newRow("LeftHasNewTypeNow_ReturnsFalse") << left << right << false;
        right.type = Eye::Type::left;
        QTest::newRow("RightHasNewTypeNow_ReturnsTrue") << left << right << true;
        left.perspectiveProjection.scale(2.0f);
        QTest::newRow("LeftHasNewPerspectiveProjectionNow_ReturnsFalse") << left << right << false;
        right.perspectiveProjection.scale(2.0f);
        QTest::newRow("RightHasNewPerspectiveProjectionNow_ReturnsTrue") << left << right << true;
        left.headTransform.scale(4.0f);
        QTest::newRow("LeftHasNewHeadTransformNow_ReturnsFalse") << left << right << false;
        right.headTransform.scale(4.0f);
        QTest::newRow("RightHasNewHeadTransformNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Eye>(); }

    void serializableInterface_data() {
        Eye eye{};
        eye.type = Eye::Type::both;
        eye.perspectiveProjection.scale(3.0f);
        eye.headTransform.scale(9.0f);
        QTest::addColumn<Eye>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << eye;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Eye>(); }
};

QTEST_APPLESS_MAIN(EyeTest)

#include "Components/Interaction/EyeTest.moc"
