/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  cbselement.cpp
*
*/

//  INCLUDE FILES

#include <utf.h>
#include "e32base.h"
#include "s32strm.h"
#include "cbselement.h"
#include "cbsbitmap.h"


CBSElement* CBSElement::NewL(
						const TDesC8& aElementId,
						TInt aData )
    {

	CBSElement* self = new ( ELeave ) CBSElement() ;
    CleanupStack::PushL( self );
    self->ConstructL( EBSInt,
    				  aElementId,
    				  aData,
    				  ETrue,
    				  NULL,
    				  KNullDesC,
    				  KNullDesC8,
    				  NULL );
    CleanupStack::Pop( self );  //self
    return self;
    }


CBSElement* CBSElement::NewL(
						const TDesC8& aElementId,
						const TDesC& aData,
						TBSElementType aType /*= EText*/ )
    {
    if( aType != EBSFile && aType != EBSText )
        {
        User::Leave( KErrArgument );
        }

    CBSElement* self = new ( ELeave ) CBSElement() ;
    CleanupStack::PushL( self );
    self->ConstructL( aType,
    				  aElementId,
    				  0,
    				  EFalse,
    				  NULL,
    				  aData,
    				  KNullDesC8,
    				  NULL);
    CleanupStack::Pop( self );  //self
    return self;
    }


CBSElement* CBSElement::NewL(
						const TDesC8& aElementId,
						const TDesC8& aData )
    {
    CBSElement* self = new ( ELeave ) CBSElement() ;
    CleanupStack::PushL( self );
    self->ConstructL( EBSBuffer,
    				  aElementId,
    				  0,
    				  EFalse,
    				  NULL,
    				  KNullDesC,
    				  aData,
    				  NULL);
    CleanupStack::Pop( self );  //self
    return self;
    }

CBSElement* CBSElement::NewL(
						const TDesC8& aElementId,
						RBSObjOwningPtrArray<MBSElement>& aStructure )
    {
    CBSElement* self = new ( ELeave ) CBSElement();
    CleanupStack::PushL( self );
    self->ConstructL( EBSList,
    				  aElementId,
    				  0,
    				  EFalse,
    				  NULL,
    				  KNullDesC,
    				  KNullDesC8,
    				  &aStructure );
    CleanupStack::Pop( self );  //self
    return self;
    }

CBSElement* CBSElement::NewL(
    					const TDesC8& aElementId,
    					CBSBitmap* aBitmap )
    {
    CBSElement* self = new ( ELeave ) CBSElement() ;
    CleanupStack::PushL( self );
    self->ConstructL( EBSBitmap,
    				  aElementId,
    				  0,
    				  EFalse,
    				  aBitmap,
    				  KNullDesC,
    				  KNullDesC8,
    				  NULL );

	if( !aBitmap )
		{
		User::Leave( KErrArgument );
		}

    CleanupStack::Pop( self );  //self
    return self;
    }

void CBSElement::ConstructL(
						 TBSElementType aElementType,
					 	 const TDesC8& aElementId,
					 	 TInt aIntData,
					 	 TBool aIntDataInitialized,
					 	 CBSBitmap* aBitmap,
					 	 const TDesC& aTextData,
					 	 const TDesC8& aBufferData,
					 	 RBSObjOwningPtrArray<MBSElement>* aStructure )
    {
	iElementType = aElementType;

	iElementId = aElementId.AllocL();

	iIntData = aIntData;
	iIntDataInitialized = aIntDataInitialized;

	iBitmapData = aBitmap;

	if( 0 != aTextData.Compare( KNullDesC() ) )
		{
		iTextData = aTextData.AllocL();
		}
	if( 0 != aBufferData.Compare( KNullDesC8() ) )
		{
		iBufferData = aBufferData.AllocL();
		}
	if( aStructure )
		{
		iArrayInitialized = ETrue;
		iArray = *aStructure;
		}
    }


CBSElement::~CBSElement()
    {
	delete iTextData;
	delete iBufferData;
	delete iElementId;

	iArray.Close();
	delete iBitmapData;
    }


