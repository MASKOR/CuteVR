/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_MATRIX_4X4
#define CUTE_VR_INTERNAL_MATRIX_4X4

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QMatrix4x4>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of 4x4 matrices between Qt and OpenVR format.
    namespace Matrix4x4 {
        /// @internal@brief Converts an OpenVR 3x4 matrix to a Qt 4x4 matrix.
        inline QMatrix4x4 from(vr::HmdMatrix34_t const &o) {
            return {o.m[0][0], o.m[0][1], o.m[0][2], o.m[0][3],
                    o.m[1][0], o.m[1][1], o.m[1][2], o.m[1][3],
                    o.m[2][0], o.m[2][1], o.m[2][2], o.m[2][3],
                    0.0f, 0.0f, 0.0f, 1.0f};
        }

        /// @internal@brief Converts a 4x4 matrix from OpenVR to Qt format.
        inline QMatrix4x4 from(vr::HmdMatrix44_t const &o) {
            return {o.m[0][0], o.m[0][1], o.m[0][2], o.m[0][3],
                    o.m[1][0], o.m[1][1], o.m[1][2], o.m[1][3],
                    o.m[2][0], o.m[2][1], o.m[2][2], o.m[2][3],
                    o.m[3][0], o.m[3][1], o.m[3][2], o.m[3][3]};
        }

        /// @internal@brief Converts a 4x4 matrix from Qt to OpenVR format.
        inline vr::HmdMatrix44_t from(QMatrix4x4 const &o) {
            return {{{o(0, 0), o(0, 1), o(0, 2), o(0, 3)},
                     {o(1, 0), o(1, 1), o(1, 2), o(1, 3)},
                     {o(2, 0), o(2, 1), o(2, 2), o(2, 3)},
                     {o(3, 0), o(3, 1), o(3, 2), o(3, 3)}}};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_MATRIX_4X4
