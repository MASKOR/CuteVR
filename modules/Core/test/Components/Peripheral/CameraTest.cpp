/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Peripheral/Camera.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Peripheral::Camera;

class CameraTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Camera camera{};
        camera.intrinsicParameters(0, 0) = 0.5f;
        camera.resolution = {300, 400};
        camera.frameRate = 7.0f;
        QTest::addColumn<Camera>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << camera;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Camera>(); }

    void equalityComparableInterface_data() {
        Camera left{}, right{};
        QTest::addColumn<Camera>("left");
        QTest::addColumn<Camera>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.intrinsicParameters(0, 0) = 2.0f;
        QTest::newRow("LeftHasNewIntrinsicParametersNow_ReturnsFalse") << left << right << false;
        right.intrinsicParameters(0, 0) = 2.0f;
        QTest::newRow("RightHasNewIntrinsicParametersNow_ReturnsTrue") << left << right << true;
        left.resolution = {100, 200};
        QTest::newRow("LeftHasNewResolutionNow_ReturnsFalse") << left << right << false;
        right.resolution = {100, 200};
        QTest::newRow("RightHasNewResolutionNow_ReturnsTrue") << left << right << true;
        left.frameRate = 3.0f;
        QTest::newRow("LeftHasNewFrameRateNow_ReturnsFalse") << left << right << false;
        right.frameRate = 3.0f;
        QTest::newRow("RightHasNewFrameRateNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Camera>(); }

    void serializableInterface_data() {
        Camera camera{};
        camera.intrinsicParameters(0, 0) = 2.0f;
        camera.resolution = {100, 200};
        camera.frameRate = 3.0f;
        QTest::addColumn<Camera>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << camera;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Camera>(); }
};

QTEST_APPLESS_MAIN(CameraTest)

#include "Components/Peripheral/CameraTest.moc"
