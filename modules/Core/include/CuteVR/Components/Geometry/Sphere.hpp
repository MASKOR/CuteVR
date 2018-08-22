/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_GEOMETRY_SPHERE
#define CUTE_VR_COMPONENTS_GEOMETRY_SPHERE

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Geometry {
    /// @brief A Sphere is defined by its radius.
    /// @details The origin of the sphere is set to (0, 0, 0) respectively its center.
    struct Sphere final :
            public CategorizedComponent<Component::Category::geometry, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The radius of the sphere in meters.
        Q_PROPERTY(qreal r MEMBER r FINAL)

    public: // destructor
        ~Sphere() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        qreal r{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Geometry::Sphere)

#endif // CUTE_VR_COMPONENTS_GEOMETRY_SPHERE
