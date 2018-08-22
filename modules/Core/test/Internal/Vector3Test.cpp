/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Vector3.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Vector3::from;

class Vector3Test :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdVector3Given_ReturnsEquivalentQVector3D() {
        vr::HmdVector3_t vector{};
        for (auto row = 0; row < 3; row++) {
            vector.v[row] = static_cast<float>(row);
        }
        auto qVector{from(vector)};
        for (auto row = 0; row < 3; row++) {
            QVERIFY(vector.v[row] == qVector[row]);
        }
    }

    void from_QVector3DGiven_ReturnsEquivalentHmdVector3() {
        QVector3D qVector{};
        for (auto row = 0; row < 3; row++) {
            qVector[row] = static_cast<float>(row);
        }
        auto vector{from(qVector)};
        for (auto row = 0; row < 3; row++) {
            QVERIFY(qVector[row] == vector.v[row]);
        }
    }
};

QTEST_APPLESS_MAIN(Vector3Test)

#include "Internal/Vector3Test.moc"

#endif // CUTE_VR_OPEN_VR
