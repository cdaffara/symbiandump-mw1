/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include "tsdataobservertask.h"
#include "tstaskmonitorglobals.h"
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsDataObserverTask::CTsDataObserverTask( MTsDataObserverStorage& aStorage, 
                                          const RMessage2& aMsg )
:
    iStorage( aStorage ),
    iMsg( aMsg )
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsDataObserverTask::~CTsDataObserverTask()
    {
    if( EFalse == iMsg.IsNull() )
        {
        iMsg.Complete( iDataChanged ? KErrNone : KErrCancel );
        }
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsDataObserverTask::ExecuteLD( MTsDataObserverStorage& aStorage,
                                     const RMessage2& aMsg )
    {
    CTsDataObserver *self = new (ELeave)CTsDataObserverTask( aStorage, aMsg );
    CleanupStack::PushL( self );
    aStorage.PushL( self );
    CleanupStack::Pop( self );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsDataObserverTask::DataChanged()
    {
    iDataChanged = ETrue;
    iStorage.Pop( this );
    delete this; 
    }
