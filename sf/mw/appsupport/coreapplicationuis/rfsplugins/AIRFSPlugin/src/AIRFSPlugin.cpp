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
* Description: 
*     
*
*/


// INCLUDE FILES
#include "AIRFSPlugin.h"
#include "AIRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CAIRFSPlugin::CAIRFSPlugin()
    {
    }

CAIRFSPlugin::CAIRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CAIRFSPlugin::~CAIRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CAIRFSPlugin* CAIRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CAIRFSPlugin::NewL()"));
    #endif
    CAIRFSPlugin* self = new (ELeave) CAIRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CAIRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CAIRFSPlugin::ConstructL()"));
    #endif
    }

void CAIRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CAIRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CAIRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidAIRFSPlugin);
    TUint32 hexkey(KNormAIRFSPlugin);
    HBufC* hbuf = HBufC::New( KMaxPath );
    TPtr buf = hbuf->Des();
    buf.Zero();
    buf.AppendNumFixedWidthUC( hexuid.iUid, EHex, KHexLength );
    buf.Append(_L(":"));
    buf.AppendNumFixedWidthUC( hexkey, EHex, KHexLength );
    buf.Append(_L(";"));
    RDebug::Print( buf );
    aPath.Copy( buf );    
    delete hbuf;
    }

void CAIRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
