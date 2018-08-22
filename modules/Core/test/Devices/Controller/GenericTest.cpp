/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Devices/Controller/Generic.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;

class GenericTest :
        public QObject {
Q_OBJECT

private slots: // tests
};

QTEST_APPLESS_MAIN(GenericTest)

#include "Devices/Controller/GenericTest.moc"
