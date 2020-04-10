#ifndef EXTERNFLAGS_H
#define EXTERNFLAGS_H

#include <QObject>
#include <QtGlobal>
#include <QVector>

#include "signaldescription.h"



enum Tmk_flags{mk_flag_none=0,mk_flag_ok=1,mk_flag_warn=2,mk_flag_error=3};
#pragma pack(push, 1)
struct t_mk_flag{
    qint8 src;             // источник
    qint16 ev_id;          // кто поставил
    qint8 type;
    qint8 rang;
    quint64 id;
    t_mk_flag(){src=0;ev_id=0;type=0;rang=0;id=0;}
};
struct t_flag_buffer{
    qint8 buffer_number;
    qint8 buffer_count;
    qint16 flags_count_in_buffer;
    t_mk_flag flags[];
    t_flag_buffer(){buffer_number=0;buffer_count=0;flags_count_in_buffer=0;}
};

enum {maxflags=(476-4)/sizeof(t_mk_flag)};

#pragma pack(pop)


class ExternFlags : public QObject
{
    Q_OBJECT
public:
    explicit ExternFlags(QObject *parent = nullptr);

     static bool getFlag(QString &name,quint8 src,quint64 id,t_mk_flag &mk_flag);
     static QDateTime timeChanged(QString &name);
     static QVector<t_mk_flag> getFlags(QString &name,quint8 src, quint64 id);
     static QVector<GtBuffer *> getBuffers(QString &name);


signals:

public slots:
protected:
};

#endif // EXTERNFLAGS_H
