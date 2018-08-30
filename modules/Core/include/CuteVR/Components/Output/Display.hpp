/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_COMPONENTS_OUTPUT_DISPLAY
#define CUTE_VR_COMPONENTS_OUTPUT_DISPLAY

#include <QtCore/QSize>

#include <CuteVR/Component.hpp>

namespace CuteVR { namespace Components { namespace Output {
    /// @brief A Display that is used to present information or rendered graphics.
    struct Display final :
            public CategorizedComponent<Component::Category::output, Component> {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "a")
        /// @brief The native resolution of the display in pixels.
        Q_PROPERTY(QSize resolution MEMBER resolution FINAL)
        /// @brief The native refresh rate of the display in hertz.
        Q_PROPERTY(qreal refreshRate MEMBER refreshRate FINAL)

    public: // destructor
        ~Display() override = default;

    public: // methods
        QSharedPointer<Cloneable> clone() const override;

        bool equals(Component const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        QSize resolution{};
        qreal refreshRate{0.0};
    };
}}}

Q_DECLARE_METATYPE(CuteVR::Components::Output::Display)

#endif // CUTE_VR_COMPONENTS_OUTPUT_DISPLAY
