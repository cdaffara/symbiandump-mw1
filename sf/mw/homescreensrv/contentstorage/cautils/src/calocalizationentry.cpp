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
 * Description:  ?Description
 *
 */

#include <s32mem.h>
#include "calocalizationentry.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//

CCaLocalizationEntry::~CCaLocalizationEntry( )
    {
    iTableName.Close();
    iAttribName.Close();
    iStringId.Close();
    iLocalName.Close();
    iQmFilename.Close();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaLocalizationEntry* CCaLocalizationEntry::NewL( )
    {
    CCaLocalizationEntry* loc = CCaLocalizationEntry::NewLC( );
    CleanupStack::Pop( loc );
    return loc;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C CCaLocalizationEntry* CCaLocalizationEntry::NewLC( )
    {
    CCaLocalizationEntry* self = new ( ELeave ) CCaLocalizationEntry( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

/*// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( iId );
    aStream.WriteInt32L( iRowId );
    aStream.WriteUint32L( iTableName.Length() );
    aStream.WriteL( iTableName, iTableName.Length() );
    aStream.WriteUint32L( iAttribName.Length() );
    aStream.WriteL( iAttribName, iAttribName.Length() );
    aStream.WriteUint32L( iStringId.Length() );
    aStream.WriteL( iStringId, iStringId.Length() );
    aStream.CommitL();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::InternalizeL( RReadStream& aStream )
    {
    iId = aStream.ReadInt32L();
    iRowId = aStream.ReadInt32L();
    
    TInt length = aStream.ReadUint32L();
    iTableName.Close();
    iTableName.CreateL( iTableName );
    aStream.ReadL( iTableName, length );
    
    length = aStream.ReadUint32L();
    iAttribName.Close();
    iAttribName.CreateL( iAttribName );
    aStream.ReadL( iAttribName, length );
    
    length = aStream.ReadUint32L();
    iStringId.Close();
    iStringId.CreateL( iStringId );
    aStream.ReadL( iStringId, length );
    }*/

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaLocalizationEntry::ConstructL( )
    {

    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCaLocalizationEntry::CCaLocalizationEntry( )
    {

    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCaLocalizationEntry::GetRowId( ) const
    {
    return iRowId;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCaLocalizationEntry::GetTextId( ) const
    {
    return iId;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const RBuf& CCaLocalizationEntry::GetAttributeName( ) const
    {
    return iAttribName;
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetAttributeNameL( const TDesC& aAttribName )
    {
    iAttribName.Close( );
    iAttribName.CreateL( aAttribName );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const RBuf& CCaLocalizationEntry::GetTableName( ) const
    {
    return iTableName;
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetTableNameL( const TDesC& aTableName )
    {
    iTableName.Close( );
    iTableName.CreateL( aTableName );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const RBuf& CCaLocalizationEntry::GetStringId( ) const
    {
    return iStringId;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const RBuf& CCaLocalizationEntry::GetQmFilename( ) const
    {
    return iQmFilename;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetQmFilenameL( const TDesC& aQmFilename )
    {
	iQmFilename.Close( );
	iQmFilename.CreateL( aQmFilename );
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C const RBuf& CCaLocalizationEntry::GetLocalizedString( ) const
    {
    return iLocalName;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetLocalizedStringL( const TDesC& aLocalName )
    {
    iLocalName.Close( );
    iLocalName.CreateL( aLocalName );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetStringIdL( const TDesC& aStringId )
    {
    iStringId.Close( );
    iStringId.CreateL( aStringId );
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetRowId( TUint aRowId )
    {
    iRowId = aRowId;
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CCaLocalizationEntry::SetTextId( TUint aId )
    {
    iId = aId;
    }

