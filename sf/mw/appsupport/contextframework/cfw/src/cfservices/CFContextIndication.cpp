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
* Description:  CCFContextIndication class implementation.
*
*/


#include "cfcontextindication.h"
#include "CFContextIndicationImpl.h"

EXPORT_C CCFContextIndication* CCFContextIndication::NewL( const TUid& aUid )
    {
    return CCFContextIndicationImpl::NewL( aUid );
    }

EXPORT_C CCFContextIndication* CCFContextIndication::NewLC( const TUid& aUid )
    {
    return CCFContextIndicationImpl::NewLC( aUid );
    }
