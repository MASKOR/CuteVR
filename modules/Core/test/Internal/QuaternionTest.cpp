/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Quaternion.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Quaternion::from;

class QuaternionTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdQuaternionGiven_ReturnsEquivalentQQuaternion() {
        vr::HmdQuaternionf_t quaternion{0.1f, 0.2f, 0.3f, 0.4f};
        auto qQuaternion{from(quaternion)};
        QVERIFY(quaternion.w == qQuaternion.scalar());
        QVERIFY(quaternion.x == qQuaternion.x());
        QVERIFY(quaternion.y == qQuaternion.y());
        QVERIFY(quaternion.z == qQuaternion.z());
    }

    void from_QQuaternionGiven_ReturnsEquivalentHmdQuaternion() {
        QQuaternion qQuaternion{0.5f, 0.6f, 0.7f, 0.8f};
        auto quaternion{from(qQuaternion)};
        QVERIFY(qQuaternion.scalar() == quaternion.w);
        QVERIFY(qQuaternion.x() == quaternion.x);
        QVERIFY(qQuaternion.y() == quaternion.y);
        QVERIFY(qQuaternion.z() == quaternion.z);
    }
};

QTEST_APPLESS_MAIN(QuaternionTest)

#include "Internal/QuaternionTest.moc"

#endif // CUTE_VR_OPEN_VR
