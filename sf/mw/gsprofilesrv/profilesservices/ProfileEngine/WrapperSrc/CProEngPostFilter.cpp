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
* Description: Implementation of CProEngPostFilter.
*
*/



// INCLUDE FILES
#include "CProEngPostFilter.h"

#include <MCLFItem.h>
#include "CProEngMediaVariation.h"
#include "CProEngDrmCommonWrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngPostFilter::CProEngPostFilter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProEngPostFilter::CProEngPostFilter()
    {
    }

// -----------------------------------------------------------------------------
// CProEngPostFilter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProEngPostFilter::ConstructL()
    {
    iMediaVariation = CProEngMediaVariation::NewL();
    iDrmWrapper = CProEngDrmCommonWrapper::NewL();
    }

// -----------------------------------------------------------------------------
// CProEngPostFilter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngPostFilter* CProEngPostFilter::NewL()
    {
    CProEngPostFilter* self = new (ELeave) CProEngPostFilter();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProEngPostFilter::~CProEngPostFilter()
    {
    delete iDrmWrapper;
    delete iMediaVariation;
    }

// -----------------------------------------------------------------------------
// CProEngPostFilter::FilterItemsL
// Method for filtering the source list.
// -----------------------------------------------------------------------------
//
void CProEngPostFilter::FilterItemsL( const TArray<MCLFItem*>& aItemList,
                                  RPointerArray<MCLFItem>& aFilteredItemList )
    {
    // Process all items in the item list
    TInt count( aItemList.Count() );
    for( TInt i = 0; i < count; i++ )
        {
        MCLFItem* currentItem = aItemList[i];
        // Get the path of the media file from the item
        TPtrC fileNameAndPath;
        TInt error( currentItem->GetField( ECLFFieldIdFileNameAndPath,
                                       fileNameAndPath ) );
        
        // Add the alert tone file to the filtered list,
        // if it is OK according to the Media variation
        if( error == KErrNone )
            {
            TBuf< KMaxDataTypeLength > dataType;
            iMediaVariation->GetDataTypeL( fileNameAndPath, dataType );
            TBool isSupported( iMediaVariation->IsSupported( dataType ) );
            TBool isProtected( iDrmWrapper->IsProtected( fileNameAndPath ) );
            if( ( isProtected &&
                  iMediaVariation->IsAllowedProtected( dataType ) ) ||
                ( !isProtected &&
                     iMediaVariation->IsAllowedUnProtected( dataType ) ) )
                {
                aFilteredItemList.AppendL( currentItem );
                }
            }
        }
    }

//  End of File

