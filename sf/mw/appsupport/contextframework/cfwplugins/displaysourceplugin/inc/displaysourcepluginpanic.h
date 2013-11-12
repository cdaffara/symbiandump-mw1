/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Display Source Plugin panic definitions.
*
*/


#ifndef DISPLAYSOURCEPLUGINPANIC_H
#define DISPLAYSOURCEPLUGINPANIC_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// DATA TYPES

#ifdef _DEBUG

/** Panic category */
_LIT( KPanicCat, "DISPLAYSOURCEPLUGIN" );

/** Panic codes */
enum TDisplaySourcePluginPanicReason
    {
    ERequestAlreadyIssued,
    };

/** Local panic function */
LOCAL_C void Panic( TDisplaySourcePluginPanicReason aReason )
    {
    User::Panic( KPanicCat, aReason );
    }

#endif



#endif // DISPLAYSOURCEPANIC_H
