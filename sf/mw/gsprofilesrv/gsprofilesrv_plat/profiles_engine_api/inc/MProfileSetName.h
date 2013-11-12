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
* Description:  Set profile name interface.
*
*/



#ifndef __MPROFILESETNAME_H
#define __MPROFILESETNAME_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Set profile name interface.
*  This interface offers method to set profile name.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileSetName
    {
    protected:  // Destructor

        virtual ~MProfileSetName() {};

    public: // New functions

        /**
        * Set name of the profile.
        * @since 2.0
        * @param aName Name of the profile.
        */
        virtual void SetNameL( const TDesC& aName ) = 0;

    };

#endif      // __MPROFILESETNAME_H

// End of File
