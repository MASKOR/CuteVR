/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_VECTOR_4
#define CUTE_VR_INTERNAL_VECTOR_4

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QVector4D>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of four-dimensional vectors between Qt and OpenVR format.
    namespace Vector4 {
        /// @internal@brief Converts a four-dimensional vector from OpenVR to Qt format.
        inline QVector4D from(vr::HmdVector4_t const &o) {
            return {o.v[0], o.v[1], o.v[2], o.v[3]};
        }

        /// @internal@brief Converts a four-dimensional vector from Qt to OpenVR format.
        inline vr::HmdVector4_t from(QVector4D const &o) {
            return {{o[0], o[1], o[2], o[3]}};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_VECTOR_4
