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
    ./source/Components/Availability.cpp
    ./source/Components/Description.cpp
    ./source/Components/Pose.cpp
    ./source/Configurations/Core.cpp
    ./source/Extension/CuteException.cpp
    ./source/Extension/Trilean.cpp
    ./source/Component.cpp
    ./source/ConfigurationServer.cpp
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
    ./test/Components/AvailabilityTest.cpp
    ./test/Components/DescriptionTest.cpp
    ./test/Components/PoseTest.cpp
    ./test/Extension/CuteExceptionTest.cpp
    ./test/Extension/EitherTest.cpp
    ./test/Extension/OptionalTest.cpp
    ./test/Extension/TrileanTest.cpp
    ./test/Internal/ColorTest.cpp
    ./test/Internal/Matrix3x3Test.cpp
    ./test/Internal/Matrix3x4Test.cpp
    ./test/Internal/Matrix4x4Test.cpp
    ./test/Internal/PropertyTest.cpp
    ./test/Internal/QuaternionTest.cpp
    ./test/Internal/Vector2Test.cpp
    ./test/Internal/Vector3Test.cpp
    ./test/Internal/Vector4Test.cpp
    ./test/ComponentTest.cpp
    ./test/ConfigurationServerTest.cpp)

# create module
add_library(Core SHARED "") # LEGACY: CMake 3.10 requires source files
decorate_module(Core "${_SOURCES}" "Qt5::Core;Qt5::Gui" "OpenVR::OpenVR")
test_module(Core "${_TESTS}")
install_module(Core "Qt5::Core;Qt5::Gui" "(Internal|Emulator)")
