/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_EXTENSION_CUTE_EXCEPTION
#define CUTE_VR_EXTENSION_CUTE_EXCEPTION

#include <utility>
#include <QtCore/QException>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include <CuteVR/Interface/EqualityComparable.hpp>
#include <CuteVR/Interface/Serializable.hpp>

namespace CuteVR { namespace Extension {
    /// @brief Abstract class for exceptions used in CuteVR.
    /// @details It extends the standard exception with useful properties to give them more semantics and usage options.
    struct CuteException :
            public QException,
            public Interface::EqualityComparable<Extension::CuteException>,
            public Interface::Serializable {
    Q_GADGET
        Q_CLASSINFO("author", "Marcus Meeßen")
        Q_CLASSINFO("package", "CuteVR")
        Q_CLASSINFO("module", "Core")
        Q_CLASSINFO("revision", "b")
        /// @brief Provides an additional numerical hint to the exception.
        /// @details The meaning of this "magic" number can usually be obtained from the documentation of the respective
        /// method or function. If there is only one case in which the exception occurs, the hint is usually set to 0.
        Q_PROPERTY(quint32 hint MEMBER hint FINAL)
        /// @brief Provides a message as a human readable description of the exception.
        /// @details This message is mostly relevant for developers. To translate errors for the end user needs, it is
        /// better to use the hint.
        /// @see #what
        Q_PROPERTY(QString message MEMBER message FINAL)
        /// @brief Provides the severity of the exception.
        Q_PROPERTY(CuteVR::Extension::CuteException::Severity severity MEMBER severity FINAL)
        /// @brief Possibly contains a previous exception that may have led to this exception.
        Q_PROPERTY(QSharedPointer<CuteException> previous MEMBER previous FINAL)

    public: // types
        /// @brief Serves to classify the severity of an exception.
        /// @details Normally, only critical exceptions are thrown. Warnings and informative exceptions have already
        /// been handled at the respective point in the code, but often indicate programming faults.
        enum class Severity :
                quint8 {
            undefined, ///< Default if no value has been given.
            informative, ///< The exception points to minor problems.
            warning, ///< The exception has already been handled, but indicates misuse by the developer.
            critical, ///< The piece of code where the exception occurs cannot handle it itself.
        };

        Q_ENUM(Severity)

    public: // destructor
        ~CuteException() override = default;

    public: // methods
        /// @brief Provides the message in the way prescribed by std::exception.
        /// @return The message, but as a character array.
        char const *what() const noexcept override;

        bool equals(CuteException const &other) const noexcept override;

        QDataStream &serialize(QDataStream &stream) const override;

        QDataStream &deserialize(QDataStream &stream) override;

    public: // variables
        quint32 hint{0};
        QString message{};
        CuteVR::Extension::CuteException::Severity severity{Severity::undefined};
        QSharedPointer<CuteException> previous{nullptr};
    };

    /// @brief Adds some static polymorphism to CuteException to meet the standards of QException.
    /// @tparam DerivedT The class that derives from this template class.
    /// @pre DerivedT must be derived from this class in order to use the default constructor.
    template<class DerivedT>
    struct CuriousCuteException :
            public CuteException {
    public: // destructor
        ~CuriousCuteException() override = default;

    public: // methods
        /// @brief Creates a new instance of the class that inherits CuriousCuteException.
        /// @param hint A hint to narrow down the error.
        /// @param message A message to quickly help the developer understanding the problem.
        /// @param severity The severity of the exception.
        /// @return A new instance of the derived exception class.
        /// @version 0.8
        /// @deprecated Use the new create interface instead.
        static QSharedPointer<CuteException> create(quint32 const hint = 0, QString message = "",
                                                    Severity const severity = Severity::undefined) noexcept {
            QSharedPointer<DerivedT> exception{new DerivedT};
            exception->hint = hint;
            exception->message = std::move(message);
            exception->severity = severity;
            return exception;
        }

        /// @brief Creates a new instance of the class that inherits CuriousCuteException.
        /// @param hint A hint to narrow down the error.
        /// @param message A message to quickly help the developer understanding the problem.
        /// @param severity The severity of the exception.
        /// @param previous A previous exception that may have led to this exception.
        /// @return A new instance of the derived exception class.
        /// @version 1.0
        static QSharedPointer<CuteException> create(Severity const severity, quint32 const hint, QString message = "",
                                                    QSharedPointer<CuteException> previous =
                                                            QSharedPointer<CuteException>{nullptr}) noexcept {
            QSharedPointer<DerivedT> exception{new DerivedT};
            exception->severity = severity;
            exception->hint = hint;
            exception->message = std::move(message);
            exception->previous.swap(previous);
            return exception;
        }

        /// @brief Throws an exception with the type of the derived class.
        void raise() const override {
            throw *static_cast<DerivedT const *>(this);
        }

        /// @brief Clones the exception to send it between different threads.
        QException *clone() const override {
            return new DerivedT(*static_cast<DerivedT const *>(this));
        }

    private: // constructor, ensure CRTP is used correctly
        friend DerivedT;

        CuriousCuteException() = default;
    };

    /// @ostream{exception severity}
    QDataStream &operator<<(QDataStream &stream, CuteException::Severity severity);

    /// @istream{exception severity}
    QDataStream &operator>>(QDataStream &stream, CuteException::Severity &severity);

    /// @ostream{shared pointer of an exception}
    QDataStream &operator<<(QDataStream &stream, QSharedPointer<CuteException> const &serializable);

    /// @istream{shared pointer of an exception}
    QDataStream &operator>>(QDataStream &stream, QSharedPointer<CuteException> &serializable);
}}

Q_DECLARE_METATYPE(CuteVR::Extension::CuteException)

Q_DECLARE_METATYPE(CuteVR::Extension::CuteException::Severity)

Q_DECLARE_METATYPE(QSharedPointer<CuteVR::Extension::CuteException>)

#endif // CUTE_VR_EXTENSION_CUTE_EXCEPTION
