/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Configuration item base for configuration items.
*
*/


#include "ximpconfigurationitembase.h"
#include "ximpitemparent.h"

// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::OrderOfItems()
// ---------------------------------------------------------------------------
//
TInt CXIMPConfigurationItemBase::OrderOfItems( const CXIMPConfigurationItemBase& aFirst, 
                                               const CXIMPConfigurationItemBase& aSecond )
    {
    if( &aFirst < &aSecond )
        {
        return -1;
        }
    else if( &aFirst == &aSecond )
        {
        return 0;
        }
    else
        {
        return 1;
        }
    }    


// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::CXIMPConfigurationItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPConfigurationItemBase::CXIMPConfigurationItemBase( MXIMPItemParentBase& aParent )
: CXIMPSubscriptionItemBase( aParent )
    {
    }

// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::BaseConstructL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::BaseConstructL()
    {
    CXIMPSubscriptionItemBase::BaseConstructL();
    }

// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::~CXIMPConfigurationItemBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPConfigurationItemBase::~CXIMPConfigurationItemBase()
    {
    iConfigContexts.Close();
    iConfigItems.Close();
    }

// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::ConfigurationStatus()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPConfigurationItemBase::TConfigurationStatus 
           CXIMPConfigurationItemBase::ConfigurationStatus( MXIMPPscContext* aContext )
    {
    // Calculate situation
    TInt count = iConfigContexts.Count() + iConfigItems.Count();
    TBool contextFound = IsContext( aContext );
    if( count == 0 )
        {
        return ENotConfiguredAtAll;
        }
    else if( contextFound )
        {
        if( count == 1 )
            {
            return EConfiguredForCtxOnly;
            }
        return EConfiguredForCtxAndOthers;
        }
    return EConfiguredForOtherCtxOnly;
    }
    
// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::AddConfiguratorL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::AddConfiguratorL( MXIMPPscContext* aContext )
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    if( IsContext( aContext ) )
        {
        User::Leave( KErrAlreadyExists );
        }    
    iConfigContexts.InsertInOrderL( aContext, order );
    User::LeaveIfError( Open() );
    }
    
// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::RemoveConfigurator()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::RemoveConfigurator( MXIMPPscContext* aContext )
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );
    TInt index = iConfigContexts.FindInOrder( aContext, order );
    if( index >= 0 )
        {
        iConfigContexts.Remove( index );
        Close();
        }
    else
        {
        iParent.UnregisterExpiringItem( this );
        }
    }

// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::AddConfiguratorL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::AddConfiguratorL( CXIMPConfigurationItemBase* aItem )
    {
    TLinearOrder< CXIMPConfigurationItemBase > order( OrderOfItems );
    TInt index = iConfigItems.FindInOrder( aItem, order );    
    if( index != KErrNotFound )
        {
        return;
        }
    
    iConfigItems.InsertInOrderL( aItem, order );
    User::LeaveIfError( Open() );
    }
    
// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::RemoveConfigurator()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::RemoveConfigurator( CXIMPConfigurationItemBase* aItem )
    {
    TLinearOrder< CXIMPConfigurationItemBase > order( OrderOfItems );
    TInt index = iConfigItems.FindInOrder( aItem, order );
    if( index >= 0 )
        {
        iConfigItems.Remove( index );
        Close();
        }
    else
        {
        iParent.UnregisterExpiringItem( this );
        }
    }    
    
// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::IsContext()
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXIMPConfigurationItemBase::IsContext( MXIMPPscContext* aContext ) const
    {
    TLinearOrder< MXIMPPscContext > order( OrderOfContexts );    
    return iConfigContexts.FindInOrder( aContext, order ) != KErrNotFound;
    }
    
// ---------------------------------------------------------------------------
// CXIMPConfigurationItemBase::ForceClose()
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPConfigurationItemBase::ForceClose()
    {
    CXIMPSubscriptionItemBase::ForceClose();
    TInt count = iConfigContexts.Count();
    for( TInt a = 0; a < count; ++a )
        {
        // RemoveSubscriber removes first one every time.
        RemoveConfigurator( iConfigContexts[ 0 ] );
        }
    count = iConfigItems.Count();
    for( TInt a = 0; a < count; ++a )
        {
        RemoveConfigurator( iConfigItems[ 0 ] );
        }
    }    
// End of file
