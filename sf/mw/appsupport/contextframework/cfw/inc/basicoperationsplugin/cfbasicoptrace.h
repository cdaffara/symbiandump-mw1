/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Trace definitions.
*
*/


#ifndef CFBASICOPTRACE_H
#define CFBASICOPTRACE_H

#include "cftrace.h"


//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
//


/**
* DOM prefix info trace.
*/
#define _DOM_PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[DOM_INFO]: %S : " L##aMsg )
#define _ACTION_DOM_PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[INFO]: %S : " L##aMsg )

/**
* Re-define info trace prefix used by BasicOperationsPlugIn.
*/
#undef _PREFIX_INFO
#define _PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[DOM_INFO]: " L##aMsg )

/**
* Prefix action info trace.
*/
#define _ACTION_PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[INFO]: " L##aMsg )

//-----------------------------------------------------------------------------
// DOM info trace macro
//-----------------------------------------------------------------------------
//
#ifdef DOM_INFO_TRACE
    /**
    * DOM info creation.
    */
    #define CREATE_DOM_INFO( aSelf, aNode )\
        {\
        if ( aSelf != NULL )\
            {\
            aSelf->GenerateDomTreeLocationInfoL( aNode );\
            }\
        }

    /**
    * DOM info log message definitions.
    */
    #ifdef TRACE_INTO_FILE
    
        #define DOM_INFO( aMsg )\
            {\
            if ( iDomTreeLocationInfo != NULL )\
                {\
                TPtrC info( *iDomTreeLocationInfo );\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _DOM_PREFIX_INFO( aMsg ), &info );\
                }\
            }
        #define ACTION_DOM_INFO( aMsg )\
            {\
            if ( iDomTreeLocationInfo != NULL )\
                {\
                TPtrC info( *iDomTreeLocationInfo );\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _ACTION_DOM_PREFIX_INFO( aMsg ), &info );\
                }\
            }

    #else//TRACE_INTO_FILE not defined

        #define DOM_INFO( aMsg )\
            {\
            if ( iDomTreeLocationInfo != NULL )\
                {\
                TPtrC info( *iDomTreeLocationInfo );\
                RDebug::Print( _DOM_PREFIX_INFO( aMsg ), &info );\
                }\
            }
        #define ACTION_DOM_INFO( aMsg )\
            {\
            if ( iDomTreeLocationInfo != NULL )\
                {\
                TPtrC info( *iDomTreeLocationInfo );\
                RDebug::Print( _ACTION_DOM_PREFIX_INFO( aMsg ), &info );\
                }\
            }

    #endif // TRACE_INTO_FILE

#else // DOM_INFO_TRACE not defined
    #define CREATE_DOM_INFO( aSelf, aNode )
    #define DOM_INFO( aMsg )
    #define ACTION_DOM_INFO( aMsg )
#endif // DOM_INFO_TRACE


//-----------------------------------------------------------------------------
// Action info trace macros
//-----------------------------------------------------------------------------
//
#ifdef INFO_TRACE

    /**
    * Action info log message definitions.
    * Create normal INFO trace messages that need to be generated from action
    * firing functions.
    */
    #ifdef TRACE_INTO_FILE
    
        #define ACTION_INFO( aMsg )\
            {\
            RFileLogger::Write( KDir, KFile, EFileLoggingModeAppend, _ACTION_PREFIX_INFO( aMsg ) );\
            }
        #define ACTION_INFO_1( aMsg, aP1 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _ACTION_PREFIX_INFO( aMsg ), aP1 );\
            }
        #define ACTION_INFO_2( aMsg, aP1, aP2 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _ACTION_PREFIX_INFO( aMsg ), aP1, aP2 );\
            }
        #define ACTION_INFO_3( aMsg, aP1, aP2, aP3 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _ACTION_PREFIX_INFO( aMsg ), aP1, aP2, aP3 );\
            }
        #define ACTION_INFO_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _ACTION_PREFIX_INFO( aMsg ), aP1, aP2, aP3, aP4 );\
            }

    #else//TRACE_INTO_FILE not defined

        #define ACTION_INFO( aMsg )\
            {\
            RDebug::Print( _ACTION_PREFIX_INFO( aMsg ) );\
            }
        #define ACTION_INFO_1( aMsg, aP1 )\
            {\
            RDebug::Print( _ACTION_PREFIX_INFO( aMsg ), aP1 );\
            }
        #define ACTION_INFO_2( aMsg, aP1, aP2 )\
            {\
            RDebug::Print( _ACTION_PREFIX_INFO( aMsg ), aP1, aP2 );\
            }
        #define ACTION_INFO_3( aMsg, aP1, aP2, aP3 )\
            {\
            RDebug::Print( _ACTION_PREFIX_INFO( aMsg ), aP1, aP2, aP3 );\
            }
        #define ACTION_INFO_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            RDebug::Print( _ACTION_PREFIX_INFO( aMsg ), aP1, aP2, aP3, aP4 );\
            }

    #endif//TRACE_INTO_FILE
        
#else//INFO_TRACE not defined

    #define ACTION_INFO( aMsg )
    #define ACTION_INFO_1( aMsg, aP1 )
    #define ACTION_INFO_2( aMsg, aP1, aP2 )
    #define ACTION_INFO_3( aMsg, aP1, aP2, aP3 )
    #define ACTION_INFO_4( aMsg, aP1, aP2, aP3, aP4 )

#endif//INFO_TRACE


#endif // CFBASICOPTRACE_H
