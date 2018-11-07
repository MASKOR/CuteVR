/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_COLOR
#define CUTE_VR_INTERNAL_COLOR

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QColor>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of colors between Qt and OpenVR format.
    namespace Color {
        /// @internal@brief Converts a color from OpenVR to Qt format.
        inline QColor from(vr::HmdColor_t const &o) {
            return QColor::fromRgbF(o.r, o.g, o.b, o.a);
        }

        /// @internal@brief Converts a color from Qt to OpenVR format.
        inline vr::HmdColor_t from(QColor const &o) {
            return {static_cast<float>(o.redF()), static_cast<float>(o.greenF()),
                    static_cast<float>(o.blueF()), static_cast<float>(o.alphaF())};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_COLOR
