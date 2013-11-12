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
* Description:  Interface for controlpanel plugins, with this interface client application can launch a view from outside of controlpanel application.
*
*/


#ifndef CPLAUNCHERINTERFACE_H
#define CPLAUNCHERINTERFACE_H

#include <QtPlugin>
#include <QList>

/*!
    \class CpLauncherInterface
    \brief The class CpLauncherInterface defines an interface for plugins, if one controlpanel plugin implements
    this interface, client application can launch a setting view from outside of controlpanel application by two ways.
    
    (1) launch setting view in client process, using CpPluginLauncher.
    
    \code
    CpBaseSettingView *settingView = CpPluginLauncher::launchSettingView("cpmyplugin.dll",QVariant());
    \endcode
    
    (2) launch setting view in embedded mode, using QtHighway client API.
    
    \code
    if (mRequest) {
        delete mRequest;
        mRequest = 0;
    }
    
    mRequest = mAppMgr.create("com.nokia.symbian.ICpPluginLauncher", "launchSettingView(QString,QVariant)", true);

    if (!mRequest)
    {
        return;
    }
    else
    {
        connect(mRequest, SIGNAL(requestOk(QVariant)), SLOT(handleReturnValue(QVariant)));
        connect(mRequest, SIGNAL(requestError(int,QString)), SLOT(handleError(int,QString)));
    }

    // Set arguments for request 
    QList<QVariant> args;
    args << QVariant("cpmyplugin.dll");
    args << QVariant();
    mRequest->setArguments(args);

    mRequest->setSynchronous(false);    
    // Make the request
    if (!mRequest->send())
    {
        //report error     
    }
    \endcode
    
 */

class QVariant;
class CpBaseSettingView;

class CpLauncherInterface
{
public:
    /*!
     Destructor of CpLauncherInterface.
     */
    virtual ~CpLauncherInterface()
    {
    }
    
    /*!
     Create a CpBaseSettingView by the parameter hint.
     */
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const = 0;
};

Q_DECLARE_INTERFACE(CpLauncherInterface, "com.nokia.controlpanel.launcher.interface/1.0");

#endif /* CPLAUNCHERINTERFACE_H */
