/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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




#ifndef DEBUG_H_
#define DEBUG_H_

#include <e32debug.h>

#ifdef _DEBUG
#define TRACE_FUNC RDebug::Printf("\t%s[%x]", __PRETTY_FUNCTION__, iInstance);
#define TRACE_FUNC_ENTRY RDebug::Printf("\t+%s[%x]", __PRETTY_FUNCTION__, iInstance);
#define TRACE_FUNC_EXIT RDebug::Printf("\t-%s[%x]", __PRETTY_FUNCTION__, iInstance);
#else
#define TRACE_FUNC
#define TRACE_FUNC_ENTRY 
#define TRACE_FUNC_EXIT
#endif

#endif /* DEBUG_H_ */
