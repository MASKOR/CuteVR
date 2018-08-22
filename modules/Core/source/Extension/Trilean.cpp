/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Extension/Trilean.hpp>

using namespace CuteVR;
using Extension::Trilean;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Trilean>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Extension {
    QDataStream &operator<<(QDataStream &stream, Trilean trilean) {
        return stream << static_cast<quint8>(trilean);
    }

    QDataStream &operator>>(QDataStream &stream, Trilean &trilean) {
        return stream >> reinterpret_cast<quint8 &>(trilean);
    }
}}

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0) // LEGACY: Qt 5.7 has no Q_NAMESPACE/Q_ENUM_NS prevents MOC note
#include "../../include/CuteVR/Extension/moc_Trilean.cpp" // LEGACY: CMake 3.8 ignores include paths
#endif
