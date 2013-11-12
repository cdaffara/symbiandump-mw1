/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPublishContextAction class declaration.
*
*/



#include "cfpublishcontextaction.h"
#include "cfextendedcontextinterface.h"
#include "cfbasicoptrace.h"

#include <cfoperationservices.h>
#include <cfcontextobject.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCFPublishContextAction::CCFPublishContextAction
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CCFPublishContextAction::CCFPublishContextAction( MCFOperationServices& aServices,
    MCFExtendedContextInterface& aCF,
    CCFContextObject* aContextObject,
    TBool aAutoDefine ): CCFScriptAction( aServices, CCFScriptAction::EPublishContext ),
    iCF( aCF ),
    iContextObject( aContextObject ),
    iAutoDefine( aAutoDefine )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFPublishContextAction::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCFPublishContextAction::ConstructL()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CCFPublishContextAction::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFPublishContextAction* CCFPublishContextAction::NewL(
    MCFOperationServices& aServices,
    CCFContextObject* aContextObject,
    TBool aAutoDefine )
    {
    FUNC_LOG;

    CCFPublishContextAction* self  = CCFPublishContextAction::NewLC(
        aServices, aContextObject, aAutoDefine );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CCFPublishContextAction::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCFPublishContextAction* CCFPublishContextAction::NewLC(
    MCFOperationServices& aServices,
    CCFContextObject* aContextObject,
    TBool aAutoDefine )
    {
    FUNC_LOG;

    CCFPublishContextAction* self = new( ELeave ) CCFPublishContextAction(
        aServices,
        ( MCFExtendedContextInterface& ) aServices.ContextInterface(),
        aContextObject,
        aAutoDefine );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CCFPublishContextAction::~CCFPublishContextAction()
    {
    FUNC_LOG;

    delete iContextObject;
    }


// -----------------------------------------------------------------------------
// CCFPublishContextAction::ActL
// -----------------------------------------------------------------------------
//
void CCFPublishContextAction::ActL()
    {
    FUNC_LOG;

    TInt err = iCF.PublishContextFromAction( *iContextObject, iAutoDefine );
    ERROR_2( err, "Failed to publish context as script action: %S, %S",
            &iContextObject->Type(),
            &iContextObject->Value() );
    }

// -----------------------------------------------------------------------------
// CCFPublishContextAction::GetSecurityPolicy
// -----------------------------------------------------------------------------
//
TInt CCFPublishContextAction::GetSecurityPolicy( TSecurityPolicy& aPolicy )
	{
    FUNC_LOG;

	return iCF.GetSecurityPolicy( iContextObject->Source(),
            iContextObject->Type(),
            aPolicy );
	}
