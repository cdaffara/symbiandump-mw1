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
####################################################################################
#
# Browser - common settings - browser.pri
#
#   any edits made here that are common to app and mw should also be made 
#   in the app\browserui\browserui.pri file
#
####################################################################################

isEmpty(_BROWSER_PRI_INCLUDED_): {
    
    _BROWSER_PRI_INCLUDED_=1

    symbian {
        CONFIG += debug_and_release
        MMP_RULES += EXPORTUNFROZEN
    }
   
    CONFIG(debug, debug|release) {
        SUBDIRPART = Debug
    } else {
        SUBDIRPART = Release
    }
    
    # Figure out the root of where stuff should go (this could be done via configure)
symbian {
    OUTPUT_DIR = $$PWD
} else {
    OUTPUT_DIR = $$PWD/../../..
}
    OBJECTS_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    MOC_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    RCC_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    UI_DIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/tmp
    TEMPDIR= $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/$$TARGET/build
    DESTDIR = $$OUTPUT_DIR/WrtBuild/$$SUBDIRPART/bin
    QMAKE_RPATHDIR = $$DESTDIR $$QMAKE_RPATHDIR
    QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR
    
    # Add the output dirs to the link path too
    LIBS += -L$$DESTDIR
    
    DEPENDPATH += .
    INCLUDEPATH += .

    symbian {
        BROWSER_INCLUDE = $$MW_LAYER_PUBLIC_EXPORT_PATH(browser)
    } else {
        BROWSER_INCLUDE = $$PWD/include/
    }
    
    QMAKE_CXXFLAGS_DEBUG += -g
    
    CONFIG(gcov)   {
       LIBS +=   -lgcov
       QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    }
}

