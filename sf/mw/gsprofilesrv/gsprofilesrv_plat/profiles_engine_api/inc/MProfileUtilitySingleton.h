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
* Description:  Profile Utility interface.
*
*/



#ifndef __MPROFILEUTILITYSINGLETON_H__
#define __MPROFILEUTILITYSINGLETON_H__

//  INCLUDES

// FORWARD DECLARATIONS
class MProfileUtilitySingleton;
class MProfilesLocalFeatures;

// FUNCTION PROTOTYPES
/**
* Return reference to MProfileUtilitySingleton instance. Create it if
* necessary. For each succesful InstanceL() call there must be a corres-
* ponding Release() call, so that the internal reference counting works.
* @since 3.2
* @return Reference to the only MProfileUtilitySingleton instance.
*/
IMPORT_C MProfileUtilitySingleton& ProfileUtilityInstanceL();

/**
* Release the resources of MProfileUtilitySingleton (when there are no
* references to it anymore). For each succesful InstanceL() call there
* must be a corresponding Release() call, so that the internal
* reference counting works.
* @since 3.2
*/
IMPORT_C void ReleaseProfileUtility();


// CLASS DECLARATION
/**
*  Profile utility class.
*
*  @lib profileeng.lib
*  @since 3.2
*/
class MProfileUtilitySingleton
    {
    protected:  // Destructor

        virtual ~MProfileUtilitySingleton() {};

    public: // New functions

        /**
        * Returns a reference to MProfilesLocalFeatures instance owned by
        * this MProfileUtility instance.
        * @since 3.2
        */
        virtual MProfilesLocalFeatures& ProfilesLocalFeatures() = 0;

    };

#endif      // __MPROFILEUTILITYSINGLETON_H__

// End of File

