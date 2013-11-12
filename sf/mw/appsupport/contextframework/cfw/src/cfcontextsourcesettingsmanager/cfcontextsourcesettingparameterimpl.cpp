/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingParameterImpl class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingparameterimpl.h"
#include "cftrace.h"

CCFContextSourceSettingParameterImpl* CCFContextSourceSettingParameterImpl::NewL()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingParameterImpl* self =
        CCFContextSourceSettingParameterImpl::NewLC();
    CleanupStack::Pop( self );
    
    return self;
    }

CCFContextSourceSettingParameterImpl* CCFContextSourceSettingParameterImpl::NewLC()
    {
    FUNC_LOG;
    
    CCFContextSourceSettingParameterImpl* self =
        new( ELeave ) CCFContextSourceSettingParameterImpl;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }
    
CCFContextSourceSettingParameterImpl::~CCFContextSourceSettingParameterImpl()
    {
    FUNC_LOG;
    
    delete iName;
    iAttributes.ResetAndDestroy();
    }
    
CCFContextSourceSettingParameterImpl::CCFContextSourceSettingParameterImpl()
    {
    FUNC_LOG;
    }

void CCFContextSourceSettingParameterImpl::ConstructL()
    {
    FUNC_LOG;
    
    iName = KNullDesC().AllocL();
    }
    
// METHODS

//----------------------------------------------------------------------------
// CCFContextSourceSettingParameterImpl::Name
//----------------------------------------------------------------------------
//
const TDesC& CCFContextSourceSettingParameterImpl::Name() const
    {
    FUNC_LOG;
    
    return *iName;
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingParameterImpl::Attributes
//----------------------------------------------------------------------------
//
const RKeyValueArray& CCFContextSourceSettingParameterImpl::Attributes() const
    {
    FUNC_LOG;
    
    return iAttributes;
    }

//----------------------------------------------------------------------------
// CCFContextSourceSettingParameterImpl::SetNameL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingParameterImpl::SetNameL( const TDesC& aName )
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
// CCFContextSourceSettingParameterImpl::AddAttributeL
//----------------------------------------------------------------------------
//
void CCFContextSourceSettingParameterImpl::AddAttributeL(
    CCFKeyValuePair* aAttribute )
    {
    FUNC_LOG;
    
    iAttributes.AppendL( aAttribute );
    }
