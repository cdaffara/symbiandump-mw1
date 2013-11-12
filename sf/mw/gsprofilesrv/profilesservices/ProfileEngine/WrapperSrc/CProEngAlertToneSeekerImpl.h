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
* Description:  This class implements MProEngAlertToneSeeker interface.
*
*/



#ifndef CPROENGALERTTONESEEKERIMPL_H
#define CPROENGALERTTONESEEKERIMPL_H

// INCLUDES
#include <e32base.h>
#include <MProEngAlertToneSeeker.h>
#include <MCLFOperationObserver.h>
#include    <badesca.h>  // CDesCArray

// FORWARD DECLARATIONS
class MCLFItemListModel;
class MCLFContentListingEngine;
class CProEngPostFilter;
class RFs;
class TParse;

// CLASS DECLARATION

/**
* This class implements MProEngAlertToneSeeker interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngAlertToneSeekerImpl ) : public CBase,
                                                  public MProEngAlertToneSeeker,
                                                  public MCLFOperationObserver
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngAlertToneSeekerImpl* NewL();

        /**
         * Destructor.
         */
         virtual ~CProEngAlertToneSeekerImpl();

    public:  // Functions from base classes

        /**
         * From MProEngAlertToneSeeker
         */
        void FetchAlertToneListL( MProEngAlertToneSeekerObserver& aObserver );

        /**
         * From MProEngAlertToneSeeker
         */
        void CancelFetch();

        /**
         * From MCLFOperationObserver
         */
        void HandleOperationEventL( TCLFOperationEvent aOperationEvent,
                                    TInt aError );

    private:  // New functions

        /**
         * Creates the list model to fetch files through CLF.
         */
        void CreateListModelL();

        /**
         * Read the files in ROM and save them in iRomFiles.
         */
        void CreateRomFileListL();

        /**
         * Helper method for CreateRomFileListL() to handle reading of the
         * contents of one directory and adding the files found in ROM file
         * array.
         */
        void AppendRomFilesL( RFs& aFs, const TDesC& aPath, TParse& aParse );
                                                         
    private:

        /**
         * C++ default constructor.
         */
        CProEngAlertToneSeekerImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Own: Array holding the tones/videos in ROM
        CDesCArray* iRomFiles;

        // Own: Model for files not in ROM
        MCLFItemListModel* iModel;

        // Own: Content listing engine
        MCLFContentListingEngine* iContentListingEngine;

        // Ref: client-implemented observer
        MProEngAlertToneSeekerObserver* iObserver;

        // Own: Post filter for the list model
        CProEngPostFilter* iPostFilter;
    };

#endif      //  CPROENGALERTTONESEEKERIMPL_H

// End of File

