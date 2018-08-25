/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <QtCore/QMetaType>

#include <CuteVR/Identifier.hpp>

using namespace CuteVR;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Identifier>("CuteVR::Identifier");
        }
    } registerMetaTypes; // NOLINT
}
