/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  This class adds a setting view to main window, and restore previous view when back key clicked.
*
*/

#include "cpviewlauncher.h"
#include <hbinstance.h>
#include <hbview.h>
#include <hbmainwindow.h>
#include <QCoreApplication>

static HbMainWindow *mainWindow() 
{
    QList< HbMainWindow* > mainWindows = hbInstance->allMainWindows();
    if (!mainWindows.isEmpty()) {
        return mainWindows.front();
    }
    return 0;
}

void CpViewLauncher::launchView(HbView *view)
{
	if (view) {
		CpViewLauncher *launcher = new CpViewLauncher();
		launcher->internalLaunchView(view);
	}
}

CpViewLauncher::CpViewLauncher() :
    mView(0),
    mPreView(0)
{
}

CpViewLauncher::~CpViewLauncher()
{
}

void CpViewLauncher::internalLaunchView(HbView *view)
{
    //Q_ASSERT(view);   should not use
    
    HbMainWindow *mainWnd = ::mainWindow();
    
    if (mainWnd && view)
    {
        mView = view;
      
        mPreView = mainWnd->currentView();
        
        if (mPreView) {
            QObject::connect(mView, SIGNAL(aboutToClose()), this, SLOT(viewDone()));
        }
        else {
            QObject::connect(mView, SIGNAL(aboutToClose()), qApp, SLOT(quit()));
        }
  
       
        // activates the plugin view
        mainWnd->addView(mView);
        mainWnd->setCurrentView(mView);   
    }
}

void CpViewLauncher::viewDone()
{    
    HbMainWindow *mainWnd = ::mainWindow();
    
    if (mainWnd) {
        if (mView) {               
            //restore previous status
            mainWnd->removeView(mView);
        
            mView->deleteLater();
            mView = 0;
    
            mainWnd->setCurrentView(mPreView);
        }
    }

    deleteLater();
}

//End of File
