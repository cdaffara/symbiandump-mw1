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

#ifndef TVOUTVIEW_H
#define TVOUTVIEW_H

#include <hbmainwindow.h>
#include <QObject>
#include <GSServerEngine.h>

class TvOutView : public QObject
    {
    Q_OBJECT

public:
    TvOutView(HbMainWindow *window,int);
    ~TvOutView();

private slots:
    void currentIndexModified(int);
private:
	int currentValue;
	CGSServerEngine* iServerEngine;
	
public:
    };

#endif // TVOUTVIEW_H
