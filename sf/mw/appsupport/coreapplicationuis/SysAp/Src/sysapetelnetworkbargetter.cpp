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
* Description:  CSysApEtelNetworkBarGetter implementation
*
*/


// INCLUDES
#include <e32debug.h>
#include <etelmm.h>
#include "sysapetelnetworkbargetter.h"
#include "SysAp.hrh"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::NewL() 
// ----------------------------------------------------------------------------
CSysApEtelNetworkBarGetter* CSysApEtelNetworkBarGetter::NewL( MSysApEtelNetworkBarGetterCallback& aCallback, 
                                                              RMobilePhone& aPhone )
    {
    TRACES( RDebug::Print(_L("CSysApEtelNetworkBarGetter::NewL" ) ) );
    CSysApEtelNetworkBarGetter* self = new(ELeave) CSysApEtelNetworkBarGetter( aCallback, aPhone );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::~CSysApEtelNetworkBarGetter() 
// ----------------------------------------------------------------------------

CSysApEtelNetworkBarGetter::~CSysApEtelNetworkBarGetter()
    {
    TRACES( RDebug::Print(_L("CSysApEtelNetworkBarGetter::~CSysApEtelNetworkBarGetter" ) ) );
    Cancel();
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::GetBarCountL
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarGetter::GetBarCountL()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarGetter::GetBarCountL" ) ) );
    iPhone.GetSignalStrength( iStatus, iSignalStrength, iSignalBars );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::RunL
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarGetter::RunL()
    {
    TRACES( RDebug::Print(_L("CSysApEtelNetworkBarGetter::RunL: status=%d, iSignalBars=%d" ), 
            iStatus.Int(), iSignalBars ) );
    
    iCallback.GetBarCountCompleted( iSignalBars );
    }

// -----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::DoCancel
// -----------------------------------------------------------------------------
//
void CSysApEtelNetworkBarGetter::DoCancel()
    {
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarGetter::DoCancel" ) ) );
    if ( IsActive() )
        {
        iPhone.CancelAsyncRequest( EMobilePhoneGetSignalStrength );
        }
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApEtelNetworkBarGetter::ConstructL()
    {    
    TRACES( RDebug::Print( _L("CSysApEtelNetworkBarGetter::ConstructL" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApEtelNetworkBarGetter::CSysApEtelNetworkBarGetter() 
// ----------------------------------------------------------------------------
CSysApEtelNetworkBarGetter::CSysApEtelNetworkBarGetter( MSysApEtelNetworkBarGetterCallback& aCallback, 
                                                        RMobilePhone& aPhone ) 
    : CActive(EPriorityStandard), 
      iCallback( aCallback ),
      iPhone( aPhone )
    {
    CActiveScheduler::Add(this);
    }    

// End of file
