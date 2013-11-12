/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProEngObserverBase
*
*/



// INCLUDE FILES
#include    "CProEngObserverBase.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngObserverBase::CProEngObserverBase
// -----------------------------------------------------------------------------
//
CProEngObserverBase::CProEngObserverBase( TUint32 aKey )
    : CActive( EPriorityStandard ), iKey( aKey )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CProEngObserverBase::
// -----------------------------------------------------------------------------
//
void CProEngObserverBase::RunL()
    {
    TInt error( RequestNotification() );

    if( !error )
        {
        TRAP( error, NotifyObserverL() );
        }

    if( error )
        {
        NotifyError( error );
        }
    }

//  End of File

