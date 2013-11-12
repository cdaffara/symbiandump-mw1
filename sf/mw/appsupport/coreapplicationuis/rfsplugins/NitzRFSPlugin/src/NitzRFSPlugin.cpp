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
#include "NitzRFSPlugin.h"
#include "NitzRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CNitzRFSPlugin::CNitzRFSPlugin()
    {
    }

CNitzRFSPlugin::CNitzRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CNitzRFSPlugin::~CNitzRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CNitzRFSPlugin* CNitzRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CNitzRFSPlugin::NewL()"));
    #endif
    CNitzRFSPlugin* self = new (ELeave) CNitzRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CNitzRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CNitzRFSPlugin::ConstructL()"));
    #endif
    }

void CNitzRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CNitzRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CNitzRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidNitzRFSPlugin);
    TUint32 hexkey(KNormNitzRFSPlugin);
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

void CNitzRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
