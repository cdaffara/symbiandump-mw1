/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorCommand class declaration.
 *
*/


#ifndef SENSORSOURCECOMMAND_H
#define SENSORSOURCECOMMAND_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION
class CCFContextSourceCommand;

/**
 * This class wraps up the sensor source command information.
 *
 * @lib None.
 * @since S60 5.0
 */
class CSensorSourceCommand : public CBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aCommand General context source command from which the sensor
     *  source command is constructed.
     * @return CSensorSourceCommand*
     */
    static CSensorSourceCommand* NewL( const CCFContextSourceCommand& aCommand );
    static CSensorSourceCommand* NewLC( const CCFContextSourceCommand& aCommand );

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aSid Sid of the client.
     * @param aScriptId Script ID where the command is from.
     * @param aFeatureId Feature ID where the command is used.
     * @param aCommandId The actual command ID.
     * @param aChannelId Sensor channel associated by the command.
     * @return CSensorSourceCommand*
     */
    static CSensorSourceCommand* NewL( const TSecureId& aSid,
        TInt aScriptId,
        const TDesC& aFeatureId,
        const TDesC& aCommandId,
        TUint32 aChannelId );
    static CSensorSourceCommand* NewLC( const TSecureId& aSid,
        TInt aScriptId,
        const TDesC& aFeatureId,
        const TDesC& aCommandId,
        TUint32 aChannelId );
    
    /**
     * C++ destructor.
     */
    virtual ~CSensorSourceCommand( );
    
public:

    /**
     * Get secure ID of the client.
     * @return TSecureId.
     */
    TSecureId SecureId() const;
    
    /**
     * Get script ID of the client.
     * @return TInt, KErrNotFound if the client is not a script.
     */
    TInt ScriptId() const;
    
    /**
     * Get feature ID of the client.
     * @return TPtrC.
     */
    TPtrC FeatureId() const;
    
    /**
     * Get command id.
     * @return TPtrC.
     */
    TPtrC CommandId() const;
    
    /**
     * Channel ID.
     * @return TUint32.
     */
    TUint32 ChannelId() const;
    
private:

    CSensorSourceCommand( );
    void ConstructL( const CCFContextSourceCommand& aCommand );
    void ConstructL( const TSecureId& aSid,
        TInt aScriptId,
        const TDesC& aFeatureId,
        const TDesC& aCommandId,
        TUint32 aChannelId );
    
private: // Data

    /** Script ID of the client */
    TInt iScriptId;

    /** Secure ID of the client */
    TSecureId iSid;
    
    /** Feature ID of the client */
    HBufC* iFeatureId;
    
    /** Sensor channel ID */
    TUint32 iChannelId;
    
    /** Command id */
    HBufC* iCommandId;
    };
    
#endif // SENSORSOURCECOMMAND_H

