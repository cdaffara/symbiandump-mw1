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
* Description:  Profile 3D tone settings interface.
*
*/



#ifndef MPROFILE3DTONESETTINGS_H
#define MPROFILE3DTONESETTINGS_H

//  INCLUDES
#include <ProfileInternal.hrh>

// CLASS DECLARATION

/**
*  Profile 3D tone settings interface.
*  @since 3.1
*/
class MProfile3DToneSettings
    {
    protected:  // Destructor

        virtual ~MProfile3DToneSettings() {};

    public: // New functions

        /**
        * Returns PTT ringing tone file name.
        * @since 3.1
        * @return Returns PTT ringing tone file name.
        */
        virtual TProfile3DToneEffect Effect() const = 0;

        /**
         * Returns PTT status of the profile.
         * @since 3.1
         * @return Returns PTT status of the profile.
         */
        virtual TProfile3DToneEcho Echo() const = 0;

    };

#endif      //  MPROFILE3DTONESETTINGS_H

// End of File

