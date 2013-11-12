/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef TSTASKSETTINGS_H
#define TSTASKSETTINGS_H

#include <QPixmap>

#include "tstaskmonitor_global.h"

class TsTaskSettingsPrivate;

class TSTASKMONITOR_EXPORT TsTaskSettings
{
    
public:
    TsTaskSettings();
    ~TsTaskSettings();
    
    bool registerScreenshot(const QPixmap &screenshot);
    bool unregisterScreenshot();
    bool setVisibility(bool);
    
private:
    TsTaskSettingsPrivate *d_ptr;   
};

#endif //TSTASKSETTINGS_H
