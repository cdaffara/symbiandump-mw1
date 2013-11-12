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
#include "UnitconverterRFSPlugin.h"
#include "UnitconverterRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CUnitconverterRFSPlugin::CUnitconverterRFSPlugin()
    {
    }

CUnitconverterRFSPlugin::CUnitconverterRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CUnitconverterRFSPlugin::~CUnitconverterRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CUnitconverterRFSPlugin* CUnitconverterRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CUnitconverterRFSPlugin::NewL()"));
    #endif
    CUnitconverterRFSPlugin* self = new (ELeave) CUnitconverterRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CUnitconverterRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CUnitconverterRFSPlugin::ConstructL()"));
    #endif
    }

void CUnitconverterRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CUnitconverterRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CUnitconverterRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidUnitconverterRFSPlugin);
    TUint32 hexkey(KNormUnitconverterRFSPlugin);
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

void CUnitconverterRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
