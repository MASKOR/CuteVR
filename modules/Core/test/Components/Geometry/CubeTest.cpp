/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Cube.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Geometry::Cube;

class CubeTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Cube cube{};
        cube.a = 2.0f;
        QTest::addColumn<Cube>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << cube;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Cube>(); }

    void equalityComparableInterface_data() {
        Cube left{}, right{};
        QTest::addColumn<Cube>("left");
        QTest::addColumn<Cube>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.a = 3.0f;
        QTest::newRow("LeftHasNewANow_ReturnsFalse") << left << right << false;
        right.a = 3.0f;
        QTest::newRow("RightHasNewANow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Cube>(); }

    void serializableInterface_data() {
        Cube cube{};
        cube.a = 4.0f;
        QTest::addColumn<Cube>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << cube;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Cube>(); }
};

QTEST_APPLESS_MAIN(CubeTest)

#include "Components/Geometry/CubeTest.moc"
