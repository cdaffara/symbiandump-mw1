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
* Description:  Profiles Engine utils.
*
*/



#ifndef __PROFILEENGUTILS_H
#define __PROFILEENGUTILS_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class RFs;
class RWriteStream;
class CRepository;

// CLASS DECLARATION

/**
*  Profiles Engine utils.
*
*  @lib ?library
*  @since 2.0
*/
class ProfileEngUtils
    {
    public: // New functions

        /**
         * Reads alternate service setting value from RSSSettings.
         * @param aValue on return contains the value of the alternate line
         * service setting.
         */
        static void GetAlternateLineService( TInt& aValue );

        /**
         * Checks if the alternate line has been selected.
         * @return ETrue if als is supported and the alternate line has been
         * selected, EFalse otherwise.
         */
        static TBool IsAlternateLineSelected();

        /**
        * Resolves the Central Repository key for the given setting of the
        * given profile.
        * @since 2.8
        * @param aPartialKey is the part of the key which identifies the
        * setting in Central Repository (these keys are defined in Profiles-
        * PrivateCRKeys.h)
        * @param aProfileId is the id of the profile the setting of which
        * the caller is interested in
        * @return the real key to be used with Central Repository
        */
        static TUint32 ResolveKey( TUint aPartialKey, TInt aProfileId );

        /**
        * Gets the binary setting with the given id from Central Repository.
        * This can be used to only read settings that contain strings of
        * elements which length must be != 2 bytes.
        * @since 2.8
        * @param aCenRep reference to a repository containing the needed key
        * @param aId the key of the value to be fetched
        * @param aDes 8-bit buffer which contains the value read from Central
        * Repository on return. If the read value is 2 zero bytes, the
        * buffer will be empty on return.
        */
        static void GetBinarySettingL( CRepository& aCenRep, TUint32 aId,
                                       TDes8& aDes );
        /**
        * Converts 4 bytes from a binary buffer to TUint32.
        * @since 2.8
        * @param aResult is a reference to the integer which will contain the
        * value on return
        * @param aBuf is the buffer from which the 32 bits are read
        * @param aOffset is the offset from which the reading from aBuf should
        * start
        */
        static void GetUint32Presentation( TUint32& aResult,
                                           const TDesC8& aBuf,
                                           TInt aOffset );

        /**
         * Searches for given 8-bit value from a binary value stored in
         * Central Repository.
         * @since 3.0
         * @param aCenRep Central Repository containing the key
         * @param aCenRepKey the key from which the value should be searched
         * @param aValue the 8-bit value to be searched
         * @return the index of the 8-bit value if found, otherwise
         * KErrNotFound.
         */
        static TInt FindValueFromBinaryKeyL( CRepository& aCenRep,
                                             TUint32 aCenRepKey,
                                             TUint8 aValue );

        /**
         * Returns power save mode.
         * @return Power save mode.
         */
        static TBool PowerSaveModeL();

        /**
         * Returns power save mode.
         * @return Power save mode.
         */
        static TBool PowerSaveMode();
    };

#endif      // __PROFILEENGUTILS_H

// End of File
