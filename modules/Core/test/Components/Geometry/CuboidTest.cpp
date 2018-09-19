/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Cuboid.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Geometry::Cuboid;

class CuboidTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Cuboid cuboid{};
        cuboid.a = 1.0f;
        cuboid.b = 2.0f;
        cuboid.c = 3.0f;
        QTest::addColumn<Cuboid>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << cuboid;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Cuboid>(); }

    void equalityComparableInterface_data() {
        Cuboid left{}, right{};
        QTest::addColumn<Cuboid>("left");
        QTest::addColumn<Cuboid>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.a = 4.0f;
        QTest::newRow("LeftHasNewANow_ReturnsFalse") << left << right << false;
        right.a = 4.0f;
        QTest::newRow("RightHasNewANow_ReturnsTrue") << left << right << true;
        left.b = 5.0f;
        QTest::newRow("LeftHasNewBNow_ReturnsFalse") << left << right << false;
        right.b = 5.0f;
        QTest::newRow("RightHasNewBNow_ReturnsTrue") << left << right << true;
        left.c = 6.0f;
        QTest::newRow("LeftHasNewCNow_ReturnsFalse") << left << right << false;
        right.c = 6.0f;
        QTest::newRow("RightHasNewCNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Cuboid>(); }

    void serializableInterface_data() {
        Cuboid cuboid{};
        cuboid.a = 7.0f;
        cuboid.b = 8.0f;
        cuboid.c = 9.0f;
        QTest::addColumn<Cuboid>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << cuboid;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Cuboid>(); }
};

QTEST_APPLESS_MAIN(CuboidTest)

#include "Components/Geometries/CuboidTest.moc"
