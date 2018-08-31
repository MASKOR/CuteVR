/// @file
/// @author Marcus Meeßen
/// @copyright Copyright (c) 2017-2018 Marcus Meeßen
/// @copyright Copyright (c) 2018      MASKOR Institute FH Aachen

#ifndef CUTE_VR_MACROS
#define CUTE_VR_MACROS

/// @brief Packs arguments containing commas, such as templates, so that they can be passed as a single argument to
/// another macro.
#define ARG(...) __VA_ARGS__

/// @brief Returns only the second argument when Doxygen runs. Intended to hide large code blocks of type traits and
/// SFINAE from the documentation.
#ifdef DOXYRUN
#define DOXYLIE(hide, show) show
#else
#define DOXYLIE(hide, show) hide
#endif

// FIXME: **URGENT** Is it even allowed to copy original Qt code (LGPL) to here (3-Clause BSD)?
// TODO: Copying a macro is a massive hack and not our style. However, there is currently no official solution for wrapper classes such as CuteVR::Extension::Optional.
// TODO: If the macro stops working in a later Qt version, corresponding legacy code must be used here.
/// @brief This macro is a copy of `Q_DECLARE_METATYPE_TEMPLATE_1ARG`, but removes the requirement that a single
/// argument template class must also be a sequential container. It allows to use a template class as a meta-type
/// without the need to call `Q_DECLARE_METATYPE` for each specialization.
/// @copyright Copyright (C) 2016 The Qt Company Ltd.
/// @copyright Copyright (C) 2014 Olivier Goffart <ogoffart@woboq.com>
/// @copyright Contact: https://www.qt.io/licensing/
/// @copyright Using the GNU Lesser General Public License
#define Q_DECLARE_METATYPE_TEMPLATE_1ARG_CUTE(SINGLE_ARG_TEMPLATE) \
QT_BEGIN_NAMESPACE \
template <typename T> \
struct QMetaTypeId< SINGLE_ARG_TEMPLATE<T> > \
{ \
    enum { \
        Defined = QMetaTypeId2<T>::Defined \
    }; \
    static int qt_metatype_id() \
    { \
        static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
        if (const int id = metatype_id.load()) \
            return id; \
        const char *tName = QMetaType::typeName(qMetaTypeId<T>()); \
        Q_ASSERT(tName); \
        const int tNameLen = int(qstrlen(tName)); \
        QByteArray typeName; \
        typeName.reserve(int(sizeof(#SINGLE_ARG_TEMPLATE)) + 1 + tNameLen + 1 + 1); \
        typeName.append(#SINGLE_ARG_TEMPLATE, int(sizeof(#SINGLE_ARG_TEMPLATE)) - 1) \
            .append('<').append(tName, tNameLen); \
        if (typeName.endsWith('>')) \
            typeName.append(' '); \
        typeName.append('>'); \
        const int newId = qRegisterNormalizedMetaType< SINGLE_ARG_TEMPLATE<T> >( \
                        typeName, \
                        reinterpret_cast< SINGLE_ARG_TEMPLATE<T> *>(quintptr(-1))); \
        metatype_id.storeRelease(newId); \
        return newId; \
    } \
}; \
QT_END_NAMESPACE

// TODO: If the macro stops working in a later Qt version, corresponding legacy code must be used here.
/// @brief This macro simply calls `Q_DECLARE_METATYPE_TEMPLATE_2ARG`, an undocumented macro from Qt that allows to use
/// a template class as a meta-type without the need to call `Q_DECLARE_METATYPE` for each specialization.
#define Q_DECLARE_METATYPE_TEMPLATE_2ARG_CUTE(arg) Q_DECLARE_METATYPE_TEMPLATE_2ARG(arg)

#endif // CUTE_VR_MACROS
