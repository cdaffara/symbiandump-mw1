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
* Description:  Declaration of CSAnimPlugin class
*
*/


#ifndef SANIMPLUGIN_H
#define SANIMPLUGIN_H

#include <e32base.h>

class RFs;

/**
*  Base class for all start-up animation plug-in interface classes
*
*  @lib None
*  @since S60 3.2
*/
class CSAnimPlugin : public CBase
    {

protected:

    /**
    * Constructor.
    *
    * @since S60 3.2
    */
    CSAnimPlugin();

public:

    /**
    * Set the repeat count value for the plug-in.
    *
    * @since S60 3.2
    *
    * @param aRepeatCount How many times to repeat playing the plug-in content.
    * Zero indicates infinite.
    */
    IMPORT_C void SetRepeatCount( const TInt aRepeatCount );

    /**
    * Load a file.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aFileName Name of the file to load.
    * @param aStatus Request to complete when the contents of the file have
    * been loaded.
    */
    virtual void Load(
        RFs& aFs,
        const TDesC& aFileName,
        TRequestStatus& aStatus ) = 0;

    /**
    * Start the plug-in activity (playing a tone or an animation or showing an
    * image).
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when activity has finished.
    */
    virtual void Start( TRequestStatus& aStatus ) = 0;

    /**
    * Cancel activity.
    *
    * @since S60 3.2
    */
    virtual void Cancel() = 0;

protected:

    /**
    * Complete client request with the given status code.
    *
    * @since S60 3.2
    *
    * @param aStatusCode KErrNone or one of Symbian error codes.
    */
    IMPORT_C void CompleteClientRequest( const TInt aStatusCode );

    /**
    * Set request pending and store request status to member variable.
    *
    * @since S60 3.2
    *
    * @param aStatus Request status from client.
    */
    IMPORT_C void SetClientRequest( TRequestStatus& aStatus );

private:

    /** Copy constructor. */
    CSAnimPlugin( const CSAnimPlugin& );
    /** Assignment operator. */
    CSAnimPlugin& operator=( const CSAnimPlugin& );

protected: // data

    /** To be used to inform client about request completion. May be NULL. Not owned. */
    TRequestStatus* iClientStatus;

    /**
    * Indicates how many times to repeat playing the plug-in content.
    * Zero indicates infinite.
    */
    TInt iRepeatCount;

    TAny* iReserved1; /** Reserved for future use. */
    TAny* iReserved2; /** Reserved for future use. */
    TInt iReserved3; /** Reserved for future use. */
    TInt iReserved4; /** Reserved for future use. */

    };

#endif // SANIMPLUGIN_H
