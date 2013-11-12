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
* Description:  Extended profile interface.
*
*/

#ifndef __MPROFILEVIBRASETTINGS_H
#define __MPROFILEVIBRASETTINGS_H


// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile vibra settings interface.
*  This interface offers methods to get vibra settings
*
*  @lib N/A
*  @since 10.1
*/
class MProfileVibraSettings
{
protected:  // Destructor
    
    virtual ~MProfileVibraSettings() {};
    
public:
    
    /**
    * Returns alert vibra settings
    * @since 10.1
    * @return Returns vibra settings
    */
    virtual TInt AlertVibra() const = 0;
    
    /**
    * Returns ring alert vibra
    * @since 10.1
    * @return Returns ring alert vibra
    */
    virtual TBool RingAlertVibra() const = 0;
    
    /**
    * Returns message alert vibra
    * @since 10.1
    * @return Returns message alert vibra
    */    
    virtual TBool MessageAlertVibra() const = 0;
    
    /**
    * Returns Email alert vibra
    * @since 10.1
    * @return Returns Email alert vibra
    */
    virtual TBool EmailAlertVibra() const = 0;
    
    /**
    * Returns reminder alert vibra
    * @since 10.1
    * @return Returns reminder alert vibra
    */
    virtual TBool ReminderAlarmVibra() const = 0;
    
    /**
    * Returns information vibra
    * @since 10.1
    * @return Returns information alert vibra
    */
    virtual TBool InformationVibra() const = 0;
};

#endif  // __MPROFILEVIBRASETTINGS_H

//End of File
