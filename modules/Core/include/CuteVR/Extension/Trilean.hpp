/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_EXTENSION_TRILEAN
#define CUTE_VR_EXTENSION_TRILEAN

#include <QtCore/QMetaType>
#include <QtCore/QDataStream>

namespace CuteVR { namespace Extension {
    /// @brief Names for the three different possible values of a three-valued logic.
    /// @details Allows easy use of three-valued logic and comparisons with Boolean values. For these values the
    /// following also applies: `no==false` and `yes==true`
    enum Trilean :
            quint8 {
        no, yes, maybe
    };

    /// @brief Calculates the value of the three-valued AND operation according to Kleene.
    /// @details
    /// | three-valued AND logic            |||||
    /// | :---: | :---: | :---: | :---: | :---: |
    /// | **A AND B**  || **B**               |||
    /// |^      |^      | no    | maybe | yes   |
    /// | **A** | no    | no    | no    | no    |
    /// |^      | maybe | no    | maybe | maybe |
    /// |^      | yes   | no    | maybe | yes   |
    /// @param left The left operand.
    /// @param right The right operand.
    /// @return The result of the AND operation (see details).
    constexpr Trilean operator&&(Trilean const left, Trilean const right) noexcept {
        return Trilean{left == no || right == no ? no : left == yes && right == yes ? yes : maybe};
    }

    /// @brief Calculates the value of the three-valued OR operation according to Kleene.
    /// @details
    /// | three-valued OR logic             |||||
    /// | :---: | :---: | :---: | :---: | :---: |
    /// | **A OR B**   || **B**               |||
    /// |^      |^      | no    | maybe | yes   |
    /// | **A** | no    | no    | maybe | yes   |
    /// |^      | maybe | maybe | maybe | yes   |
    /// |^      | yes   | yes   | yes   | yes   |
    /// @param left The left operand.
    /// @param right The right operand.
    /// @return The result of the OR operation (see details).
    constexpr Trilean operator||(Trilean const left, Trilean const right) noexcept {
        return Trilean{left == no && right == no ? no : left == yes || right == yes ? yes : maybe};
    }

    /// @brief
    /// @brief Calculates the value of the three-valued NOT operation according to Kleene.
    /// @param trilean The operand.
    /// @details
    /// | three-valued NOT logic ||
    /// | :---: | :-------------: |
    /// | **A** | **NOT A**       |
    /// | no    | yes             |
    /// | maybe | maybe           |
    /// | yes   | no              |
    /// @return
    constexpr Trilean operator!(Trilean const trilean) noexcept {
        return Trilean{trilean == yes ? no : trilean == no ? yes : maybe};
    }

    /// @ostream{trilean}
    QDataStream &operator<<(QDataStream &stream, Trilean trilean);

    /// @istream{trilean}
    QDataStream &operator>>(QDataStream &stream, Trilean &trilean);

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0) // LEGACY: Qt 5.7 has no Q_NAMESPACE/Q_ENUM_NS
    Q_NAMESPACE

    Q_ENUM_NS(Trilean)
#endif
}}

Q_DECLARE_METATYPE(CuteVR::Extension::Trilean)

#endif // CUTE_VR_EXTENSION_TRILEAN
