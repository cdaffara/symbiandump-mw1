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
* Description:   CNcdEntityRefParser implementation
*
*/


#include "ncd_pp_purchaseimpl.h"
#include "ncdprotocolutils.h"

CNcdPreminetProtocolPurchaseImpl* 
CNcdPreminetProtocolPurchaseImpl::NewL() 
    {
    CNcdPreminetProtocolPurchaseImpl* self =
        new (ELeave) CNcdPreminetProtocolPurchaseImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
CNcdPreminetProtocolPurchaseImpl* 
CNcdPreminetProtocolPurchaseImpl::NewLC() 
    {
    CNcdPreminetProtocolPurchaseImpl* self =
        new (ELeave) CNcdPreminetProtocolPurchaseImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
void CNcdPreminetProtocolPurchaseImpl::ConstructL()
    {
    }

CNcdPreminetProtocolPurchaseImpl::~CNcdPreminetProtocolPurchaseImpl()
    {
    }

