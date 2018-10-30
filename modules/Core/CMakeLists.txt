# Copyright (c) 2017-2018 Marcus Meeßen
# Copyright (c) 2018      MASKOR Institute FH Aachen
#
# CuteVR 'Core' module

find_package(OpenVR 1.0 REQUIRED)
find_package(Qt5 5.4.0 REQUIRED COMPONENTS Gui)

cpack_add_component(Core REQUIRED
        GROUP Library)

# collect sources
set(_SOURCES
    ./source/Components/Geometry/Cube.cpp
    ./source/Components/Geometry/Cuboid.cpp
    ./source/Components/Geometry/Sphere.cpp
    ./source/Components/Input/Axis.cpp
    ./source/Components/Input/Button.cpp
    ./source/Components/Interaction/Activity.cpp
    ./source/Components/Interaction/Eye.cpp
    ./source/Components/Interaction/Hand.cpp
    ./source/Components/Output/Display.cpp
    ./source/Components/Peripheral/Battery.cpp
    ./source/Components/Peripheral/Camera.cpp
    ./source/Components/Sensor/Accelerometer.cpp
    ./source/Components/Sensor/Gyroscope.cpp
    ./source/Components/Sensor/Magnetometer.cpp
    ./source/Components/Sensor/Proximity.cpp
    ./source/Components/Availability.cpp
    ./source/Components/Description.cpp
    ./source/Components/Pose.cpp
    ./source/Configurations/Core.cpp
    ./source/Devices/Controller/Generic.cpp
    ./source/Devices/HeadMountedDisplay/Generic.cpp
    ./source/Devices/Tracker/Generic.cpp
    ./source/Devices/TrackingReference/Generic.cpp
    ./source/Devices/TrackedDevice.cpp
    ./source/Extension/CuteException.cpp
    ./source/Extension/Trilean.cpp
    ./source/Internal/DefaultAvailabilityProvider.cpp
    ./source/Internal/DefaultAxesProvider.cpp
    ./source/Internal/DefaultButtonsProvider.cpp
    ./source/Internal/DefaultDescriptionsProvider.cpp
    ./source/Internal/DefaultDisplaysProvider.cpp
    ./source/Internal/DefaultEyesProvider.cpp
    ./source/Internal/DefaultHandsProvider.cpp
    ./source/Internal/DefaultPoseProvider.cpp
    ./source/Component.cpp
    ./source/ConfigurationServer.cpp
    ./source/Device.cpp
    ./source/DeviceServer.cpp
    ./source/DriverServer.cpp
    ./source/Identifier.cpp)
set(_TESTS
    ./test/Components/Geometry/CubeTest.cpp
    ./test/Components/Geometry/CuboidTest.cpp
    ./test/Components/Geometry/SphereTest.cpp
    ./test/Components/Input/AxisTest.cpp
    ./test/Components/Input/ButtonTest.cpp
    ./test/Components/Interaction/ActivityTest.cpp
    ./test/Components/Interaction/EyeTest.cpp
    ./test/Components/Interaction/HandTest.cpp
    ./test/Components/Output/DisplayTest.cpp
    ./test/Components/Peripheral/BatteryTest.cpp
    ./test/Components/Peripheral/CameraTest.cpp
    ./test/Components/Sensor/AccelerometerTest.cpp
    ./test/Components/Sensor/GyroscopeTest.cpp
    ./test/Components/Sensor/MagnetometerTest.cpp
    ./test/Components/Sensor/ProximityTest.cpp
    ./test/Components/AvailabilityTest.cpp
    ./test/Components/DescriptionTest.cpp
    ./test/Components/PoseTest.cpp
    ./test/Devices/Controller/GenericTest.cpp
    ./test/Devices/HeadMountedDisplay/GenericTest.cpp
    ./test/Devices/Tracker/GenericTest.cpp
    ./test/Devices/TrackingReference/GenericTest.cpp
    ./test/Devices/TrackedDeviceTest.cpp
    ./test/Extension/CuteExceptionTest.cpp
    ./test/Extension/EitherTest.cpp
    ./test/Extension/OptionalTest.cpp
    ./test/Extension/TrileanTest.cpp
    ./test/Internal/ColorTest.cpp
    ./test/Internal/DefaultAvailabilityProviderTest.cpp
    ./test/Internal/DefaultAxesProviderTest.cpp
    ./test/Internal/DefaultButtonsProviderTest.cpp
    ./test/Internal/DefaultDescriptionsProviderTest.cpp
    ./test/Internal/DefaultDisplaysProviderTest.cpp
    ./test/Internal/DefaultEyesProviderTest.cpp
    ./test/Internal/DefaultHandsProviderTest.cpp
    ./test/Internal/DefaultPoseProviderTest.cpp
    ./test/Internal/Matrix3x3Test.cpp
    ./test/Internal/Matrix3x4Test.cpp
    ./test/Internal/Matrix4x4Test.cpp
    ./test/Internal/PropertyTest.cpp
    ./test/Internal/QuaternionTest.cpp
    ./test/Internal/Vector2Test.cpp
    ./test/Internal/Vector3Test.cpp
    ./test/Internal/Vector4Test.cpp
    ./test/ComponentTest.cpp
    ./test/ConfigurationServerTest.cpp
    ./test/DeviceTest.cpp)

# create module
add_library(Core SHARED "") # LEGACY: CMake 3.10 requires source files
decorate_module(Core "${_SOURCES}" "Qt5::Core;Qt5::Gui" "OpenVR::OpenVR")
test_module(Core "${_TESTS}")
install_module(Core "Qt5::Core;Qt5::Gui" "(Internal|Emulator)")
