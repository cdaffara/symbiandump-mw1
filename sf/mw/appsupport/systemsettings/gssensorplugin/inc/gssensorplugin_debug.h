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
* Description:  Macro definitions for tracing and debugging purposes.
*
*/


#ifndef GSSENSORPLUGIN_DEBUG_H
#define GSSENSORPLUGIN_DEBUG_H

#ifdef _DEBUG

#include <e32debug.h> // RDebug

// -----------------------------------------------------------------------------
    // COMPONENT_TRACE_FLAG
    // -----------------------------------------------------------------------------
    #ifdef COMPONENT_TRACE_FLAG

        #define TRACE_( AAA ) do\
            { _LIT( logStr, AAA ); RDebug::Print( logStr ); } while ( 0 )

        #define TRACE_1( AAA, BBB ) do\
             { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB ); } while ( 0 )

        #define TRACE_2( AAA, BBB, CCC ) do\
             { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB, CCC ); } while ( 0 )

        #define TRACE_3( AAA, BBB, CCC, DDD ) do\
             { _LIT( logStr, AAA ); RDebug::Print( logStr, BBB, CCC, DDD ); } while ( 0 )

    #else

        #define TRACE_( AAA )
        #define TRACE_1( AAA, BBB )
        #define TRACE_2( AAA, BBB, CCC )
        #define TRACE_3( AAA, BBB, CCC, DDD )
        
    #endif // COMPONENT_TRACE_FLAG

#else // _DEBUG

    #define TRACE_( AAA )
    #define TRACE_1( AAA, BBB )
    #define TRACE_2( AAA, BBB, CCC )
    #define TRACE_3( AAA, BBB, CCC, DDD )

#endif // _DEBUG

#endif // GSSENSORPLUGIN_DEBUG_H