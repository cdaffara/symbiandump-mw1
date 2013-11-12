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
* Description: 
*  
*  This interface provides a method for getting access point UIDs mapped from
*  default CommDb to current CommDb.
*
*
*/


#ifndef RFS_AP_MAPPER_H
#define RFS_AP_MAPPER_H

//  INCLUDES

#include <e32std.h>

// CLASS DECLARATION

/**
* Mixin class providing AP mapping from default CommDb access point UIDs
* to current CommDb access point UIDs.
*/
class MRfsApMapper
    { 
    public:     // new functions

        /**
        * Maps an AP UID that is valid in the default CommDb to a valid
        * AP UID in the currently used CommDb.
        *
        * @param aOldUid UID in the default CommDb
        * @param aNewUid corresponding UID in the current CommDb is stored here
        *
        * @return ETrue if AP was mapped, EFalse if not found.
        */
        virtual TBool MapAccessPoint( TUint aOldUid, TUint& aNewUid ) = 0;
    };

#endif      // RFS_AP_MAPPER_H
