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
* Description:  CFContextSourceSettingImpl class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingimpl.h"
#include "cftrace.h"

CCFContextSourceSettingImpl* CCFContextSourceSettingImpl::NewL()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingImpl* self = CCFContextSourceSettingImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

CCFContextSourceSettingImpl* CCFContextSourceSettingImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingImpl* self = new( ELeave ) CCFContextSourceSettingImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }
    
CCFContextSourceSettingImpl::~CCFContextSourceSettingImpl()
    {
    FUNC_LOG;
    
    delete iName;
    iAttributes.ResetAndDestroy();
    iParameters.ResetAndDestroy();
    }
    
CCFContextSourceSettingImpl::CCFContextSourceSettingImpl()
    {
    FUNC_LOG;
    }

void CCFContextSourceSettingImpl::ConstructL()
    {
    FUNC_LOG;
    
    iName = KNullDesC().AllocL();
    }
    
// METHODS

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::Name
//----------------------------------------------------------------------------
//
const TDesC& CCFContextSourceSettingImpl::Name() const
    {
    FUNC_LOG;
    
    return *iName;
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::Attributes
//----------------------------------------------------------------------------
//
const RKeyValueArray& CCFContextSourceSettingImpl::Attributes() const
    {
    FUNC_LOG;
    
    return iAttributes;
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::Parameters
//----------------------------------------------------------------------------
//
const RSettingParameterArray& CCFContextSourceSettingImpl::Parameters() const
    {
    FUNC_LOG;
    
    return iParameters;
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::SetNameL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingImpl::SetNameL( const TDesC& aName )
    {
    FUNC_LOG;
    
    TPtr namePtr = iName->Des();
    if( aName.Length() < namePtr.MaxLength() )
        {
        // Just copy
        namePtr.Copy( aName );
        }
    else
        {
        // Create a new one
        delete iName;
        iName = NULL;
        iName = aName.AllocL();
        }
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::AddAttributeL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingImpl::AddAttributeL(
    CCFKeyValuePair* aAttribute )
    {
    FUNC_LOG;
    
    iAttributes.AppendL( aAttribute );
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingImpl::AddParameterL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingImpl::AddParameterL(
    CCFContextSourceSettingParameter* aParameter )
    {
    FUNC_LOG;
    
    iParameters.AppendL( aParameter );
    }
