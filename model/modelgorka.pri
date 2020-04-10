
INCLUDEPATH +=$$QTROOTMVP \
    $$QTROOTMVP/common \
    $$QTROOTMVP/signalmanager \
    $$QTROOTMVP/el_base/model \
    $$QTROOTMVP/el_gmc/model \
    $$QTROOTMVP/el_gorka/model

HEADERS += \
    $$QTROOTMVP/libshared_global.h \
    $$QTROOTMVP/common/mvp_system.h \
    $$QTROOTMVP/common/basemodelgroup.h \
    $$QTROOTMVP/common/baseobject.h \
    $$QTROOTMVP/common/baseobjecttools.h \
    $$QTROOTMVP/common/groupbaseobjects.h \
    $$QTROOTMVP/common/mvp_objectfactory.h \
    $$QTROOTMVP/common/listobjstr.h \
    $$QTROOTMVP/common/objectid.h \
    $$QTROOTMVP/common/objectlink.h \
    $$QTROOTMVP/common/qobject2xml.h \
    $$QTROOTMVP/common/station.h \
    $$QTROOTMVP/common/formulaobjts.h \
    $$QTROOTMVP/signalmanager/gmc_tu.h \
    $$QTROOTMVP/signalmanager/archivemanager.h \
    $$QTROOTMVP/signalmanager/dmpacket2.h \
    $$QTROOTMVP/signalmanager/iarchive.h \
    $$QTROOTMVP/signalmanager/q_gtnet.h \
    $$QTROOTMVP/signalmanager/signalchanel.h \
    $$QTROOTMVP/signalmanager/signalsmanager.h \
    $$QTROOTMVP/signalmanager/tuts.h \
    $$QTROOTMVP/signalmanager/dbk_description.h \
    $$QTROOTMVP/signalmanager/gmc_dbk.h \
    $$QTROOTMVP/signalmanager/signaldescription.h \
    $$QTROOTMVP/el_base/model/m_base.h \
    $$QTROOTMVP/el_base/model/m_rc.h \
    $$QTROOTMVP/el_base/model/m_strel.h \
    $$QTROOTMVP/el_base/model/m_marsh.h \
    $$QTROOTMVP/el_base/model/m_marsh_list.h \
    $$QTROOTMVP/el_base/model/m_marsh_str.h \
    $$QTROOTMVP/el_base/model/m_svet.h \
    $$QTROOTMVP/el_gmc/model/m_rc_dso.h \
    $$QTROOTMVP/el_gmc/model/m_dso.h \
    $$QTROOTMVP/el_gmc/model/m_dso2.h \
    $$QTROOTMVP/el_gmc/model/m_strel_dso_dbk.h \
    $$QTROOTMVP/el_gmc/model/m_rc_zkr_dso.h \
    $$QTROOTMVP/el_gorka/model/m_gor_svet_4.h \
    $$QTROOTMVP/el_gorka/model/modelgroupgorka.h \
    $$QTROOTMVP/el_gorka/model/m_rc_gor.h \
    $$QTROOTMVP/el_gorka/model/m_strel_gor.h \
    $$QTROOTMVP/el_gorka/model/m_strel_gor_y.h \
    $$QTROOTMVP/el_gorka/model/m_rtds.h \
    $$QTROOTMVP/el_gorka/model/m_ipd.h \
    $$QTROOTMVP/el_gorka/model/m_zkr.h \
    $$QTROOTMVP/el_gorka/model/m_kzp.h \
    $$QTROOTMVP/el_gorka/model/m_rc_gor_park.h \
    $$QTROOTMVP/el_gorka/model/m_controllerars.h \
    $$QTROOTMVP/el_gorka/model/m_zam.h \
    $$QTROOTMVP/el_gmc/model/m_svet_gmc.h \
    $$QTROOTMVP/el_gmc/model/m_strel_gor_gmc.h


SOURCES += \
    $$QTROOTMVP/common/baseobjecttools.cpp \
    $$QTROOTMVP/common/mvp_system.cpp \
    $$QTROOTMVP/common/groupbaseobjects.cpp \
    $$QTROOTMVP/common/mvp_objectfactory.cpp \
    $$QTROOTMVP/common/qobject2xml.cpp \
    $$QTROOTMVP/common/station.cpp \
    $$QTROOTMVP/common/formulaobjts.cpp \
    $$QTROOTMVP/signalmanager/gmc_tu.cpp \
    $$QTROOTMVP/signalmanager/archivemanager.cpp \
    $$QTROOTMVP/signalmanager/dmpacket2.cpp \
    $$QTROOTMVP/signalmanager/q_gtnet.cpp \
    $$QTROOTMVP/signalmanager/signalchanel.cpp \
    $$QTROOTMVP/signalmanager/signalsmanager.cpp \
    $$QTROOTMVP/signalmanager/dbk_description.cpp \
    $$QTROOTMVP/signalmanager/signaldescription.cpp \
    $$QTROOTMVP/el_base/model/m_base.cpp \
    $$QTROOTMVP/el_base/model/m_rc.cpp \
    $$QTROOTMVP/el_base/model/m_strel.cpp \
    $$QTROOTMVP/el_base/model/m_marsh.cpp \
    $$QTROOTMVP/el_base/model/m_marsh_list.cpp \
    $$QTROOTMVP/el_base/model/m_marsh_str.cpp \
    $$QTROOTMVP/el_base/model/m_svet.cpp \
    $$QTROOTMVP/el_gmc/model/m_rc_dso.cpp \
    $$QTROOTMVP/el_gmc/model/m_dso.cpp \
    $$QTROOTMVP/el_gmc/model/m_dso2.cpp \
    $$QTROOTMVP/el_gmc/model/m_strel_dso_dbk.cpp \
    $$QTROOTMVP/el_gmc/model/m_rc_zkr_dso.cpp \
    $$QTROOTMVP/el_gorka/model/m_gor_svet_4.cpp \
    $$QTROOTMVP/el_gorka/model/modelgroupgorka.cpp \
    $$QTROOTMVP/el_gorka/model/m_rtds.cpp \
    $$QTROOTMVP/el_gorka/model/m_ipd.cpp \
    $$QTROOTMVP/el_gorka/model/m_rc_gor.cpp \
    $$QTROOTMVP/el_gorka/model/m_strel_gor.cpp \
    $$QTROOTMVP/el_gorka/model/m_strel_gor_y.cpp \
    $$QTROOTMVP/el_gorka/model/m_controllerars.cpp \
    $$QTROOTMVP/el_gorka/model/m_zam.cpp \
    $$QTROOTMVP/el_gorka/model/m_kzp.cpp \
    $$QTROOTMVP/el_gorka/model/m_rc_gor_park.cpp \
    $$QTROOTMVP/el_gorka/model/m_zkr.cpp \
    $$QTROOTMVP/el_gmc/model/m_svet_gmc.cpp \
    $$QTROOTMVP/el_gmc/model/m_strel_gor_gmc.cpp

