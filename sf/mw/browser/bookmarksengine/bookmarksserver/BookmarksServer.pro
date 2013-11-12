#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, 
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:
#

TEMPLATE = app
TARGET = BookMarkServer 

# establish relative root dir
ROOT_DIR = $$PWD/../..

include($$ROOT_DIR/browser.pri)

QT += core network xml sql 
QT -= gui

HEADERS += $$PWD/inc/bookmarkserver.h \
           $$PWD/../commoninclude/bookmarkscommonengine.h
SOURCES += $$PWD/src/servermain.cpp \
           $$PWD/src/bookmarkserver.cpp 


isEmpty(BEDROCK_OUTPUT_DIR): {
    symbian {
    	CONFIG(release,release|debug):BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../WrtBuild/Release
    	CONFIG(debug,release|debug):BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../WrtBuild/Debug

    else {
    	CONFIG(release,release|debug):BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../../../../WrtBuild/Release
    	CONFIG(debug,release|debug):BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR=$$PWD/../../../../../WrtBuild/Debug
    }    
} else {
    BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR=$$BEDROCK_OUTPUT_DIR
}

QMAKE_RPATHDIR += $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/bin
QMAKE_LIBDIR += $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/bin


INCLUDEPATH += $$PWD/inc $$PWD/../commoninclude $$PWD/../../ipc/serviceipc $$PWD/../../ipc/serviceipcserver

# Use IPC components from import directory.
include($$PWD/../../../../../import/import.pri)
LIBS += -lwrtserviceipcserver

CONFIG(gcov)   {
   LIBS +=   -lgcov
   QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
   message( "building for coverage statics" )
}

     
OBJECTS_DIR = $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServer/tmp
DESTDIR = $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/bin
MOC_DIR = $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServer/tmp
RCC_DIR = $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServer/tmp
TEMPDIR = $$BOOKMARK_DATAMODEL_SERVER_OUTPUT_DIR/BookMarkServer/build




CONFIG (maemo) {
    include(../../../../cwrt-maemo.pri)
} 

symbian: {
    TARGET.EPOCHEAPSIZE = 0x20000 0x1000000 // Min 128kb, Max 16Mb
    TARGET.UID3 = 0x200267E7
    TARGET.VID = VID_DEFAULT
    TARGET.EPOCALLOWDLLDATA=1
    TARGET.CAPABILITY = All -TCB -DRM -AllFiles
    CONFIG +=no_icon
}

symbian: {
    INCLUDEPATH +=  $$PWD $$MW_LAYER_SYSTEMINCLUDE $$APP_LAYER_SYSTEMINCLUDE
}


symbian:MMP_RULES += SMPSAFE
