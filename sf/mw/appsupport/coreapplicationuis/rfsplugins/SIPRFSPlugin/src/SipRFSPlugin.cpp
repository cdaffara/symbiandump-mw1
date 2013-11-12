/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "SipRFSPlugin.h"
#include "SipRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSIPRFSPlugin::CSIPRFSPlugin()
    {
    }

CSIPRFSPlugin::CSIPRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CSIPRFSPlugin::~CSIPRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CSIPRFSPlugin* CSIPRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CSIPRFSPlugin::NewL()"));
    #endif
    CSIPRFSPlugin* self = new (ELeave) CSIPRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CSIPRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CSIPRFSPlugin::ConstructL()"));
    #endif
    }

void CSIPRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CSIPRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CSIPRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidSIPRFSPlugin);
    TUint32 hexkey(KNormSIPRFSPlugin);
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

void CSIPRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
