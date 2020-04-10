#ifndef OLD_SYS_TYPES_H
#define OLD_SYS_TYPES_H

#include <qglobal.h>
typedef signed char int8;
typedef qint16 int16;
typedef qint32 int32;

typedef unsigned char uint8;
typedef quint16 uint16;
typedef quint32 uint32;

#ifndef __int16
#define __int16 qint16
#endif
typedef qint16 _int16;


#endif // OLD_SYS_TYPES_H
