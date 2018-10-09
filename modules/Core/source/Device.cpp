/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#include <cassert>
#include <QtCore/QReadWriteLock>
#include <openvr.h>

#include <CuteVR/Internal/DefaultDescriptionsProvider.hpp>
#include <CuteVR/Device.hpp>
#include <CuteVR/DriverServer.hpp>

using namespace CuteVR;
using Components::Description;
using Extension::Optional;
using Internal::DefaultDescriptionsProvider;

namespace {
    struct RegisterMetaTypes {
        RegisterMetaTypes() {
            qRegisterMetaType<Device::Category>();
            qRegisterMetaType<QMap<Identifier, Description>>();
        }
    } registerMetaTypes; // NOLINT
}

namespace CuteVR {
    QDataStream &operator<<(QDataStream &stream, Device::Category const category) {
        return stream << static_cast<quint8>(category);
    }

    QDataStream &operator>>(QDataStream &stream, Device::Category &category) {
        return stream >> reinterpret_cast<quint8 &>(category);
    }

    QDataStream &operator<<(QDataStream &stream, QSharedPointer<Device> const &serializable) {
        return serializable->serialize(stream);
    }

    QDataStream &operator>>(QDataStream &stream, QSharedPointer<Device> &serializable) {
        return serializable->deserialize(stream);
    }
}

class Device::Private {
public: // variables
    QReadWriteLock initializeLock{QReadWriteLock::RecursionMode::Recursive};
    bool initialized{false};
    QSharedPointer<DefaultDescriptionsProvider> descriptionsProvider;
    QReadWriteLock updateLock{QReadWriteLock::RecursionMode::Recursive};
    bool current{true};
    QMap<Identifier, Description> descriptionsCurrent{};
    QMap<Description::Type, Identifier> descriptionsByType{};
};

Device::Device(Identifier const identifier) :
        identifier{identifier},
        _private{new Private} {}

Device::~Device() = default;

Optional<Identifier> Device::description(Description::Type const type) const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->descriptionsByType.contains(type)
           ? Extension::Optional<Identifier>{_private->descriptionsByType.value(type)}
           : Extension::Optional<Identifier>{};
}

Device::Category Device::category() const noexcept {
    return Category::undefined;
}

void Device::destroy() {
    QWriteLocker{&_private->initializeLock};
    if (_private->initialized) {
        _private->descriptionsProvider.clear();
        _private->initialized = false;
    }
}

bool Device::isDestroyed() const noexcept {
    QReadLocker{&_private->initializeLock};
    return !_private->initialized;
}

bool Device::equals(Device const &other) const noexcept {
    return (category() == other.category()) &&
           (identifier == other.identifier);
}

void Device::initialize() {
    QWriteLocker{&_private->initializeLock};
    if (!_private->initialized) {
        {
            QWriteLocker{&_private->updateLock};
            for (auto const &description : descriptions) {
                _private->descriptionsCurrent.insert(description.identifier, description);
                _private->descriptionsByType.insert(description.type, description.identifier);
            }
        }
        _private->descriptionsProvider
                .reset(new DefaultDescriptionsProvider{identifier, [&](Description const &description) {
                    QWriteLocker{&_private->updateLock};
                    if (!_private->descriptionsCurrent.contains(description.identifier) ||
                        _private->descriptionsCurrent.value(description.identifier) != description) {
                        _private->descriptionsCurrent.insert(description.identifier, description);
                        _private->current = false;
                        emit descriptionsChanged(_private->descriptionsCurrent);
                        emit descriptionChanged(description.identifier, description);
                    }
                }});
        DriverServer::announce(_private->descriptionsProvider.toWeakRef(), {identifier}, {
                vr::VREvent_PropertyChanged,
        });
        _private->initialized = true;
    }
}

bool Device::isInitialized() const noexcept {
    QReadLocker{&_private->initializeLock};
    return _private->initialized;
}

qint32 Device::orders(Device const &other) const noexcept {
    return identifier - other.identifier;
}

QDataStream &Device::serialize(QDataStream &stream) const {
    return stream << identifier;
}

QDataStream &Device::deserialize(QDataStream &stream) {
    Identifier testIdentifier;
    stream >> testIdentifier;
    assert(identifier == testIdentifier);
    return stream;
}

void Device::update() {
    QWriteLocker{&_private->updateLock};
    if (!_private->current) {
        descriptions = _private->descriptionsCurrent;
        _private->descriptionsByType.clear();
        for (auto const &description : descriptions) {
            _private->descriptionsByType.insert(description.type, description.identifier);
        }
        _private->current = true;
    }
}

bool Device::isCurrent() const noexcept {
    QReadLocker{&_private->updateLock};
    return _private->current;
}

#include "../include/CuteVR/moc_Device.cpp" // LEGACY: CMake 3.8 ignores include paths
