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
* Description:  Profiles engine constants
*
*/



#ifndef PROFILEENGINECONSTANTS_H
#define PROFILEENGINECONSTANTS_H

//  INCLUDES
#include <e32std.h>
#include "ProfileEng.hrh"

// CONSTANTS
const TInt KProfileFileEstimationSize( 400 );
const TInt KProfilesMaxNumberOfDynamicProfiles( 20 );
const TInt KProfilesMaxTIntLength( 11 );
const TInt KProfileMaxNameLength( PROFILES_MAX_NAME_LENGTH );
const TInt KProfileMaxCallObjectTextLength( 1000 );
const TInt KProfileBaseDynamicProfileId( 30 );    // first dynamic profile ID
// Used for profile settings cenrep-keys - profile id is encoded in the MSB:
const TInt KProfileIdShift( 24 );    

_LIT( KProfileDriveC,           "c:" );
_LIT( KProfileDriveZ,           "z:" );
_LIT( KProfileFilePrefix,       "Profile" );
_LIT( KProfileFileExtension,    ".dat" );
_LIT( KProfileFilePath,         "\\System\\Data\\Profiles\\" );
_LIT( KProfileEngineResourceDriveAndDir, "Z:ProfileEng.rsc" );
_LIT( KProfileMutexName,        "ProfileFileAccessMutexName" );

#endif      // PROFILEENGINECONSTANTS_H

// End of File
