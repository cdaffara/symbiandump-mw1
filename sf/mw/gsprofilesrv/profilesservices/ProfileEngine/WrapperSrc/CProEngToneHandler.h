/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A helper class for making correctness and DRM checkups to the
*                tones to be used as alert tones.
*
*/



#ifndef CPROENGTONEHANDLER_H
#define CPROENGTONEHANDLER_H

//  INCLUDES
#include <f32file.h> // CBase, RFs
#include <apmstd.h> // KMaxDataTypeLength
#include <data_caging_path_literals.hrh>
#include <ProfileEng.hrh> // TProfileSettingId

// FORWARD DECLARATIONS
class DRMCommon;
class CDRMHelper;
class CProEngMediaVariation;

// CLASS DECLARATION

/**
*  A helper class for making correctness and DRM checkups to the
*  tones to be used as alert tones.
*  Registers DRM protected tones as automated content
*  and removes old content from automated content list.
*  Shows a content activation query and other DRM related
*  notes if necessary.

*
*  @lib ProfileEngine.lib
*  @since Series 60 3.1
*/
NONSHARABLE_CLASS( CProEngToneHandler ) : public CBase
    {
    public:  // Constructors and destructor

        // Two-phased constructors.
        static CProEngToneHandler* NewL();
        static CProEngToneHandler* NewLC();

        // Destructor.
        ~CProEngToneHandler();

    public: // New functions

        /**
         * Checks that the given tone file exists and that it is
         * a legal file to be used as a ringing or alert tone.
         * @since 3.1
         * @param aFileName Full path and filename of the file to be checked.
         * @param aToneType Type of tone to check, see: ProfileEng.hrh
         * @return KErrNone if the file can be used as a ringing or alert tone,
         *         KErrNotFound if the file cannot be found,
         *         KErrNotSupported if a file is of an unsupported type or
         *         KErrPermissionDenied if the file is not allowed
         *         to be used as a ringing or alert tone.
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
		 *         KErrTooBig if file size exceeded file size limit.
         */
        TInt CheckToneFileL( const TDesC& aFileName, TInt aToneType );

        /**
         * Adds the file to automated content list and removes the old file
         * from automated content list.
         * Called by CProEngProfileImpl::CommitChangeL after the changes has
         * been committed in the underlying Profiles Engine.
         * @since 3.1
         * @param aSettingId setting id which identifies which tone (ringing
         *                   tone, sms tone etc.) this automated content
         *                   handling operation concerns.
         * @param aFileName Full path and filename of the tone to be used
         *                  as the new ringing or alert tone.
         * @param aOldFileName Full path and filename of the previous tone used
         *                  as the ringing or alert tone.
         * @return KErrNone, if succesful, otherwise a system-wide error code.
         */
        TInt HandleAutomatedContent( TProfileSettingId aSettingId,
                                     const TDesC& aFileName,
                                     const TDesC& aOldFileName);

    protected:  // New functions

        /**
         * Helper function for CheckToneFileL. This function does most of the
         * actual checking.
         * @since 3.1
         * @param aFileName Full path and filename of the file to be checked.
         * @param aToneType Type of tone to check, see: ProfileEng.hrh
         * @return KErrNone if the file can be used as a ringing or alert tone,
         *         KErrNotFound if the file cannot be found,
         *         KErrNotSupported if a file is of an unsupported type or
         *         KErrPermissionDenied if the file is not allowed
         *         to be used as a ringing or alert tone.
         *         KErrGeneral if DRM rights are insufficient.
		 *		   KErrArgument if the file is DRM protected
		 *		   audio/mp4 and those are not allowed as ringingtones.
		 *         KErrTooBig if file size exceeded file size limit.
         */
        TInt DoCheckToneFileL( const TDesC& aFileName, TInt aToneType );

        /**
         * Checks if a DRM protected file can be used as a ringing or alert tone.
         * @since 3.1
         * @param aFileName Full path and filename of the file to be checked.
         * @return KErrNone if the file can be used as a ringing or alert tone or
         *         KErrGeneral if DRM rights are insufficient.
         */
        TInt CheckProtectedFileL( const TDesC& aFileName );

        /**
         * Tells whether a file can be registered as automated content or not.
         * If DRM is not supported, returns always EFalse.
         * @since 3.1
         * @param aFileName File to be registered as automated content.
         * @return Returns ETrue if aFileName can be set as automated content.
         *         Returns EFalse if can't or DRM is not supported.
         */
        TBool CanSetAutomated( const TDesC& aFileName );

        /**
         * Asks user if he/she wants to activate unactivated content.
         * Shows a query. Does nothing if DRM is not supported.
         * @since 3.1
         * @param aFileName File to be registered as automated content.
         * @return Returns KErrCancel if user cancels the query.
         */
        TInt AskAutomated( const TDesC& aFileName );

        /**
         * Registers a file to automated content list without any queries.
         * Errors are omitted. Does nothing if DRM is not supported.
         * @since 3.1
         * @param aSettingId setting id which identifies which tone (ringing
         *                   tone, sms tone etc.) this automated content
         *                   setting operation concerns.
         * @param aFileName File to be registered as automated content.
         * @return KErrNone, if succesful, otherwise a system-wide error code.
         */
        TInt SetAutomated( TProfileSettingId aSettingId,
                           const TDesC& aFileName );

        /**
         * Removes a file from automated content list.
         * Errors are omitted. Does nothing if DRM is not supported.
         * @since 3.1
         * @param aFileName File to be removed from automated content list.
         */
        void RemoveAutomated( const TDesC& aFileName );

        /**
         * Returns whether a file is protected content or not.
         * Returns always EFalse if DRM is not supported.
         * @since 3.1
         * @param aFileName The file name.
         * @return Returns whether the file is protected content or not.
         */
        TBool IsProtected( const TDesC& aFileName ) const;

        /**
         * Returns whether a file is protected content or not.
         * Returns always EFalse if DRM is not supported.
         * @since 3.1
         * @param aFileName The file name.
         * @return Returns whether the file is protected content or not.
         */
        TBool IsProtectedL( const TDesC& aFileName ) const;

		/**
         * Displays an error note.
         * @since 3.1
         * @param aResourceId Resource ID to a TBUF resource.
         */
        void ShowErrorNoteL( TInt aResourceId );

        /**
         * Checks if tone file's size is larger than the size limit.
         * @since 3.1
         * @param aFileName The path to the file.
         * @return KErrNone   if the file can be used as a ringing tone.
         *         KErrTooBig if the file size limit is exceeded.
         */
        TInt CheckFileSizeLimit( const TDesC& aFileName ) const;
        /**
         * Checks if tone file's size is larger than the size limit.
         * @since 3.1
         * @return KErrNone   if the file can be used as a ringing tone.
         *         KErrTooBig if the file size limit is exceeded.
         */
        TInt CheckToneFileSize( const TDesC& aFile, TInt aSizeLimitKB ) const;
        /**
         * Reads tone file size limit from cenrep.
         * @since 3.1
         * @return leaves on error
         */
        void GetMaxToneFileSizeL( TInt& aMaxSizeKB ) const;

    private:

        /// Default constructor
        CProEngToneHandler();

        /// 2nd phase constructor
        void ConstructL();

        /// Constructor for the DRM objects
        void DrmConstructL();

        /// Destructor for the DRM objects
        void ReleaseDrm();

        /// Called by SetAutomated
        TInt SetAutomatedL( TProfileSettingId aSettingId,
                           const TDesC& aFileName );

    private:    // Data

        /// Owns: DRM Helper client.
        CDRMHelper* iDrmHelper;

        /// Owns: DRM Common client
        DRMCommon* iDrmCommon;

        /// File Server client.
        RFs iFs;

        /// Mime-type handling:
        CProEngMediaVariation* iMediaVariation;

        /// voice call file size limit
        TInt iToneFileSizeLimitKB;
    };

#endif      // CPROENGTONEHANDLER_H

// End of File
