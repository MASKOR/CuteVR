/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <CuteVR/Extension/CuteException.hpp>

using namespace CuteVR;
using Extension::CuteException;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<CuteException>();
            qRegisterMetaType<CuteException::Severity>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR { namespace Extension {
    QDataStream &operator<<(QDataStream &stream, CuteException::Severity const severity) {
        return stream << static_cast<quint8>(severity);
    }

    QDataStream &operator>>(QDataStream &stream, CuteException::Severity &severity) {
        return stream >> reinterpret_cast<quint8 &>(severity);
    }

    QDataStream &operator<<(QDataStream &stream, QSharedPointer<CuteException> const &serializable) {
        return serializable->serialize(stream);
    }

    QDataStream &operator>>(QDataStream &stream, QSharedPointer<CuteException> &serializable) {
        return serializable->deserialize(stream);
    }
}}

const char *CuteException::what() const noexcept {
    return message.toUtf8().constData();
}

QDataStream &CuteException::serialize(QDataStream &stream) const {
    return stream << hint << message << severity;
}

QDataStream &CuteException::deserialize(QDataStream &stream) {
    return stream >> hint >> message >> severity;
}

bool CuteException::equals(CuteException const &other) const noexcept {
    return (hint == other.hint) &&
           (message == other.message) &&
           (severity == other.severity);
}

#include "../../include/CuteVR/Extension/moc_CuteException.cpp" // LEGACY: CMake 3.8 ignores include paths
