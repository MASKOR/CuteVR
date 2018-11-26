/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Matrix4x4.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Matrix4x4::from;

class Matrix4x4Test :
        public QObject {
    Q_OBJECT

private slots: // tests
            void from_HmdMatrix44Given_ReturnsEquivalentQMatrix4x4() {
        vr::HmdMatrix44_t matrix{};
        for (auto row = 0; row < 4; row++) {
            for (auto column = 0; column < 4; column++) {
                matrix.m[row][column] = static_cast<float>(row * 4 + column);
            }
        }
        auto qMatrix{from(matrix)};
        for (auto row = 0; row < 4; row++) {
            for (auto column = 0; column < 4; column++) {
                QVERIFY(matrix.m[row][column] == qMatrix(row, column));
            }
        }
    }

    void from_QMatrix4x4Given_ReturnsEquivalentHmdMatrix44() {
        QMatrix4x4 qMatrix{};
        for (auto row = 0; row < 4; row++) {
            for (auto column = 0; column < 4; column++) {
                qMatrix(row, column) = static_cast<float>(row * 4 + column);
            }
        }
        auto matrix{from(qMatrix)};
        for (auto row = 0; row < 4; row++) {
            for (auto column = 0; column < 4; column++) {
                QVERIFY(qMatrix(row, column) == matrix.m[row][column]);
            }
        }
    }
};

QTEST_APPLESS_MAIN(Matrix4x4Test)

#include "Internal/Matrix4x4Test.moc"

#endif // CUTE_VR_OPEN_VR
