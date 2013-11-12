/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Message handler base class implementation.
*
*/


#include "CFMessageHandlerBase.h"

CCFMessageHandlerBase::CCFMessageHandlerBase( 
	MCFExtendedContextInterface& aCFContext,
    MCFActionInterface& aCFAction, 
    MCFScriptInterface& aScriptInterface ):
    iCFContext( aCFContext ),
    iCFAction( aCFAction ),
    iCFScriptInterface( aScriptInterface )
    {
    }
