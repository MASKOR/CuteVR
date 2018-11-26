/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_MATRIX_3X3
#define CUTE_VR_INTERNAL_MATRIX_3X3

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QMatrix3x3>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of 3x3 matrices between Qt and OpenVR format.
    namespace Matrix3x3 {
        /// @internal@brief Converts a 3x3 matrix from OpenVR to Qt format.
        inline QMatrix3x3 from(vr::HmdMatrix33_t const &o) {
            float values[3][3] = {{o.m[0][0], o.m[0][1], o.m[0][2]},
                                  {o.m[1][0], o.m[1][1], o.m[1][2]},
                                  {o.m[2][0], o.m[2][1], o.m[2][2]}};
            return QMatrix3x3{*values};
        }

        /// @internal@brief Converts a 3x3 matrix from Qt to OpenVR format.
        inline vr::HmdMatrix33_t from(QMatrix3x3 const &o) {
            return {{{o(0, 0), o(0, 1), o(0, 2)},
                     {o(1, 0), o(1, 1), o(1, 2)},
                     {o(2, 0), o(2, 1), o(2, 2)}}};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_MATRIX_3X3
