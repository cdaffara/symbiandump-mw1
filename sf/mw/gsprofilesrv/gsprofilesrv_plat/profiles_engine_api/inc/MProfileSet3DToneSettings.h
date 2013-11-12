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
* Description:  Profile set 3D tone settings interface.
*
*/



#ifndef MPROFILESET3DTONESETTINGS_H
#define MPROFILESET3DTONESETTINGS_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Profile set 3D tone settings interface.
*  @since 3.1
*/
class MProfileSet3DToneSettings
    {
    protected:  // Destructor

        virtual ~MProfileSet3DToneSettings() {};

    public: // New functions

        /**
        * Set 3D tone effect of the profile.
        * @since 3.1
        */
        virtual void SetEffect( TProfile3DToneEffect aEffect ) = 0;

        /**
         * Set 3D tone echo of the profile.
         * @since 3.1
         */
        virtual void SetEcho( TProfile3DToneEcho aEcho ) = 0;

    };

#endif      //  MPROFILESET3DTONESETTINGS_H

// End of File



