/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERNAL_QUATERNION
#define CUTE_VR_INTERNAL_QUATERNION

#if defined(CUTE_VR_OPEN_VR) || defined(DOXYRUN)

#include <openvr.h>
#include <QtGui/QQuaternion>

namespace CuteVR { namespace Internal {
    /// @internal@brief Conversion of quaternions between Qt and OpenVR format.
    namespace Quaternion {
        /// @internal@brief Converts a quaternion from OpenVR to Qt format.
        inline QQuaternion from(vr::HmdQuaternionf_t const &o) {
            return QQuaternion{o.w, o.x, o.y, o.z};
        }

        /// @internal@brief Converts a quaternion from Qt to OpenVR format.
        inline vr::HmdQuaternionf_t from(QQuaternion const &o) {
            return {o.scalar(), o.x(), o.y(), o.z()};
        }
    }
}}

#endif // CUTE_VR_OPEN_VR

#endif // CUTE_VR_INTERNAL_QUATERNION
