/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_GEOMETRY_CUBE
#define CUTE_VR_COMPONENTS_GEOMETRY_CUBE

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Geometry {
    /// @brief A Cube is defined by its edge length.
    /// @details The origin of the cube is set to (0, 0, 0) respectively its mass center, while it is aligned with the
    /// axes, and the upper right rear corner is at (a/2, a/2, a/2).
    struct Cube final :
            public CategorizedComponent<Component::Category::geometry, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The edge length of the cube in meters.
        Q_PROPERTY(qreal a MEMBER a FINAL)

    public: // destructor
        ~Cube() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        qreal a{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Geometry::Cube)

#endif // CUTE_VR_COMPONENTS_GEOMETRY_CUBE