CBSElement::CBSElement()
	{
	}

void CBSElement::Close()
	{
	delete this;
	}
	
	
TBSElementType CBSElement::ElementType()
	{
	return iElementType;
	}

const TDesC8& CBSElement::ElementId()
	{
	return *iElementId;
	}

TInt CBSElement::IntDataL()
	{
	if( !iIntDataInitialized )
		{
		User::Leave( KErrArgument );
		}
	return iIntData;
	}

const TDesC& CBSElement::TextDataL()
	{
	if( !iTextData )
		{
		User::Leave( KErrArgument );
		}
	return *iTextData;
	}

const TDesC8& CBSElement::BufferDataL()
	{
	if( !iBufferData )
		{
		User::Leave( KErrArgument );
		}
	return *iBufferData;
	}

const CBSBitmap& CBSElement::BitmapDataL()
    {
	if( !iBitmapData )
		{
		User::Leave( KErrArgument );
		}
    return *iBitmapData;
    }

TArray<MBSElement*> CBSElement::GetStructureL()
	{
	if( !iArrayInitialized )
		{
		User::Leave( KErrArgument );
		}

	return iArray.Array();
	}

const MBSElement& CBSElement::FindSubEntryL( const TDesC& aElementId )
	{
    if ( !iArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    
	MBSElement* element = NULL;
	HBufC8* id = CnvUtfConverter::ConvertFromUnicodeToUtf8L( aElementId );	
	User::LeaveIfNull( id );
	CleanupStack::PushL( id );
	for ( TInt i( 0 ) ; i < iArray.Count() ; i++ )
	    {
	    if ( iArray[ i ] )
	        {
    	    if ( !iArray[ i ]->ElementId().CompareC( *id ) )
    	        {
    	        // Match found
    	        element = iArray[ i ];
    	        i = iArray.Count();// Exit loop.
    	        }
	        }
	    }
	CleanupStack::PopAndDestroy( id );
	  
	if ( !element )
	    {
	    User::Leave( KErrNotFound );
	    }
	      
	return *element;
	}

void CBSElement::ExternalizeL( RWriteStream& aWriteStream )
    {
    // Write common header for all elements
    aWriteStream.WriteInt16L( iElementType );
    aWriteStream.WriteInt16L( iElementId->Length() );
    if( iElementId->Length() > 0 )
        {
        // Write ID only if it's defined
        aWriteStream.WriteL( *iElementId );
        }

    // Write element type specific data
    switch( iElementType )
        {
        case EBSInt:
            {
            aWriteStream.WriteInt16L( iIntData );
            break;
            }
        case EBSText:     // flowthrough
        case EBSFile:
            {
            aWriteStream.WriteInt16L( iTextData->Length() );
            aWriteStream.WriteL( *iTextData );
            break;
            }
        case EBSList:
            {
			TInt count = iArray.Count();
            aWriteStream.WriteInt16L( count );
            // Write all subitems to stream
            for( TInt i = 0; i < count; i++ )
                {
				MBSElement* element = iArray[i];
                element->ExternalizeL( aWriteStream );
                }
            break;
            }
        case EBSBitmap:
            {
			TPtrC8 ptr = iBitmapData->BitmapFileId();
			aWriteStream.WriteInt16L( ptr.Length() );
            aWriteStream.WriteL( ptr );
            
            aWriteStream.WriteInt16L( iBitmapData->BitmapId() );
            aWriteStream.WriteInt16L( iBitmapData->BitmapMaskId() );
            aWriteStream.WriteInt16L( iBitmapData->SkinIdMajor() );
            aWriteStream.WriteInt16L( iBitmapData->SkinIdMinor() );

            break;
            }
        case EBSBuffer:
        	{
            aWriteStream.WriteInt16L( iBufferData->Length() );
            aWriteStream.WriteL( *iBufferData );
        	break;
        	}
        default:
            {
            // unknown type!
            User::Leave( KErrCorrupt );
            break;
            }
        }
    }


//  END OF FILE
