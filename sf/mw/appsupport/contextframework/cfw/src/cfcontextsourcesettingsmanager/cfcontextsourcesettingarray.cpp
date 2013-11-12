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
* Description:  CFContextSourceSettingArray class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingarray.h"
#include "cfcontextsourcesettingarrayimpl.h"
#include "cftrace.h"

// MEMBER FUNCTIONS

EXPORT_C CCFContextSourceSettingArray* CCFContextSourceSettingArray::NewL()
    {
    FUNC_LOG;

    return CCFContextSourceSettingArrayImpl::NewL();
    }

EXPORT_C CCFContextSourceSettingArray* CCFContextSourceSettingArray::NewLC()
    {
    FUNC_LOG;

    return CCFContextSourceSettingArrayImpl::NewLC();
    }
