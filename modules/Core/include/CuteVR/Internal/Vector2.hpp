/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_VECTOR_2
#define CUTE_VR_INTERNAL_VECTOR_2

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QVector2D>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of two-dimensional vectors between Qt and OpenVR format.
    namespace Vector2 {
        /// @internal@brief Converts a two-dimensional vector from OpenVR to Qt format.
        inline QVector2D from(vr::HmdVector2_t const &o) {
            return {o.v[0], o.v[1]};
        }

        /// @internal@brief Converts a two-dimensional vector from Qt to OpenVR format.
        inline vr::HmdVector2_t from(QVector2D const &o) {
            return {{o[0], o[1]}};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_VECTOR_2
