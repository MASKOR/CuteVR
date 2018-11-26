/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Vector4.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Vector4::from;

class Vector4Test :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdVector4Given_ReturnsEquivalentQVector4D() {
        vr::HmdVector4_t vector{};
        for (auto row = 0; row < 4; row++) {
            vector.v[row] = static_cast<float>(row);
        }
        auto qVector{from(vector)};
        for (auto row = 0; row < 4; row++) {
            QVERIFY(vector.v[row] == qVector[row]);
        }
    }

    void from_QVector4DGiven_ReturnsEquivalentHmdVector4() {
        QVector4D qVector{};
        for (auto row = 0; row < 4; row++) {
            qVector[row] = static_cast<float>(row);
        }
        auto vector{from(qVector)};
        for (auto row = 0; row < 4; row++) {
            QVERIFY(qVector[row] == vector.v[row]);
        }
    }
};

QTEST_APPLESS_MAIN(Vector4Test)

#include "Internal/Vector4Test.moc"

#endif // CUTE_VR_OPEN_VR
