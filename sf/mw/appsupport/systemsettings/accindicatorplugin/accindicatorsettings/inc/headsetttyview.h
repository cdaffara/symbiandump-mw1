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
 * Description:
 *
 */

#ifndef HEADSETTTYVIEW_H
#define HEADSETTTYVIEW_H

#include <hbmainwindow.h>
#include <QObject>
#include <centralrepository.h>
#include <AccessorySettings.h>
#include <AccessoryServer.h>

class CRepository;
class CGSServerEngine;

class HeadsetTtyView : public QObject
    {
    Q_OBJECT

public:
    HeadsetTtyView(HbMainWindow *window,int);
    ~HeadsetTtyView();

private slots:
    void currentIndexModified(int);
private:
	int currentValue;
	TInt defaultaccessory;
	
	// Handle to the Central Repository.
    CRepository*    iAccessoryRepository;

    /** Accessory server connection. */
    RAccessoryServer iAccServer;

    /** Accessory settings connection. */
    RAccessorySettings iAccessorySettings;

public:
    };

#endif // HEADSETTTYVIEW_H
