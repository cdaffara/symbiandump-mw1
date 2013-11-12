/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: Process launched by the accessory indicator plugin to 
 * show the view and user can change the settings. 
 *
 */

#include <hbapplication.h>

#include "headsetttyview.h"
#include "tvoutview.h"

int main(int argc, char *argv[])
    {
    HbApplication app(argc, argv);
    int retVal;
    // Get the params from the process.
    QStringList args(app.arguments());
    int accessoryMode = QString(args.at(1)).toInt(); // HeadSet,TTY,TV-OUT
    int accessoryType = QString(args.at(2)).toULong();// Wired or Wireless
    
    HbMainWindow *window = new HbMainWindow();
    
    // If accessory mode is Tv-Out load tvout.docml or else load headset.docml
    QObject *view;
    if( accessoryMode == EAccModeTVOut )
        {
        view = new TvOutView(window,accessoryType);
        }
    else
        {
        view = new HeadsetTtyView(window,accessoryType);
        }
    
    window->show();
    retVal = app.exec();
    delete window;
    delete view;
    return retVal;
    }
