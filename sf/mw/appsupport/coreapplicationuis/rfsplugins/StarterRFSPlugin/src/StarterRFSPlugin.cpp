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
#include "StarterRFSPlugin.h"
#include "StarterRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CStarterRFSPlugin::CStarterRFSPlugin()
    {
    }

CStarterRFSPlugin::CStarterRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CStarterRFSPlugin::~CStarterRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CStarterRFSPlugin* CStarterRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CStarterRFSPlugin::NewL()"));
    #endif
    CStarterRFSPlugin* self = new (ELeave) CStarterRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CStarterRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CStarterRFSPlugin::ConstructL()"));
    #endif
    }

void CStarterRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CStarterRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CStarterRFSPlugin::GetScriptL( const TRfsReason aType, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidStarterRFSPlugin);
    TUint32 hexkey;
    if (aType == ENormalRfs)
        {
    	  hexkey = KNormStarterRFSPlugin;
    	  }
    else
        {
    	  hexkey = KInitStarterRFSPlugin;
    	  }
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

void CStarterRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
