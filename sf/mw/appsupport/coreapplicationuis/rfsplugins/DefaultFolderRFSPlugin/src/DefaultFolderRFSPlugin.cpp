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
#include "DefaultFolderRFSPlugin.h"
#include "DefaultFolderRFSPluginPrivateCRKeys.h"

// LOCAL CONSTANTS
const TInt KHexLength = 8;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CDefaultFolderRFSPlugin::CDefaultFolderRFSPlugin()
    {
    }

CDefaultFolderRFSPlugin::CDefaultFolderRFSPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CDefaultFolderRFSPlugin::~CDefaultFolderRFSPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CDefaultFolderRFSPlugin* CDefaultFolderRFSPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CDefaultFolderRFSPlugin::NewL()"));
    #endif
    CDefaultFolderRFSPlugin* self = new (ELeave) CDefaultFolderRFSPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CDefaultFolderRFSPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CDefaultFolderRFSPlugin::ConstructL()"));
    #endif
    }

void CDefaultFolderRFSPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CDefaultFolderRFSPlugin::RestoreFactorySettingsL()"));
    #endif
    }

void CDefaultFolderRFSPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    // Add CenRep UID and key
    TUid hexuid(KCRUidDefaultFolderRFSPlugin);
    TUint32 hexkey1(KInitDefaultFolderRFSPlugin1);
    TUint32 hexkey2(KInitDefaultFolderRFSPlugin2);
    HBufC* hbuf = HBufC::New( KMaxPath );
    TPtr buf = hbuf->Des();
    buf.Zero();
    buf.AppendNumFixedWidthUC( hexuid.iUid, EHex, KHexLength );
    buf.Append(_L(":"));
    buf.AppendNumFixedWidthUC( hexkey1, EHex, KHexLength );
    buf.Append(_L(","));
    buf.AppendNumFixedWidthUC( hexkey2, EHex, KHexLength );    
    buf.Append(_L(";"));
    RDebug::Print( buf );
    aPath.Copy( buf );    
    delete hbuf;
    }

void CDefaultFolderRFSPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
