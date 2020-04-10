

MVP_DIR_LIST+= \
                $$MVP_SYB_ROOT  \
                $$MVP_SYB_ROOT/common \
                $$MVP_SYB_ROOT/signalmanager \
                $$MVP_SYB_ROOT/model \
                $$MVP_SYB_ROOT/model/el_base \
                $$MVP_SYB_ROOT/model/el_gmc \
                $$MVP_SYB_ROOT/model/el_gorka \
                $$MVP_SYB_ROOT/model/el_gorka/gorka_headers \
                $$MVP_SYB_ROOT/model/el_diag \
                $$MVP_SYB_ROOT/model/el_lpdiag \


for(dir, $$list($$MVP_DIR_LIST) ) {
          INCLUDEPATH+= $$dir
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          HEADERS+= $$files( $$dir/*.h)
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          SOURCES+= $$files( $$dir/*.cpp)
}


QT+=concurrent


