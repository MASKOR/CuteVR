/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_INTERFACE_SERIALIZABLE
#define CUTE_VR_INTERFACE_SERIALIZABLE

#include <QtCore/QDataStream>

namespace CuteVR { namespace Interface {
    /// @interface Serializable
    /// @brief The derived class can serialize into a data stream and deserialize from a data stream.
    class Serializable {
        friend QDataStream &operator<<(QDataStream &stream, Serializable const &serializable) {
            return serializable.serialize(stream);
        }

        friend QDataStream &operator>>(QDataStream &stream, Serializable &serializable) {
            return serializable.deserialize(stream);
        }

    public: // destructor
        virtual ~Serializable() = default;

    public: // methods
        /// @brief Inserts the values of the object into the output stream.
        /// @param stream The stream into which the values are inserted.
        /// @return Returns the given stream to allow chaining.
        virtual QDataStream &serialize(QDataStream &stream) const = 0;

        /// @brief Extracts the values from the input stream into the object.
        /// @param stream The stream from which the values are extracted.
        /// @return Returns the given stream to allow chaining.
        virtual QDataStream &deserialize(QDataStream &stream) = 0;
    };
}}

#endif // CUTE_VR_INTERFACE_SERIALIZABLE
