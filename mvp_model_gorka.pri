MVP_DIR_LIST+= \
                $$PWD  \
                $$PWD/common \
                $$PWD/model \
                $$PWD/model/el_base \
                $$PWD/model/el_gorka \
                $$PWD/model/el_gorka/gorka_headers
                


for(dir, $$list($$MVP_DIR_LIST) ) {
          INCLUDEPATH+= $$dir
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          HEADERS+= $$files( $$dir/*.h)
}
for(dir, $$list($$MVP_DIR_LIST) ) {
          SOURCES+= $$files( $$dir/*.cpp)
}
