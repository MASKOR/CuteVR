/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Devices/TrackedDevice.hpp>
#include <CuteVR/Internal/TestHelper.hpp>

using namespace CuteVR;
using Devices::TrackedDevice;

class TrackedDeviceTest :
        public QObject {
Q_OBJECT

private slots: // tests
};

QTEST_APPLESS_MAIN(TrackedDeviceTest)

#include "Devices/TrackedDeviceTest.moc"
