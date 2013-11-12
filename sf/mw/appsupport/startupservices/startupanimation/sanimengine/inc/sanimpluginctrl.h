/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CSAnimPluginCtrl class
*
*/


#ifndef SANIMPLUGINCTRL_H
#define SANIMPLUGINCTRL_H

#include <e32base.h>

class CSAnimPlugin;

/**
*  Active object in control of one CSAnimPlugin object.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimPluginCtrl ) : public CActive
    {

public:

    /**
    * Constructs a CSAnimPluginCtrl object.
    *
    * @since S60 3.2
    *
    * @param aFileName Name of the file to load with the plug-in.
    * @param aIfUid Identifies the interface the plug-in must implement.
    * @param Construction parameters for ECom.
    * @return The new object
    */
    static CSAnimPluginCtrl* NewL(
        const TDesC& aFileName,
        const TUint32 aIfUid,
        TAny* aConstructionParameters );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimPluginCtrl();

    /**
    * Load the file with the plug-in.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aCallBack Identifies a call-back function to call when loading is
    * complete.
    */
    void Load( RFs& aFs, const TCallBack& aCallBack );

    /**
    * Start the plug-in.
    *
    * @since S60 3.2
    *
    * @param aCallBack Identifies a call-back function to call when the plug-in
    * has finished.
    */
    void Start( const TCallBack& aCallBack );

    /**
    * Return a success code of the last activity (Load/Start).
    *
    * @since S60 3.2
    *
    * @return KErrNone or one of the Symbian error codes.
    */
    TInt SuccessCode() const;

    /**
    * Get a reference to the plug-in object.
    *
    * @since S60 3.2
    *
    * @return A reference to the plug-in object.
    */
    CSAnimPlugin& Plugin();

protected:

    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2
    */
    virtual void DoCancel();

    /**
    * From CActive.
    * Handles an active object's request completion event.
    * Never leaves.
    *
    * @since S60 3.2
    */
    virtual void RunL();

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    */
    CSAnimPluginCtrl();

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    *
    * @param aFileName Name of the file to load with the plug-in.
    * @param aIfUid Identifies the interface the plug-in must implement.
    * @param Construction parameters for ECom.
    */
    void ConstructL(
        const TDesC& aFileName,
        const TUint32 aIfUid,
        TAny* aConstructionParameters );

    /**
    * Extract the extension part from the file name, and return it.
    *
    * @since S60 3.2
    *
    * @param aFileName File name.
    * @param aExtension On return, will contain the extension part of the file
    * name.
    */
    void GetExtension( const TDesC& aFileName, TDes& aExtension );

    /**
    * Find a plug-in which supports the given file extension and load it.
    *
    * @since S60 3.2
    *
    * @param aExtension The file extension which the plug-in must support.
    * @param aIfUid Identifies the interface the plug-in must implement.
    * @param Construction parameters for ECom.
    */
    void FindPluginByExtensionL(
        const TDesC& aExtension,
        const TUint32 aIfUid,
        TAny* aConstructionParameters );

private:

    /** Copy constructor. */
    CSAnimPluginCtrl( const CSAnimPluginCtrl& );
    /** Assignment operator. */
    CSAnimPluginCtrl& operator=( const CSAnimPluginCtrl& );

private: // data

    /** Identifies the file to load with the plug-in. Owned. May not be NULL. */
    HBufC* iFileName;

    /** The plug-in object to control. Owned. May not be NULL. */
    CSAnimPlugin* iPlugin;

    /** Plug-in identifier in ECom. */
    TUid iDtorIDKey;

    /** Call-back function to call when action completes. */
    TCallBack iCallBack;

    /** Identifies the success/failure of the last activity. */
    TInt iSuccessCode;

    /** Updated to ETrue after Load has successfully completed. */
    TBool iReadyToStart;

    };

#endif // SANIMPLUGINCTRL_H
