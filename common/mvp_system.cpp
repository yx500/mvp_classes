#include "mvp_system.h"

#include "qobject2xml.h"
#include "baseobject.h"
#include "signaldescription.h"

MVP_System MVP;

MVP_System::MVP_System() : QObject(nullptr)
{

}

MVP_System::~MVP_System()
{
}

QObject *MVP_System::loadObject(QString fileName,QStringList *errorLog)
{
    if (mFileName2QObject.contains(fileName)){
        return mFileName2QObject[fileName];
    }
    QObject2XML XML;
    QObject *O=XML.readFile(fileName);
    if (O){
        mFileName2QObject[fileName]=O;
        mQObject2FileName[O]=fileName;
        BaseObject *B=qobject_cast<BaseObject *>(O);
        if (B) B->updateAfterLoad();
    }
    if (errorLog!=nullptr) *errorLog=XML.errorLog;
    return O;
}

bool MVP_System::isObjectLoaded(QString fileName) const
{
    return mFileName2QObject.contains(fileName);
}

QString MVP_System::fileName4Object(QObject *O) const
{
    if (mQObject2FileName.contains(O)) return mQObject2FileName[O];
    return QString();
}

IGtBufferInterface *MVP_System::setGetGtBufferInterface(IGtBufferInterface *i)
{
    return setDefaultGetGtBufferInterface(i);
}


