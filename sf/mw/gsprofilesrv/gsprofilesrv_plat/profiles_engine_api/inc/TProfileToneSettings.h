/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile tone settings structure.
*
*/



#ifndef __TPROFILETONESETTINGS_H
#define __TPROFILETONESETTINGS_H

// INCLUDES
#include <Profile.hrh>
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile tone settings structure.
*
*  @lib N/A
*  @since 2.0
*/
struct TProfileToneSettings
    {
    TProfileRingingType  iRingingType;
    TProfileKeypadVolume iKeypadVolume;
    TInt                 iRingingVolume;         // 1 - 10
    TBool                iVibratingAlert;        // ETrue = On
    TBool                iWarningAndGameTones;   // ETrue = On
    TInt                 iMediaVolume;           // 1 - 10
    TBool                iTextToSpeech;          // ETrue = On
    };

#endif      // __TPROFILETONESETTINGS_H

// End of File
