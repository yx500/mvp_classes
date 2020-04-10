#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T08:28:13
#
#-------------------------------------------------

QT       += core gui network widgets

TEMPLATE = lib

# Версия библиотеки.
VERSION = 1.0.0

DESTDIR = $$PWD/../libs

# Если операционная система из семейства unix
unix: {
                CONFIG (debug, debug|release) {
                        # Такое название имеет debug-версия библиотеки
                        TARGET = mvpd
                } else {
                        # А такое release-версия
                        TARGET = mvp
                }
} else {
                TARGET = $$qtLibraryTarget(mvp)
}
        # Первый параметр необходим для сборки #библиотеки в linux (qmake, make all),
        # второй для сборки под остальными ОС.
#CONFIG += debug_and_release build_all
        # Указываем папки для объектных файлов. Для unix-подобных ОС это критично.
        # Если этого не сделать, то будет собираться только release версия библиотеки,
        # либо только отладочная. Связано это с тем, что файлы будут замещать друг друга.
#CONFIG (debug, debug|release) {
#                OBJECTS_DIR = build/debug
#        } else {
#                OBJECTS_DIR = build/release
#        }


DEFINES +=EXPORT_LIBRARY




MVP_SYB_ROOT=../mvp
include($$MVP_SYB_ROOT/mvp_all.pri)


unix {
    target.path = /usr/lib
    INSTALLS += target
}




















