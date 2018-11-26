/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Matrix3x3.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Matrix3x3::from;

class Matrix3x3Test :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdMatrix33Given_ReturnsEquivalentQMatrix3x3() {
        vr::HmdMatrix33_t matrix{};
        for (auto row = 0; row < 3; row++) {
            for (auto column = 0; column < 3; column++) {
                matrix.m[row][column] = static_cast<float>(row * 3 + column);
            }
        }
        auto qMatrix{from(matrix)};
        for (auto row = 0; row < 3; row++) {
            for (auto column = 0; column < 3; column++) {
                QVERIFY(matrix.m[row][column] == qMatrix(row, column));
            }
        }
    }

    void from_QMatrix3x3Given_ReturnsEquivalentHmdMatrix33() {
        QMatrix3x3 qMatrix{};
        for (auto row = 0; row < 3; row++) {
            for (auto column = 0; column < 3; column++) {
                qMatrix(row, column) = static_cast<float>(row * 3 + column);
            }
        }
        auto matrix{from(qMatrix)};
        for (auto row = 0; row < 3; row++) {
            for (auto column = 0; column < 3; column++) {
                QVERIFY(qMatrix(row, column) == matrix.m[row][column]);
            }
        }
    }
};

QTEST_APPLESS_MAIN(Matrix3x3Test)

#include "Internal/Matrix3x3Test.moc"

#endif // CUTE_VR_OPEN_VR
