/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Components/Geometry/Sphere.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Components::Geometry::Sphere;

class SphereTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void cloneableInterface_data() {
        Sphere sphere{};
        sphere.r = 1.0f;
        QTest::addColumn<Sphere>("object");
        QTest::newRow("HasAllValuesCustomized_ClonedHasSameValues") << sphere;
    }

    void cloneableInterface() { Internal::cloneableInterfaceTestHelper<Sphere>(); }

    void equalityComparableInterface_data() {
        Sphere left{}, right{};
        QTest::addColumn<Sphere>("left");
        QTest::addColumn<Sphere>("right");
        QTest::addColumn<bool>("result");
        QTest::newRow("LeftAndRightAreDefault_ReturnsTrue") << left << right << true;
        left.r = 2.0f;
        QTest::newRow("LeftHasNewRNow_ReturnsFalse") << left << right << false;
        right.r = 2.0f;
        QTest::newRow("RightHasNewRNow_ReturnsTrue") << left << right << true;
    }

    void equalityComparableInterface() { Internal::equalityComparableInterfaceTestHelper<Sphere>(); }

    void serializableInterface_data() {
        Sphere sphere{};
        sphere.r = 3.0f;
        QTest::addColumn<Sphere>("object");
        QTest::newRow("HasAllValuesCustomized_SerializedHasSameValues") << sphere;
    }

    void serializableInterface() { Internal::serializableInterfaceTestHelper<Sphere>(); }
};

QTEST_APPLESS_MAIN(SphereTest)

#include "Components/Geometry/SphereTest.moc"
