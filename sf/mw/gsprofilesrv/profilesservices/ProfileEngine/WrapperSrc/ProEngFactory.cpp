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
* Description:  Implementation of the ProEngFactory.
*
*/



// INCLUDE FILES
#include    <ProEngFactory.h>
#include    "CProEngEngineImpl.h"
#include    "CProEngNotifyHandlerImpl.h"
#include    "CProEngAlertToneSeekerImpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ProEngFactory::NewEngineL
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngEngine* ProEngFactory::NewEngineL()
    {
    return CProEngEngineImpl::NewL();
    }

// -----------------------------------------------------------------------------
// ProEngFactory::NewEngineLC
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngEngine* ProEngFactory::NewEngineLC()
    {
    return CProEngEngineImpl::NewLC();
    }

// -----------------------------------------------------------------------------
// ProEngFactory::NewEngineL
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngEngine* ProEngFactory::NewEngineL( RFs& aFs )
    {
    return CProEngEngineImpl::NewL( aFs );
    }

// -----------------------------------------------------------------------------
// ProEngFactory::NewEngineLC
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngEngine* ProEngFactory::NewEngineLC( RFs& aFs )
    {
    return CProEngEngineImpl::NewLC( aFs );
    }

// -----------------------------------------------------------------------------
// ProEngFactory::NewNotifyHandlerL
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngNotifyHandler* ProEngFactory::NewNotifyHandlerL()
    {
    return CProEngNotifyHandlerImpl::NewL();
    }

// -----------------------------------------------------------------------------
// ProEngFactory::NewAlertToneSeekerL
// -----------------------------------------------------------------------------
//
EXPORT_C MProEngAlertToneSeeker* ProEngFactory::NewAlertToneSeekerL()
    {
    return CProEngAlertToneSeekerImpl::NewL();
    }

//  End of File

