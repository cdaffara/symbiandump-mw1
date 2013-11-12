/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionIndication class implementation.
*
*/


#include "cfactionindication.h"
#include "CFActionIndicationImpl.h"

EXPORT_C CCFActionIndication* CCFActionIndication::NewL()
    {
    return CCFActionIndicationImpl::NewL();
    }

EXPORT_C CCFActionIndication* CCFActionIndication::NewLC()
    {
    return CCFActionIndicationImpl::NewLC();
    }
