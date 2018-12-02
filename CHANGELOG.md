<!--
Template:
### [major.minor.patch] YYYY-MM-DD
#### Changes
###### Added (optional)
###### Changed (optional)
###### Deprecated (optional)
###### Removed (optional)
###### Fixed (optional)
###### Security (optional)
#### Known Issues
M:modules
N:namespace
C:class
E:enum
F:function/method
V:variable
O:other
-->

Changelog
=========
All changes to the interface of CuteVR are summarized in this document and separated according to their release
versions. All mentioned versions are marked with the corresponding tag in our version management.

### Versions
###### Future
* [Closed Alpha](#closed-alpha)
* [Closed Beta](#closed-beta)
* Latest: \[B: _0.8.0-beta-dev.stage_]
###### Current
* [[0.8.0-beta]](#[0.8.0-beta]-2018-12-03) (master)
###### Former

---

### Preview
#### Closed Alpha
* M:Render
  * C:Model \[B: _0.8.x-dev.7_]: \
    Provides the 3D model to a specific device.
* M:Device
  * Rift CV1 support \[B: _0.8.x-dev.4_]
  * Rift DK1 support \[B: _0.8.x-dev.5_]
  * Rift DK2 support \[B: _0.8.x-dev.6_]

#### Closed Beta
* M:Render
  * C:Composer \[B: _0.8.1-beta-dev.1_]: \
    Sends a rendered image to the eye of a specific head-mounted display.
* M:Device
  * Vive 2016/1.0 support \[B: _0.8.1-beta-dev.2_]
  * Vive 2018/2.0/PRO support \[B: _0.8.1-beta-dev.3_]

---

### [0.8.0-beta] 2018-12-03
#### Changes 
###### Added
* [M:Core](https://cutevr.gitlab.io/CuteVR/core.html)
  * [C:ConfigurationServer](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1ConfigurationServer.html)
  * [N:Configurations](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Configurations.html)
    * [N:Core](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Configurations_1_1Core.html)
      * [E:Feature](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Configurations_1_1Core.html)
      / [E:Parameter](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Configurations_1_1Core.html)
  * [C:DriverServer](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1DriverServer.html)
  * [C:DeviceServer](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1DeviceServer.html)
  * [C:Component](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Component.html)
  * [N:Components](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components.html)
    * [N:Geometry](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Geometry.html)
      * [C:Cube](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Geometry_1_1Cube.html)
      / [C:Cuboid](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Geometry_1_1Cuboid.html)
      / [C:Sphere](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Geometry_1_1Sphere.html)
      * Placeholders: Cone, ConeFrustum, Cylinder, Ellipsoid, Pyramid, PyramidFrustum
    * [N:Input](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Input.html)
      * [C:Axis](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Input_1_1Axis.html)
      / [C:Button](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Input_1_1Button.html)
      * Placeholders: Key, Wheel
    * [N:Interaction](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Interaction.html)
      * [C:Activity](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Interaction_1_1Activity.html)
      / [C:Eye](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Interaction_1_1Eye.html)
      / [C:Hand](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Interaction_1_1Hand.html)
      * Placeholders: Ear
    * [N:Output](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Output.html)
      * [C:Display](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Output_1_1Display.html)
      * Placeholders: Haptic, Speaker
    * [N:Peripheral](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Peripheral.html)
      * [C:Battery](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Peripheral_1_1Battery.html)
      / [C:Camera](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Peripheral_1_1Camera.html)
    * [N:Sensor](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Components_1_1Sensor.html)
      * [C:Accelerometer](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Sensor_1_1Accelerometer.html)
      / [C:Gyroscope](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Sensor_1_1Gyroscope.html)
      / [C:Magnetometer](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Sensor_1_1Magnetometer.html)
      / [C:Proximity](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Sensor_1_1Proximity.html)
    * [C:Availability](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Availability.html)
    / [C:Description](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Description.html)
    / [C:Pose](https://cutevr.gitlab.io/CuteVR/structCuteVR_1_1Components_1_1Pose.html)
  * [C:Device](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Device.html)
  * [N:Devices](https://cutevr.gitlab.io/CuteVR/)
    * [N:Controller](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Devices_1_1Controller.html)
      * [C:Generic](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Devices_1_1Controller_1_1Generic.html)
    * [N:HeadMountedAudio](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Devices_1_1HeadMountedAudio.html)
      * Placeholders: Generic
    * [N:HeadMountedDisplay](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Devices_1_1HeadMountedDisplay.html)
      * [C:Generic](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Devices_1_1HeadMountedDisplay_1_1Generic.html)
    * [N:Tracker](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Devices_1_1Tracker.html)
      * [C:Generic](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Devices_1_1Tracker_1_1Generic.html)
    * [N:TrackingReference](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Devices_1_1TrackingReference.html)
      * [C:Generic](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Devices_1_1TrackingReference_1_1Generic.html)
    * [C:TrackedDevice](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Devices_1_1TrackedDevice.html)
  * [C:System](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1System.html)
  * [N:Extension](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Extension.html)
    * [C:CuteException](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Extension_1_1CuteException.html)
    / [C:Either](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Extension_1_1Either.html)
    / [C:Optional](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Extension_1_1Optional.html)
    / [E:Trilean](https://cutevr.gitlab.io/CuteVR/Trilean_8hpp.html)
  * [N:Interface](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Interface.html)
    * [C:Cloneable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1Cloneable.html)
    / [C:CyclicHandler](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1CyclicHandler.html)
    / [C:Destroyable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1Destroyable.html)
    / [C:EqualityComparable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1EqualityComparable.html)
    / [C:EventHandler](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1EventHandler.html)
    / [C:Initializable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1Initializable.html)
    / [C:OrderComparable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1OrderComparable.html)
    / [C:Serializable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1Serializable.html)
    / [C:TrackingHandler](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1TrackingHandler.html)
    / [C:Updatable](https://cutevr.gitlab.io/CuteVR/classCuteVR_1_1Interface_1_1Updatable.html)
  * [N:Internal](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal.html)
    * [N:Color](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Color.html) (OpenVR to Qt)
    * [N:Matrix3x3](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Matrix3x3.html) (OpenVR to Qt)
    * [N:Matrix3x4](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Matrix3x4.html) (OpenVR to Qt)
    * [N:Matrix4x4](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Matrix4x4.html) (OpenVR to Qt)
    * [N:Property](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Property.html) (OpenVR to Qt)
    * [N:Quaternion](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Quaternion.html) (OpenVR to Qt)
    * [N:Vector2](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Vector2.html) (OpenVR to Qt)
    * [N:Vector3](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Vector3.html) (OpenVR to Qt)
    * [N:Vector4](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Vector4.html) (OpenVR to Qt)
    * [C:TestComponent](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Vector4.html)
    / [C:TestDevice](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Internal_1_1Vector4.html)
    * [O:TestHelper](https://cutevr.gitlab.io/CuteVR/TestHelper_8hpp.html)
    * Undocumented: C:DefaultAvailabilityProvider
    / C:DefaultAxesProvider
    / C:DefaultButtonsProvider
    / C:DefaultDescriptionsProvider
    / C:DefaultDisplaysProvider
    / C:DefaultEyesProvider
    / C:DefaultHandsProvider
    / C:DefaultPoseProvider
  * [N:Emulator](https://cutevr.gitlab.io/CuteVR/namespaceCuteVR_1_1Emulator.html)
    * Undocumented: C:VRSystem / N:vr (OpenVR)

#### Known Issues
