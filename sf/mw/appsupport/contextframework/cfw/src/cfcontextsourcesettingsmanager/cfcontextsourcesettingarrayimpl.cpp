/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingArrayImpl class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingarrayimpl.h"
#include "cfcontextsourcesetting.h"
#include "cftrace.h"

// CONSTANTS

#ifdef _DEBUG
// Panic category
_LIT( KPanicCat, "SETARRAY" );

// Panic reasons
enum TPanicReason
    {
    EInvalidArrayIndex
    };
    
// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }
#endif

// MEMBER FUNCTIONS

CCFContextSourceSettingArrayImpl* CCFContextSourceSettingArrayImpl::NewL()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingArrayImpl* self =
        CCFContextSourceSettingArrayImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

CCFContextSourceSettingArrayImpl* CCFContextSourceSettingArrayImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingArrayImpl* self =
        new( ELeave ) CCFContextSourceSettingArrayImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }
    
CCFContextSourceSettingArrayImpl::~CCFContextSourceSettingArrayImpl()
    {
    FUNC_LOG;
    
    iItems.ResetAndDestroy();
    }
    
CCFContextSourceSettingArrayImpl::CCFContextSourceSettingArrayImpl()
    {
    FUNC_LOG;
    }

void CCFContextSourceSettingArrayImpl::ConstructL()
    {
    FUNC_LOG;
    }
    
// METHODS

//----------------------------------------------------------------------------
// CCFContextSourceSettingArrayImpl::Count
//----------------------------------------------------------------------------
//
TInt CCFContextSourceSettingArrayImpl::Count() const
    {
    FUNC_LOG;
    
    return iItems.Count();
    }
    
//----------------------------------------------------------------------------
// CCFContextSourceSettingArrayImpl::Item
//----------------------------------------------------------------------------
//
const CCFContextSourceSetting& CCFContextSourceSettingArrayImpl::Item(
    TInt aIndex ) const
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( aIndex >= 0 && aIndex < iItems.Count(),
        Panic( EInvalidArrayIndex ) );

    return *iItems[aIndex];
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingArrayImpl::SettingItem
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingArrayImpl::AppendItemL(
    CCFContextSourceSetting* aItem )
    {
    FUNC_LOG;
    
    iItems.AppendL( aItem );
    }
