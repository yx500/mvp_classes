

MVP_DIR_LIST+= \
                $$PWD  \
                $$PWD/common \
                $$PWD/signalmanager \
                $$PWD/model \
                $$PWD/model/el_base \
                $$PWD/model/el_gmc \
                $$PWD/model/el_gorka \
                $$PWD/model/el_gorka/gorka_headers \
                $$PWD/model/el_diag \
                $$PWD/model/el_lpdiag \
                $$PWD/visual \
                $$PWD/visual/el_base \
                $$PWD/visual/el_gmc \
                $$PWD/visual/el_gorka \
                $$PWD/visual/el_diag \
                $$PWD/visual/el_lpdiag \
                $$PWD/visual/el_visual_simple


for(dir, $$list($$MVP_DIR_LIST) ) {
          INCLUDEPATH+= $$dir
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          HEADERS+= $$files( $$dir/*.h)
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          SOURCES+= $$files( $$dir/*.cpp)
}


QT+=svg

