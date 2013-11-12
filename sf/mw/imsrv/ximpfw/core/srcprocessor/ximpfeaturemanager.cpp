/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  feature manager of XIMP
*
*/


#include "ximpfeaturemanager.h"
#include "ximpfeatureinfoimp.h"
#include "ximpobjecthelpers.h"

#include <centralrepository.h>

// ======== LOCAL FUNCTIONS ========


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPFeatureManager::CXIMPFeatureManager()
    {

    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPFeatureManager::ConstructL()
    {
    //iCenRep = CRepository::NewL();
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPFeatureManager* CXIMPFeatureManager::NewL()
    {
    CXIMPFeatureManager* self = new( ELeave ) CXIMPFeatureManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPFeatureManager::~CXIMPFeatureManager()
    {
    //delete iCenRep;
    }


// -----------------------------------------------------------------------------
// CXIMPFeatureManager::GetProtocolsL()
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CXIMPFeatureManager::GetFrameworkFeaturesL()
    {
    CXIMPFeatureInfoImp* featInfo = CXIMPFeatureInfoImp::NewLC();
    
    /*
    TInt featuresLength( KErrNotFound );
    iCenRep->Get( 0x1, featuresLength );
    
    HBufC8* tmpBuffer = HBufC8::NewLC( featuresLength );
	TPtr8 tmpPtr( tmpBuffer->Des() );
	iCenRep->Get( 0x2, tmpPtr );

    featInfo->AddFeatureL( tmpPtr );
    
    CleanupStack::PopAndDestroy( tmpBuffer );
    */

    HBufC8* pack = TXIMPObjectPacker< CXIMPFeatureInfoImp >::PackL( *featInfo );    
    CleanupStack::PopAndDestroy( featInfo ); 

    return pack;
    }
    
// End of file

