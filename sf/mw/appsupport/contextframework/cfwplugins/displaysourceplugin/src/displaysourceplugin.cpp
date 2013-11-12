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
* Description:  CDisplaySourcePlugIn class implementation.
*
*/


#include <cfcontextobject.h>
#include <cfcontextinterface.h>

#include "displaysourceplugin.h"
#include "displaysourcecontextdef.h"
#include "displayservicebase.h"
#include "displayservicelight.h"
#include "displayserviceuilayout.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

CDisplaySourcePlugIn* CDisplaySourcePlugIn::NewL(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CDisplaySourcePlugIn* self = CDisplaySourcePlugIn::NewLC( aParams );
    CleanupStack::Pop( self );

    return self;
    }

CDisplaySourcePlugIn* CDisplaySourcePlugIn::NewLC(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CDisplaySourcePlugIn* self =
        new( ELeave ) CDisplaySourcePlugIn( aParams );
    CleanupStack::PushL( self );

    return self;
    }

// Destructor
CDisplaySourcePlugIn::~CDisplaySourcePlugIn()
    {
    FUNC_LOG;
    
    // Stop services
    TInt count( iServices.Count() );
    for( TInt i = 0; i < count; i++ )
        {
        iServices[i]->Stop();
        }
    
    // Delete services
    iServices.ResetAndDestroy();
    }

CDisplaySourcePlugIn::CDisplaySourcePlugIn(
    TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {
    FUNC_LOG;
    }

// METHODS

//----------------------------------------------------------------------------
// CDisplaySourcePlugIn::HandleSettingL
//----------------------------------------------------------------------------
//
void CDisplaySourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* /*aSettingList*/ )
	{
    FUNC_LOG;
    
	// Not called since we don't have settings
	}

//----------------------------------------------------------------------------
// CDisplaySourcePlugIn::DefineContextsL
//----------------------------------------------------------------------------
//
void CDisplaySourcePlugIn::DefineContextsL()
    {
    FUNC_LOG;

    // Nothing to do here.
    }

//----------------------------------------------------------------------------
// CDisplaySourcePlugIn::InitializeL
//----------------------------------------------------------------------------
//
void CDisplaySourcePlugIn::InitializeL()
    {
    FUNC_LOG;
    
    CDisplayServiceBase* service = NULL;
    
    // Create light service
    service = CDisplayServiceLight::NewLC( iCF );
    service->StartL();
    iServices.AppendL( service );
    CleanupStack::Pop( service );
    
    // Create UI layout service
    service = CDisplayServiceUILayout::NewLC( iCF );
    service->StartL();
    iServices.AppendL( service );
    CleanupStack::Pop( service );
    }

// End of file
