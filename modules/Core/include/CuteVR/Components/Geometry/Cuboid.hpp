/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_GEOMETRY_CUBOID
#define CUTE_VR_COMPONENTS_GEOMETRY_CUBOID

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Geometry {
    /// @brief A Cuboid is defined by its three edge lengths.
    /// @details The origin of the cuboid is set to (0, 0, 0) respectively its mass center, while it is aligned with
    /// the axes, and the upper right rear corner is at (a/2, b/2, c/2).
    struct Cuboid final :
            public CategorizedComponent<Component::Category::geometry, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief One edge length of the cuboid in meters, by our convention a is the width along the x-axis.
        Q_PROPERTY(qreal a MEMBER a FINAL)
        /// @brief One edge length of the cuboid in meters, by our convention b is the height along the y-axis.
        Q_PROPERTY(qreal b MEMBER b FINAL)
        /// @brief One edge length of the cuboid in meters, by our convention c is the depth along the z-axis.
        Q_PROPERTY(qreal c MEMBER c FINAL)

    public: // destructor
        ~Cuboid() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        qreal a{0.0};
        qreal b{0.0};
        qreal c{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Geometry::Cuboid)

#endif // CUTE_VR_COMPONENTS_GEOMETRY_CUBOID
