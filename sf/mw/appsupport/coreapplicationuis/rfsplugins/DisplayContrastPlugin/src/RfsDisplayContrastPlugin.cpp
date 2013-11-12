/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <hal.h>
#include "RfsDisplayContrastPlugin.h"

// LOCAL CONSTANTS
_LIT(KScriptPath, "");

// practical contrast values for HAL are between -20 and 20
const TInt KRfsDefaultContrastSetting = 0;

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CRfsDisplayContrastPlugin::CRfsDisplayContrastPlugin()
    {
    }

CRfsDisplayContrastPlugin::CRfsDisplayContrastPlugin(TAny* /*aInitParams*/)
    {
    }

// Destructor
CRfsDisplayContrastPlugin::~CRfsDisplayContrastPlugin()
    {
    }

// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CRfsDisplayContrastPlugin* CRfsDisplayContrastPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsDisplayContrastPlugin::NewL()"));
    #endif
    CRfsDisplayContrastPlugin* self = new (ELeave) CRfsDisplayContrastPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//
void CRfsDisplayContrastPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsDisplayContrastPlugin::ConstructL()"));
    #endif
    }

void CRfsDisplayContrastPlugin::RestoreFactorySettingsL( const TRfsReason /*aType*/ )
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsDisplayContrastPlugin::RestoreFactorySettingsL()"));
    #endif
    HAL::Set( HALData::EDisplayContrast, KRfsDefaultContrastSetting );
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsDisplayContrastPlugin::RestoreFactorySettingsL(): End"));
    #endif
    }

void CRfsDisplayContrastPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& aPath )
    {
    aPath.Copy( KScriptPath);
    }

void CRfsDisplayContrastPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                        TDesC& /*aCommand*/ )
    {
    }

// End of file
