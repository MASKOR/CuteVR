/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Vector2.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Vector2::from;

class Vector2Test :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdVector2Given_ReturnsEquivalentQVector2D() {
        vr::HmdVector2_t vector{};
        for (auto row = 0; row < 2; row++) {
            vector.v[row] = static_cast<float>(row);
        }
        auto qVector{from(vector)};
        for (auto row = 0; row < 2; row++) {
            QVERIFY(vector.v[row] == qVector[row]);
        }
    }

    void from_QVector2DGiven_ReturnsEquivalentHmdVector2() {
        QVector2D qVector{};
        for (auto row = 0; row < 2; row++) {
            qVector[row] = static_cast<float>(row);
        }
        auto vector{from(qVector)};
        for (auto row = 0; row < 2; row++) {
            QVERIFY(qVector[row] == vector.v[row]);
        }
    }
};

QTEST_APPLESS_MAIN(Vector2Test)

#include "Internal/Vector2Test.moc"

#endif // CUTE_VR_OPEN_VR
