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
* Description:  CSensorSourceCommand class implementation.
 *
*/


// SYSTEM INCLUDES
#include <cfcontextsourcecommand.h>

// USER INCLUDES
#include "sensorsourcecommand.h"
#include "sensortrace.h"
#include "sensorsourcecontextdef.h"
#include "sensorutil.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand::CSensorSourceCommand( )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CSensorSourceCommand::ConstructL( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;
    
    // Store secure id
    iSid = aCommand.Sender().iSender;
    
    // Store script id
    iScriptId = aCommand.Sender().iScriptId;

    // Check that we have parameters
    const RSourceCommandParameterArray& parameters = aCommand.Parameters();
    for( TInt i = 0; i < parameters.Count(); i++ )
        {
        CCFContextSourceCommandParameter* parameter = parameters[i];
        if( parameter->Name().CompareF( KSensorCommandName ) == KErrNone )
            {
            // Get attributes
            const RKeyValueArray& attributes = parameter->Attributes();
            for( TInt j = 0; j < attributes.Count(); j++ )
                {
                CCFKeyValuePair* attribute = attributes[j];
                // channelId
                if( attribute->Key().CompareF( KSensorChannelAttributeName ) == KErrNone )
                    {
                    TUint32 channelId = 0x0;
                    if( SensorUtil::ConvertToHex( attribute->Value(), channelId ) == KErrNone )
                        {
                        iChannelId = channelId;
                        }
                    }
                // id
                else if( attribute->Key().CompareF( KSensorCommandIdName ) == KErrNone )
                    {
                    iCommandId = attribute->Value().AllocL();
                    }
                // featureId
                else if( attribute->Key().CompareF( KSensorCommandFeatureIdName ) == KErrNone )
                    {
                    iFeatureId = attribute->Value().AllocL();
                    }
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CSensorSourceCommand::ConstructL( const TSecureId& aSid,
    TInt aScriptId,
    const TDesC& aFeatureId,
    const TDesC& aCommandId,
    TUint32 aChannelId )
    {
    FUNC_LOG;
    
    iSid = aSid;
    iScriptId = aScriptId;
    iFeatureId = aFeatureId.AllocL();
    iCommandId = aCommandId.AllocL();
    iChannelId = aChannelId;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand* CSensorSourceCommand::NewL(
    const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    CSensorSourceCommand* self = CSensorSourceCommand::NewLC( aCommand );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand* CSensorSourceCommand::NewL( const TSecureId& aSid,
    TInt aScriptId,
    const TDesC& aFeatureId,
    const TDesC& aCommandId,
    TUint32 aChannelId )
    {
    FUNC_LOG;
    
    CSensorSourceCommand* self = CSensorSourceCommand::NewLC(
        aSid, aScriptId, aFeatureId, aCommandId, aChannelId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand* CSensorSourceCommand::NewLC( const CCFContextSourceCommand& aCommand )
    {
    FUNC_LOG;

    CSensorSourceCommand* self = new( ELeave ) CSensorSourceCommand;
    CleanupStack::PushL( self );
    self->ConstructL( aCommand );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand* CSensorSourceCommand::NewLC( const TSecureId& aSid,
    TInt aScriptId,
    const TDesC& aFeatureId,
    const TDesC& aCommandId,
    TUint32 aChannelId )
    {
    FUNC_LOG;
    
    CSensorSourceCommand* self = new( ELeave ) CSensorSourceCommand;
    CleanupStack::PushL( self );
    self->ConstructL( aSid, aScriptId, aFeatureId, aCommandId, aChannelId );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CSensorSourceCommand::~CSensorSourceCommand( )
    {
    FUNC_LOG;
    
    delete iFeatureId;
    delete iCommandId;
    }

//------------------------------------------------------------------------------
// CSensorSourceCommand::SecureId
//------------------------------------------------------------------------------
//
TSecureId CSensorSourceCommand::SecureId() const
    {
    FUNC_LOG;
    
    return iSid;
    }

//------------------------------------------------------------------------------
// CSensorSourceCommand::ScriptId
//------------------------------------------------------------------------------
//
TInt CSensorSourceCommand::ScriptId() const
    {
    FUNC_LOG;
    
    return iScriptId;
    }

//------------------------------------------------------------------------------
// CSensorSourceCommand::FeatureId
//------------------------------------------------------------------------------
//
TPtrC CSensorSourceCommand::FeatureId() const
    {
    FUNC_LOG;
    
    TPtrC id( KNullDesC );
    if( iFeatureId )
        {
        id.Set( *iFeatureId );
        }
    
    return id;
    }

//------------------------------------------------------------------------------
// CSensorSourceCommand::CommandId
//------------------------------------------------------------------------------
//
TPtrC CSensorSourceCommand::CommandId() const
    {
    FUNC_LOG;

    TPtrC id( KNullDesC );
    if( iCommandId )
        {
        id.Set( *iCommandId );
        }
    
    return id;
    }

//------------------------------------------------------------------------------
// CSensorSourceCommand::ChannelId
//------------------------------------------------------------------------------
//
TUint32 CSensorSourceCommand::ChannelId() const
    {
    FUNC_LOG;
    
    return iChannelId;
    }

// End of file
