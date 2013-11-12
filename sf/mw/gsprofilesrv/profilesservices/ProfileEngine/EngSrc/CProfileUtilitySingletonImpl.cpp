/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CProfileUtilitySingletonImpl.
*
*/



// INCLUDE FILES
#include "CProfileUtilitySingletonImpl.h"
#include "CProfilesLocalFeatures.h"
#include "ProfilesDebug.h"
#include <MProfilesLocalFeatures.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::CProfileUtilitySingletonImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileUtilitySingletonImpl::CProfileUtilitySingletonImpl()
    {
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileUtilitySingletonImpl::ConstructL()
    {
    iLocalFeatures = CProfilesLocalFeatures::NewL();
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::NewL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileUtilitySingletonImpl* CProfileUtilitySingletonImpl::NewL()
    {
    CProfileUtilitySingletonImpl* self
        = new ( ELeave ) CProfileUtilitySingletonImpl();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::ProfilesLocalFeatures
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MProfilesLocalFeatures& CProfileUtilitySingletonImpl::ProfilesLocalFeatures()
    {
    return *iLocalFeatures;
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::IncrementReferences
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileUtilitySingletonImpl::IncrementReferences()
    {
    ++iReferences;
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::DecrementReferences
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileUtilitySingletonImpl::DecrementReferences()
    {
    return ( --iReferences <= 0 );
    }

// -----------------------------------------------------------------------------
// CProfileUtilitySingletonImpl::~CProfileUtilitySingletonImpl
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CProfileUtilitySingletonImpl::~CProfileUtilitySingletonImpl()
    {
    delete iLocalFeatures;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ProfileUtilityInstanceL creates an implementation of MProfileUtilitySingleton
// if needed, increments the reference count and returns a reference to the only
// instance of it.
// Returns a reference to the only instance of MProfileUtilitySingleton.
// -----------------------------------------------------------------------------
//
EXPORT_C MProfileUtilitySingleton& ProfileUtilityInstanceL()
    {
    CProfileUtilitySingletonImpl* instance =
        static_cast<CProfileUtilitySingletonImpl*>( Dll::Tls() );
    if( instance == NULL )
        {
        instance = CProfileUtilitySingletonImpl::NewL();
        TInt err( Dll::SetTls( instance ) );
        if( err )
            {
            delete instance;
            //delete static_cast<CProfileUtilitySingletonImpl*>( instance );
            User::Leave( err );
            }
        }
    instance->IncrementReferences();
    //return static_cast<MProfileUtilitySingleton&>( instance );
    return *instance;
    }

// -----------------------------------------------------------------------------
// ReleaseProfileUtility decrements the count of references to the only instance
// of MProfileUtilitySingleton implementation and releases the resources if
// there are no more references to it.
// -----------------------------------------------------------------------------
//
EXPORT_C void ReleaseProfileUtility()
    {
    TAny* instance = Dll::Tls();
    if( instance != NULL )
        {
        CProfileUtilitySingletonImpl* singleton =
            static_cast<CProfileUtilitySingletonImpl*>( instance );
        if( singleton->DecrementReferences() )
            {
            delete singleton;
            Dll::SetTls( NULL );
            }
        }
    }

//  End of File


