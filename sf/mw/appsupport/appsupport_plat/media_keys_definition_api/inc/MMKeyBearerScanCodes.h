/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Declaration of Scan codes .
*       
*
*/


#ifndef __MMKEYBEARERSCANCODE_H__
#define __MMKEYBEARERSCANCODE_H__

//CONSTANTS

// Side volume Key Codes
const TInt KPSVolumeDownReleased = -3;  
const TInt KPSVolumeDownPressed  = -2;
const TInt KPSVolumeDownClicked  = -1;
const TInt KPSVolumeNoKey        = 0;
const TInt KPSVolumeUpClicked    = 1;
const TInt KPSVolumeUpPressed    = 2;
const TInt KPSVolumeUpReleased   = 3;

//ENUMS

// Media Key codes
enum TMediaKeyValues
{
    EPlayReleased =0,
    EPlayPressed,    
    EPlayClicked,
    EStopReleased,
    EStopPressed,
    EStopClicked,
    EForwardReleased,
    EForwardPressed,
    EForwardClicked,
    ERewindReleased,
    ERewindPressed,
    ERewindClicked,
    EFastForwardReleased,
    EFastForwardPressed,
    EFastForwardClicked,
    EBackwardReleased,
    EBackwardPressed,
    EBackwardClicked
};

#endif //__MMKEYBEARERSCANCODE_H__

//End of File
