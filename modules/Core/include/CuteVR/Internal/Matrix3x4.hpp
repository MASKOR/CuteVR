/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_MATRIX_3X4
#define CUTE_VR_INTERNAL_MATRIX_3X4

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QMatrix4x3>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of 3x4 matrices between Qt and OpenVR format.
    /// @attention Qt strangely does not use the convention of first specifying the number of rows and then the number
    /// of columns. A `QMatrix3x4` is actually a 4x3 matrix, so caution is advised here.
    namespace Matrix3x4 {
        /// @internal@brief Converts a 3x4 matrix from OpenVR to Qt format.
        inline QMatrix4x3 from(vr::HmdMatrix34_t const &o) {
            float values[3][4] = {{o.m[0][0], o.m[0][1], o.m[0][2], o.m[0][3]},
                                  {o.m[1][0], o.m[1][1], o.m[1][2], o.m[1][3]},
                                  {o.m[2][0], o.m[2][1], o.m[2][2], o.m[2][3]}};
            return QMatrix4x3{*values};
        }

        /// @internal@brief Converts a 3x4 matrix from Qt to OpenVR format.
        inline vr::HmdMatrix34_t from(QMatrix4x3 const &o) {
            return {{{o(0, 0), o(0, 1), o(0, 2), o(0, 3)},
                     {o(1, 0), o(1, 1), o(1, 2), o(1, 3)},
                     {o(2, 0), o(2, 1), o(2, 2), o(2, 3)}}};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_MATRIX_3X4
