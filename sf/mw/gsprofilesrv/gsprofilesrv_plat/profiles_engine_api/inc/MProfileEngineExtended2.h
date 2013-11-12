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
* Description:  Extended profile engine interface 2.
*
*/



#ifndef __MPROFILEENGINEEXTENDED2_H__
#define __MPROFILEENGINEEXTENDED2_H__

//  INCLUDES
#include <MProfileEngineExtended.h>

// DATA TYPES

// FUNCTION PROTOTYPES
class MProfileEngineExtended2;
/**
* Create extended profile engine, ownership transfered.
*
*  @lib ProfileEng.lib
*  @since 10.1
*/

IMPORT_C MProfileEngineExtended2* CreateProfileEngineExtended2L();

/**
* Create extended profile engine, ownership transfered.
*
*  @param aFs An open file server session, ownership is NOT transferred.
*  aFs must remain connected for the lifetime of the returned object.
*  @lib ProfileEng.lib
*  @since 10.1
*/
IMPORT_C MProfileEngineExtended2* CreateProfileEngineExtended2L( RFs* aFs );

// FORWARD DECLARATIONS
class MProfileExtended2;

// CLASS DECLARATION

/**
*  Extended profile engine interface.
*  This interface extend MProfileEngine interface and
*  offers methods to get profile settings and store profile settings.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileEngineExtended2 : public MProfileEngineExtended
    {
    protected:  // Destructor

        virtual ~MProfileEngineExtended2() {};

    public: // New functions

        /**
        * Create extended profile, ownership transfered.
        * @since 2.0
        * @param aId Profile ID
        * @return Instance of the MProfileExtended
        */
        virtual MProfileExtended2* Profile2LC( TInt aId ) = 0;

        /**
        * Create extended profile, ownership transfered.
        * @since 2.0
        * @param aId Profile ID
        * @return Instance of the MProfileExtended
        */
        virtual MProfileExtended2* Profile2L( TInt aId ) = 0;

        /**
        * Store profile settings. Leaves with KErrAccessDenied if one or more
        * of the settings of the target profile is/are read-only.
        * @since 2.0
        * @param aProfile Profile
        */
        virtual void CommitChange2L( MProfileExtended2& aProfile ) = 0;

       
        /**
        * Returns Master volume
        * @since 10.1
        */
        virtual TInt MasterVolumeL() const = 0;
        
        /**
        * Set master volume
        * @since 10.1
        * @param aMasterVolume: new master volume to be set
        */
        virtual void SetMasterVolumeL( TInt aMasterVolume ) = 0;
        
        /**
        * Returns Master vibra
        * @since 10.1
        */
        virtual TBool MasterVibraL() const = 0;
        
        /**
        * Set master vibra
        * @since 10.1
        * @param aMasterVibra: new master vibra to be set ETrue: On EFalse: Off
        */
        virtual void SetMasterVibraL( TBool aMasterVibra ) = 0;
        
        /**
        * Returns silence mode
        * @since 10.1
        */
        virtual TBool SilenceModeL() const = 0;
        
        /**
        * Set silence mode
        * @since 10.1
        * @param aSilenceMode: new silence mode to be set ETrue: On EFalse: Off
        */
        virtual void SetSilenceModeL( TBool aSilenceMode ) = 0;

    };

#endif      // __MPROFILEENGINEEXTENDED_H__

// End of File
