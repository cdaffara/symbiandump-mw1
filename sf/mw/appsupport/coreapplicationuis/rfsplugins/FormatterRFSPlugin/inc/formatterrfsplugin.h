/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CFormatterRFSPlugin class
*
*/


#ifndef C_FORMATTERRFSPLUGIN_H
#define C_FORMATTERRFSPLUGIN_H

//  INCLUDES
#include "rfsPlugin.h"

// CLASS DECLARATION
class CFormatterRFSPlugin: public CRFSPlugin
    {

public:

    /**
    * Two-phased constructor.
    *
    * @param aInitParams initial parameters
    * @return The new object
    */
    static CFormatterRFSPlugin* NewL( TAny* aInitParams );

    /**
    * Destructor.
    */
    virtual ~CFormatterRFSPlugin();

public: // From CRFSPlugin

    /**
    * @see CRFSPlugin.
    */
    void RestoreFactorySettingsL( const TRfsReason aType );

    /**
    * @see CRFSPlugin.
    */
    void GetScriptL( const TRfsReason aType, TDes& aPath );

    /**
    * @see CRFSPlugin.
    */
    void ExecuteCustomCommandL( const TRfsReason aType, TDesC& aCommand );

private:

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Start secure formatter process with suitable parameters and wait for it
    * to finish.
    */
    void DoSecureFormatL() const;

    /**
    * Create directory with given name to the system drive.
    */
    void CreateDirectory( const TDesC& aDirName );
    
    /**
     * Performs format on the internal mass storage drive
     */
    TInt PerformFormat(const TDesC& aDriveLetter);

private: // Data

    /** File server session. */
    RFs iFs;

    /** Identifies the drive to restore. */
    TChar iSystemDrive;

    /** Identifies the internal mass storage drive to format. */
    TChar iMassStorageDrive;
    
    /** Identifies the ROM drive. */
    TChar iDefaultRomDrive;

    /** Name of the exclude list file. NULL is exclude list does not exist. Own. */
    HBufC* iExcludeListName;
    
    /** TRUE if at leat one application exclude list is found */
    TBool iAppExcludeList;
    
    /** variables needed  for parsing internal mass storage drives from drive list */
    TDriveList iDriveListInt;
    TInt iDriveCountInt;   
    };

#endif // FORMATTERRFSPLUGIN_H
