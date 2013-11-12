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
* Description:  This class implements MProEngTones interface.
*
*/



#ifndef CPROENGTONESIMPL_H
#define CPROENGTONESIMPL_H

// INCLUDES
#include <MProEngTones.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MProfileTones;
class MProfileSetTones;
class MProfileExtraTones;
class MProfileSetExtraTones;
class CProEngToneHandler;

// CLASS DECLARATION

/**
* This class implements MProEngTones interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngTonesImpl ) : public CBase,
                                        public MProEngTones
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngTonesImpl* NewL(
            const MProfileTones& aProfileTones,
            MProfileSetTones& aProfileSetTones,
            const MProfileExtraTones& aProfileExtraTones,
            MProfileSetExtraTones& aProfileSetExtraTones,
            TUint32 aModifiableFlags );

        /**
         * Destructor.
         */
        virtual ~CProEngTonesImpl();

    public:

        /**
         * From MProEngTones
         */
        const TDesC& RingingTone1() const;

        /**
         * From MProEngTones
         */
        const TDesC& RingingTone2() const;

        /**
         * From MProEngTones
         */
        const TDesC& MessageAlertTone() const;

        /**
         * From MProEngTones
         */
        const TDesC& EmailAlertTone() const;

        /**
         * From MProEngTones
         */
        const TDesC& VideoCallRingingTone() const;

        /**
         * From MProEngTones
         */
        TInt SetRingingTone1L( const TDesC& aRingingTone );

        /**
         * From MProEngTones
         */
        TInt SetRingingTone2L( const TDesC& aRingingTone );

        /**
         * From MProEngTones
         */
        TInt SetMessageAlertToneL( const TDesC& aMessageAlertTone );

        /**
         * From MProEngTones
         */
        TInt SetEmailAlertToneL( const TDesC& aEmailAlertTone );

        /**
         * From MProEngTones
         */
        TInt SetVideoCallRingingToneL( const TDesC& aRingingTone );

    public:  // New functions

        /**
         * Checks the tones that have been modified by client, so that the
         * values in them are legal. This method is called from CommitChangeL()
         * of CProEngProfileImpl.
         * @since 3.1
         * @param aToneHandler Tone handler needed for checking the legality
         *        of the tones.
         * @return KErrNone if all the tones set are correct,
         *         KErrNotFound if a tone file cannot be found,
         *         KErrNotSupported if a file is of an unsupported type or
         *         KErrPermissionDenied if the file is not allowed
         *         to be used as a ringing or alert tone.
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected audio/MP4 and
         *		   those are not allowed as alert tones.
         */
        TInt CheckTonesL( CProEngToneHandler& aToneHandler );

        /**
         * Handles removing of the old tones from the automated content list and
         * adding of the new tones to the automated content list. This method is
         * called from CommitChangeL() of CProEngProfileImpl after the changes
         * has been committed in the underlying Profiles Engine.
         * @since 3.1
         * @param aToneHandler Tone handler needed for making the actual auto-
         *        mated content handling for the tones.
         *        of the tones.
         * @param aOldTones contains the tones that are currently set as alert
         *        tones for the same profile.
         * @return KErrNone if succesful, otherwise a system wide error code.
         */
        TInt HandleAutomatedContent( CProEngToneHandler& aToneHandler,
                                      const CProEngTonesImpl& aOldTones );

    private:

        /**
         * Replaces KNullDesC with the path to the empty tone.
         * @since 3.1
         * @param aTone The tone that is converted if it is KNullDesC.
         * @return The path to the empty tone if aTone is KNullDesC, otherwise
         *         the original tone.
         */
        const TDesC& ConvertKNullToEmptyTone( const TDesC& aTone ) const;

    private:

        /**
         * C++ constructor.
         */
        CProEngTonesImpl( const MProfileTones& aProfileTones,
                          MProfileSetTones& aProfileSetTones,
                          const MProfileExtraTones& aProfileExtraTones,
                          MProfileSetExtraTones& aProfileSetExtraTones,
                          TUint32 aModifiableFlags );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Ref: the interface to the implementation of the profile tones
        const MProfileTones& iProfileTones;

        // Ref: the interface to the implementation of the profile set tones
        MProfileSetTones& iProfileSetTones;

        // Ref: the interface to the implementation of the profile extra tones
        const MProfileExtraTones& iProfileExtraTones;

        // Ref: the interface to the implementation of the set extra tones
        MProfileSetExtraTones& iProfileSetExtraTones;

        // Flags to check is modification allowed:
        TUint32 iModifiableFlags;

        // Flags that tell which of the tones have been modified by the client
        TUint32 iModifiedFlags;

        // Null tone.
        TBuf<1> iNullTone;

        // Own: Path to empty tone
        HBufC* iEmptyTone;

    };

#endif      //  CPROENGTONESIMPL_H

// End of File

