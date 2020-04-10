#include "externflags.h"

QString defaultExtBufferName="extflag_";
int defaultExtBufferType=32;

QString externFlagsBufferName(QString &name,int i)
{
    if (name.isEmpty()) return QString("%1%2").arg(defaultExtBufferName).arg(i);
    return QString("%1%2").arg(name).arg(i);
}

ExternFlags::ExternFlags(QObject *parent) : QObject(parent)
{
}

bool ExternFlags::getFlag(QString &name, quint8 src, quint64 id, t_mk_flag &mk_flag)
{
    bool ex=false;
    QVector<GtBuffer *> l=getBuffers(name);
    foreach (GtBuffer *B, l) {
        const t_flag_buffer * flag_buffer=(t_flag_buffer *)B->A.data();
        for (int i=0;i<flag_buffer->flags_count_in_buffer;i++){
            const t_mk_flag &bmk=flag_buffer->flags[i];
            if (((src==0)||(src==bmk.src)) && (id==bmk.id))
            {
                if (!ex) {
                    mk_flag=bmk;
                    ex=true;
                } else {
                    if ((bmk.type>mk_flag.type) || ((bmk.type==mk_flag.type) && (bmk.rang>mk_flag.rang))) mk_flag=bmk;
                }
            }
        }
    }
    return ex;
}

QDateTime ExternFlags::timeChanged(QString &name)
{
    QDateTime t=QDateTime();
    QVector<GtBuffer *> l=getBuffers(name);
    foreach (GtBuffer *B, l) {
        if ((!t.isValid())|| (t<B->dataChangedTime())) t=B->dataChangedTime();
    }
    return t;
}

QVector<t_mk_flag> ExternFlags::getFlags(QString &name,quint8 src, quint64 id)
{
    QVector<t_mk_flag> v;
    QVector<GtBuffer *> l=getBuffers(name);
    foreach (GtBuffer *B, l) {
        const t_flag_buffer * flag_buffer=(t_flag_buffer *)B->A.data();
        for (int i=0;i<flag_buffer->flags_count_in_buffer;i++){
            const t_mk_flag &bmk=flag_buffer->flags[i];
            if (((src==0)||(src==bmk.src)) &&
                    (id==bmk.id))
            {
                bool ex=false;
                for (int iv=0;iv<v.size();iv++){
                    const t_mk_flag &vmk=v[iv];
                    if ((bmk.type>vmk.type) || ((bmk.type==vmk.type) && (bmk.rang>vmk.rang)))
                    {
                        v.insert(iv,bmk);
                        ex=true;break;
                    }
                }
                if (!ex) v.push_back(bmk);
            }
        }
    }
    return v;
}

QVector<GtBuffer *> ExternFlags::getBuffers(QString &name)
{
    QVector<GtBuffer *> l;
    QString bufferName=externFlagsBufferName(name,1);
    GtBuffer *B=SignalDescription::getBuffer(defaultExtBufferType,bufferName);
    if (B==nullptr) return l;
    if (B->A.size()<4) return l;
    const t_flag_buffer * flag_buffer=(t_flag_buffer *)B->A.data();
    if ((int)(4+flag_buffer->flags_count_in_buffer*sizeof(t_mk_flag))>B->A.size()) return l;
    int buffer_count=flag_buffer->buffer_count;
    if ((flag_buffer->buffer_number!=1) || (buffer_count==0)) return l;
    for (int ib=1;ib<=buffer_count;ib++){
        B=SignalDescription::getBuffer(defaultExtBufferType,bufferName);
        flag_buffer=(t_flag_buffer *)B->A.data();
        if ((flag_buffer->buffer_number!=ib) ||
                (buffer_count=flag_buffer->buffer_count) ||
                ((int)(4+flag_buffer->flags_count_in_buffer*sizeof(t_mk_flag))>B->A.size()) ||
                (B->sost!=GtBuffer::_alive)) {
            l.clear();
            return l;
        }
        l.push_back(B);
    }
    return l;
}
