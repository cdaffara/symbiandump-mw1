# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not,
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
# Description:
TEMPLATE = lib
TARGET = bookmarksapi
ROOT_DIR = $$PWD/..
include($$ROOT_DIR/browser.pri)
DEFINES += BUILDING_BOOKMARKSAPI_DLL
QT += core \
    network \
    xml \
    sql \
    script

# HEADERS += $$PWD/inc/browsercontentdllclientdefs.h \
# $$PWD/inc/browsercontentdll.h
# SOURCES += $$PWD/src/browsercontentdll.cpp
isEmpty(BEDROCK_OUTPUT_DIR): { 
    symbian { 
        CONFIG(release, debug|release):BOOKMARKSAPI_OUTPUT_DIR = $$PWD/../../WrtBuild/Release
        CONFIG(debug, debug|release):BOOKMARKSAPI_OUTPUT_DIR = $$PWD/../../WrtBuild/Debug
    }
    else { 
        CONFIG(release, debug|release):BOOKMARKSAPI_OUTPUT_DIR = $$PWD/../../../../WrtBuild/Release
        CONFIG(debug, debug|release):BOOKMARKSAPI_OUTPUT_DIR = $$PWD/../../../../WrtBuild/Debug
    }
}
else:BOOKMARKSAPI_OUTPUT_DIR = $$BEDROCK_OUTPUT_DIR
OBJECTS_DIR = $$BOOKMARKSAPI_OUTPUT_DIR/bookmarksapi/tmp
DESTDIR = $$BOOKMARKSAPI_OUTPUT_DIR/bin
MOC_DIR = $$BOOKMARKSAPI_OUTPUT_DIR/bookmarksapi/tmp
RCC_DIR = $$BOOKMARKSAPI_OUTPUT_DIR/bookmarksapi/tmp
TEMPDIR = $$BOOKMARKSAPI_OUTPUT_DIR/bookmarksapi/build

# QMAKE_LIBDIR = $$BOOKMARKSCLIENTDLL_DATAMODEL_OUTPUT_DIR/bin
INCLUDEPATH += $$PWD

# I believe the following line to be useless on all platforms. (carol.szabo@nokia.com)
# !s40:LIBS += -Llib
# CONFIG += \
# building-libs \
# depend_includepath \
# dll
CONFIG += dll

# CONFIG(release, debug|release):!CONFIG(QTDIR_build){
# !unix : contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols
# unix : contains(QT_CONFIG, reduce_relocations): CONFIG += bsymbolic_functions
# }
# CONFIG -= warn_on
*-g++*:QMAKE_CXXFLAGS += -Wreturn-type \
    -fno-strict-aliasing
CONFIG(gcov) { 
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -fprofile-arcs \
        -ftest-coverage
    message( "building for coverage statics" )
}
isEmpty(TEMPDIR) { 
    CONFIG(release, debug|release):TEMPDIR = $$DESTDIR/Release/build
    CONFIG(debug, debug|release):TEMPDIR = $$DESTDIR/Debug/build
}
CONFIG (maemo):include(../../../../cwrt-maemo.pri)
symbian: { 
    TARGET.UID3 = 0x200267E7
    TARGET.VID = VID_DEFAULT
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = All \
        -TCB \
        -DRM \
        -AllFiles
    LIBS += -lefsrv \
        -lcaf \
        -lcafutils
    INCLUDEPATH += $$MW_LAYER_PUBLIC_EXPORT_PATH(cwrt) \
        $$MW_LAYER_PUBLIC_EXPORT_PATH()
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += /epoc32/include
    bookmarksapi.sources = bookmarksapi.dll
    bookmarksapi.path = /sys/bin
    DEPLOYMENT += bookmarksapi
}
HEADERS = \
#BookmarkResultsList.h \
    TagResults.h \
    BookmarkResults.h \
    BookmarkFav.h \
    xbelreader.h \
    xbelwriter.h \
    BookmarksManager.h \
    bookmarksapi.h \
    Bookmark.h
SOURCES = \
#BookmarkResultsList.cpp \
    TagResults.cpp \
    BookmarkResults.cpp \
    BookmarkFav.cpp \
    xbelreader.cpp \
    xbelwriter.cpp \
    BookmarksManager.cpp
