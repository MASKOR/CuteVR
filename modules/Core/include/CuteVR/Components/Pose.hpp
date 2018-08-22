/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_POSE
#define CUTE_VR_COMPONENTS_POSE

#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>

#include <CuteVR/Extension/Optional.hpp>
#include <CuteVR/Extension/Trilean.hpp>
#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components {
    /// @brief A Pose is defined by position and orientation and thus serves to spatially place an object.
    struct Pose final :
            public Component {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The pose is considered valid.
        /// @details This means the data was acquired correctly within the limits of hardware-specific capabilities.
        Q_PROPERTY(CuteVR::Extension::Trilean valid MEMBER valid FINAL)
        /// @brief A matrix that transforms e.g. a point from a world coordinate system into that of the pose.
        Q_PROPERTY(QMatrix4x4 poseTransform MEMBER poseTransform FINAL)
        /// @brief Linear velocity indicates how fast an object moves along a straight line, in meters per second.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> linearVelocity MEMBER linearVelocity FINAL)
        /// @brief Linear acceleration indicates how the velocity changes over time, in meters per second squared.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> linearAcceleration MEMBER linearAcceleration FINAL)
        /// @brief Angular velocity indicates how fast an object rotates around the given pose, in radians per second.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> angularVelocity MEMBER angularVelocity FINAL)
        /// @brief Angular acceleration indicates how the velocity changes over time, in radians per second squared.
        Q_PROPERTY(CuteVR::Extension::Optional<QVector3D> angularAcceleration MEMBER angularAcceleration FINAL)

    public: // destructor
        ~Pose() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        CuteVR::Extension::Trilean valid{Extension::maybe};
        QMatrix4x4 poseTransform{};
        CuteVR::Extension::Optional<QVector3D> linearVelocity{};
        CuteVR::Extension::Optional<QVector3D> linearAcceleration{};
        CuteVR::Extension::Optional<QVector3D> angularVelocity{};
        CuteVR::Extension::Optional<QVector3D> angularAcceleration{};
    };
}}

Q_DECLARE_METATYPE(CuteVR::Components::Pose)

#endif // CUTE_VR_COMPONENTS_POSE
