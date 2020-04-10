#ifndef LIBSHARED_GLOBAL_H
#define LIBSHARED_GLOBAL_H

#include <QtCore/qglobal.h>
#  define LIBSHARED_EXPORT
#if defined(EXPORT_LIBRARY)
#  define LIBSHARED_EXPORT Q_DECL_EXPORT
#endif
#if defined(IMPORT_LIBRARY)
#  define LIBSHARED_EXPORT Q_DECL_IMPORT
#endif
#if defined(NO_LIBRARY)
#  define LIBSHARED_EXPORT
#endif

#endif // LIBSHARED_GLOBAL_H
