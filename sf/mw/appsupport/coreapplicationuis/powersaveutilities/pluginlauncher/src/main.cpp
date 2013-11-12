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

#include <hbapplication.h>
#include <QDir>
#include <hbmainwindow.h>
#include <cpbasepath.h>
#include "mainview.h"
#include <e32debug.h>

int main(int argc, char **argv)
{
    RDebug::Print( _L("plugin launcher main begin  ") ); 
    HbApplication app(argc, argv);
        
    HbMainWindow mainWindow;
    MainView *mainView = new MainView();
    mainWindow.addView(mainView);
    mainWindow.show();
    RDebug::Print( _L("plugin launcher main end") ); 
    return app.exec();
}

//End of File

