/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/
#include "mainview.h"
#include <hbmenu.h>
#include <hbaction.h>
#include <qcoreapplication.h>
#include <cppluginlauncher.h>
#include <QStringList>
#include <cpbasesettingview.h>
#include <hbmessagebox.h>
#include <e32debug.h>


MainView::MainView(QGraphicsItem *parent/* = 0*/)
: HbView(parent)
{
	RDebug::Print( _L("plugin launcher MainView begin  ") ); 
	init();
	RDebug::Print( _L("plugin launcher MainView end  ") ); 
}

MainView::~MainView()
{
RDebug::Print( _L("plugin launcher MainView desructor  ") ); 
}

void MainView::init()
{
	RDebug::Print( _L("plugin launcher MainView init begin  ") ); 
    launchInProcessProfileView();
  RDebug::Print( _L("plugin launcher MainView init end  ") ) ;   
}

void MainView::launchInProcessProfileView()
{
   RDebug::Print( _L("plugin launcher MainView launchInProcessProfileView begin  ") );  
    //static function which launches the psmplugin view
   CpPluginLauncher::launchSettingView("C:/resource/qt/plugins/controlpanel/cppsmplugin.dll","psm_view");
   RDebug::Print( _L("plugin launcher MainView launchInProcessProfileView end  "));  
}



//End of File
