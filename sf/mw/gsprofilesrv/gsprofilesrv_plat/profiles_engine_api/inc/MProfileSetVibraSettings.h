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
* Description:  MProfileSetVibraSettings interface.
*
*/


#ifndef __MPROFILESETVIBRASETTINGS_H
#define __MPROFILESETVIBRASETTINGS_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile set vibra settings interface.
*  This interface offers methods to set vibra settings
*
*  @lib N/A
*  @since 10.1
*/
class MProfileSetVibraSettings
{
protected:  // Destructor
    
    virtual ~MProfileSetVibraSettings() {};
    
public:
    
    /**
    * set alert vibra settings
    * @since 10.1
    * @param aAlertVibra vibra settings
    */
    virtual void SetAlertVibra(TInt aAlertVibra) = 0;
    
    
    /**
    * set ring alert vibra
    * @since 10.1
    * @param aRingAlertVibra ETrue: On EFalse: Off
    */
    virtual void SetRingAlertVibra(TBool aRingAlertVibra) = 0;
    
    /**
    * set message alert vibra
    * @since 10.1
    * @param aMessageAlertVibra ETrue: On EFalse: Off
    */    
    virtual void SetMessageAlertVibra(TBool aMessageAlertVibra) = 0;
    
    /**
    * set Email alert vibra
    * @since 10.1
    * @param aEmailAlertVibra ETrue: On EFalse: Off
    */
    virtual void SetEmailAlertVibra(TBool aEmailAlertVibra) = 0;
    
    /**
    * set reminder alert vibra
    * @since 10.1
    * @param aReminderAlarmVibra ETrue: On EFalse: Off
    */
    virtual void SetReminderAlarmVibra(TBool aReminderAlarmVibra) = 0;
    
    /**
    * set information vibra
    * @since 10.1
    * @param aInformationVibra ETrue: On EFalse: Off
    */
    virtual void SetInformationVibra(TBool aInformationVibra) = 0;
};

#endif

//End of File
