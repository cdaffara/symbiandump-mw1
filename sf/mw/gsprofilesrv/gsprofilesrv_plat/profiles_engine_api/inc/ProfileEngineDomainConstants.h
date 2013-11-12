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
* Description:  Profiles engine constants for S60 applications to use.
*
*/



#ifndef PROFILEENGINEDOMAINCONSTANTS_H
#define PROFILEENGINEDOMAINCONSTANTS_H

//  INCLUDES
#include <e32std.h>

// CONSTANTS
/**
 * The tone to be used when an "Off" tone is needed for an alert tone. With
 * this tone the vibra can be played even if no sound is heard. This file is
 * exported by Profiles Engine and it will always be located in ROM, so this
 * value must be appended to the ROM-drive letter, for example like this:
 * @code
 * TFileName* tf = new ( ELeave ) TFileName(
 *         TParsePtrC( PathInfo::RomRootPath() ).Drive() );
 * CleanupStack::PushL( tf );
 * tf->Append( KProfileNoSoundPath );
 * @endcode
 */
_LIT( KProfileNoSoundPath, "\\resource\\No_Sound.wav" );

#endif      // PROFILEENGINEDOMAINCONSTANTS_H

// End of File
