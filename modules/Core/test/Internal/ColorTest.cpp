/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtTest/QtTest>

#include <CuteVR/Internal/Color.hpp>

#ifdef CUTE_VR_OPEN_VR

using namespace CuteVR;
using Internal::Color::from;

class ColorTest :
        public QObject {
Q_OBJECT

private slots: // tests
    void from_HmdColorGiven_ReturnsEquivalentQColor() {
        qreal const qColorPrecision{1.0 / 0xFFFF};
        vr::HmdColor_t color{0.1f, 0.2f, 0.3f, 0.4f};
        auto qColor{from(color)};
        QVERIFY(qAbs(color.r - qColor.redF()) < qColorPrecision);
        QVERIFY(qAbs(color.g - qColor.greenF()) < qColorPrecision);
        QVERIFY(qAbs(color.b - qColor.blueF()) < qColorPrecision);
        QVERIFY(qAbs(color.a - qColor.alphaF()) < qColorPrecision);
    }

    void from_QColorGiven_ReturnsEquivalentHmdColor() {
        qreal const qColorPrecision{1.0 / 0xFFFF};
        QColor qColor{50, 60, 70, 80};
        auto color{from(qColor)};
        QVERIFY(qAbs(qColor.redF() - color.r) < qColorPrecision);
        QVERIFY(qAbs(qColor.greenF() - color.g) < qColorPrecision);
        QVERIFY(qAbs(qColor.blueF() - color.b) < qColorPrecision);
        QVERIFY(qAbs(qColor.alphaF() - color.a) < qColorPrecision);
    }
};

QTEST_APPLESS_MAIN(ColorTest)

#include "Internal/ColorTest.moc"

#endif // CUTE_VR_OPEN_VR
