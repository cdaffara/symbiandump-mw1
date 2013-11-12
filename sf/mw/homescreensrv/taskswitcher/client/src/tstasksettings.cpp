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
#include "tstasksettings.h"
#include "tstasksettings_p.h"

/*!
    @class TsTaskSettings
    @ingroup publicApi
    @brief TsTaskSettings allows application to alter the way it will be 
    handled by TsTaskMonitor.
    
    This class is one of the Qt interfaces for Task Monitor server. It can be
    used to set custom screenshot of the task, or to change the task visibility.
*/

/*!
    Constructor.
*/
TsTaskSettings::TsTaskSettings() : d_ptr(new TsTaskSettingsPrivate())
{
}

/*!
    Destructor.
*/
TsTaskSettings::~TsTaskSettings()
{
    delete d_ptr;
}

/*!
    Overwrites default screenshot of task with \a screenshot. If \a 
    screenshot is null, this method will return false.
    @return True if the screenshot is successfully saved, false otherwise.
*/
bool TsTaskSettings::registerScreenshot(const QPixmap &screenshot)
{
    return d_ptr->registerScreenshot(screenshot);
}

/*!
    Removes custom screenshot saved with registerScreenshot(). If there was no
    saved screenshot, this method will return false.
    @return True if the screenshot is successfully removed, false otherwise.
*/
bool TsTaskSettings::unregisterScreenshot()
{
    return d_ptr->unregisterScreenshot();
}

/*!
    Changes the \a visibilty of task. Hidden tasks are not present on the task
    list returned by TsTaskMonitor::taskList().
    @return True if the visibility is changed successfully, false otherwise.
*/
bool TsTaskSettings::setVisibility(bool visibility)
{
    return d_ptr->setVisibility(visibility);
}
