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
 * Description: Definition of entry attribute
 *
 */

// INCLUDE FILES
#include <s32mem.h>
#include "cainnericondescription.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCaInnerIconDescription::~CCaInnerIconDescription
// ---------------------------------------------------------
//
CCaInnerIconDescription::~CCaInnerIconDescription()
    {
    iFileName.Close();
    iSkinId.Close();
    iApplicationId.Close();
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::NewL
// ---------------------------------------------------------
//
EXPORT_C CCaInnerIconDescription* CCaInnerIconDescription::NewL()        
    {
    CCaInnerIconDescription* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::NewLC
// ---------------------------------------------------------
//
EXPORT_C CCaInnerIconDescription* CCaInnerIconDescription::NewLC()
    {
    CCaInnerIconDescription* self = new ( ELeave ) CCaInnerIconDescription();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::ConstructL
// ---------------------------------------------------------
//
void CCaInnerIconDescription::ConstructL()  
    {
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::CCaInnerIconDescription
// ---------------------------------------------------------
//
CCaInnerIconDescription::CCaInnerIconDescription()
    {   
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::SetId
// ---------------------------------------------------------
//
EXPORT_C void CCaInnerIconDescription::SetId( const TInt aId )
    {
    iId = aId;    
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::SetFileNameL
// ---------------------------------------------------------
//
EXPORT_C void CCaInnerIconDescription::SetFileNameL( const TDesC& aFileName )
    {
    if( iFileName.Length() > 0 )
        {
        iFileName.Close();
        }
    iFileName.CreateL( aFileName );
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::SetSkinIdL
// ---------------------------------------------------------
//
EXPORT_C void CCaInnerIconDescription::SetSkinIdL( const TDesC& aSkinId )
    {
    if( iSkinId.Length() > 0 )
        {
        iSkinId.Close();
        }
    iSkinId.CreateL( aSkinId );
    }

// ---------------------------------------------------------
// CCaInnerIconDescription::SetApplicationIdL
// ---------------------------------------------------------
//
EXPORT_C void CCaInnerIconDescription::SetApplicationIdL( const TDesC& aApplicationId )
    {
    if( iApplicationId.Length() > 0 )
        {
        iApplicationId.Close();
        }
    iApplicationId.CreateL( aApplicationId );
    }

// ---------------------------------------------------------------------------
// CCaInnerIconDescription::ExternalizeL
// ---------------------------------------------------------------------------
//
void CCaInnerIconDescription::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iId );
    aStream.WriteUint32L( iFileName.Length() );
    aStream.WriteL( iFileName, iFileName.Length() );
    aStream.WriteUint32L( iSkinId.Length() );
    aStream.WriteL( iSkinId, iSkinId.Length() );
    aStream.WriteUint32L( iApplicationId.Length() );
    aStream.WriteL( iApplicationId, iApplicationId.Length() );
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
// CCaInnerIconDescription::InternalizeL
// ---------------------------------------------------------------------------
//
void CCaInnerIconDescription::InternalizeL( RReadStream& aStream )
    {
    iId = aStream.ReadInt32L();
    
    TUint length = aStream.ReadUint32L();
    iFileName.Close();
    iFileName.CreateL( length );
    aStream.ReadL( iFileName, length );

    length = aStream.ReadUint32L();
    iSkinId.Close();
    iSkinId.CreateL( length );
    aStream.ReadL( iSkinId, length );
    
    length = aStream.ReadUint32L();
    iApplicationId.Close();
    iApplicationId.CreateL( length );
    aStream.ReadL( iApplicationId, length );
    }

//  End of File
