/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_IDENTIFIER
#define CUTE_VR_IDENTIFIER

#include <QtCore/QtGlobal>

namespace CuteVR {
    /// @brief Data type for identification in general within the library.
    using Identifier = quint32;
    /// @brief This identifier is used as default or error value.
    constexpr Identifier invalidIdentifier{0xFFFFFFFF};
}

#endif // CUTE_VR_IDENTIFIER
