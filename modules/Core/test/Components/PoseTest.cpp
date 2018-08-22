/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Pose.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Pose;
using Extension::Trilean;

class PoseTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Pose pose{};
        pose.valid = Trilean::no;
        pose.poseTransform.scale(3.0f);
        pose.linearVelocity.setValue(QVector3D{3.0f, 2.0f, 1.0f});
        pose.linearAcceleration.setValue(QVector3D{6.0f, 5.0f, 4.0f});
        pose.angularVelocity.setValue(QVector3D{9.0f, 8.0f, 7.0f});
        pose.angularAcceleration.setValue(QVector3D{12.0f, 11.0f, 10.0f});
        QTest::addColumn<Pose>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << pose;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Pose>(); }

    void equalityComparableInterface_data() {
        Pose left{}, right{};
        QTest::addColumn<Pose>("left");
        QTest::addColumn<Pose>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.valid = Trilean::yes;
        QTest::newRow("LeftHasNewValidNow_ReturnsFalse") << left << right << false;
        right.valid = Trilean::yes;
        QTest::newRow("RightHasNewValidNow_ReturnsTrue") << left << right << true;
        left.poseTransform.scale(2.0f);
        QTest::newRow("LeftHasNewPoseTransformNow_ReturnsFalse") << left << right << false;
        right.poseTransform.scale(2.0f);
        QTest::newRow("RightHasNewPoseTransformNow_ReturnsTrue") << left << right << true;
        left.linearVelocity.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("LeftHasNewLinearVelocityNow_ReturnsFalse") << left << right << false;
        right.linearVelocity.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        QTest::newRow("RightHasNewLinearVelocityNow_ReturnsTrue") << left << right << true;
        left.linearAcceleration.setValue(QVector3D{4.0f, 5.0f, 6.0f});
        QTest::newRow("LeftHasNewLinearAccelerationNow_ReturnsFalse") << left << right << false;
        right.linearAcceleration.setValue(QVector3D{4.0f, 5.0f, 6.0f});
        QTest::newRow("RightHasNewLinearAccelerationNow_ReturnsTrue") << left << right << true;
        left.angularVelocity.setValue(QVector3D{7.0f, 8.0f, 9.0f});
        QTest::newRow("LeftHasNewAngularVelocityNow_ReturnsFalse") << left << right << false;
        right.angularVelocity.setValue(QVector3D{7.0f, 8.0f, 9.0f});
        QTest::newRow("RightHasNewAngularVelocityNow_ReturnsTrue") << left << right << true;
        left.angularAcceleration.setValue(QVector3D{10.0f, 11.0f, 12.0f});
        QTest::newRow("LeftHasNewAngularAccelerationNow_ReturnsFalse") << left << right << false;
        right.angularAcceleration.setValue(QVector3D{10.0f, 11.0f, 12.0f});
        QTest::newRow("RightHasNewAngularAccelerationNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Pose>(); }

    void serializableInterface_data() {
        Pose pose{};
        pose.valid = Trilean::yes;
        pose.poseTransform.scale(3.0f);
        pose.linearVelocity.setValue(QVector3D{1.0f, 2.0f, 3.0f});
        pose.linearAcceleration.setValue(QVector3D{4.0f, 5.0f, 6.0f});
        pose.angularVelocity.setValue(QVector3D{7.0f, 8.0f, 9.0f});
        pose.angularAcceleration.setValue(QVector3D{10.0f, 11.0f, 12.0f});
        QTest::addColumn<Pose>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << pose;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Pose>(); }
};

QTEST_APPLESS_MAIN(PoseTest)

#include "Components/PoseTest.moc"
