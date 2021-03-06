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
* Description:  Context Framework trace definitions.
*
*/


#ifndef CFTRACE_H
#define CFTRACE_H

#include "cftraceconfiguration.hrh"

#ifdef TRACE_INTO_FILE
#include <flogger.h> // RFileLogger
#else
#include <e32debug.h> // RDebug
#endif

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
//

/**
* Prefix trace macro to complete tracing with component name.
* Returns TDesC which can be used directly with RDebug or RFileLogger.
*/
#define _PREFIX_TRACE( aMsg ) TPtrC( (const TText*)L"[ContextFw]: " L##aMsg )

/**
* Prefix error trace
*/
#define _PREFIX_ERROR( aMsg ) _PREFIX_TRACE( "[ERROR: %d]: " L##aMsg )

/**
* Prefix info trace.
*/
#define _PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[INFO]: " L##aMsg )

/**
* Prefix heap trace.
*/
#define _PREFIX_HEAP( aMsg ) _PREFIX_TRACE( "[HEAP: %d bytes used]: " L##aMsg )

/**
* Prefix macro for strings
*/
#define _PREFIX_CHAR( aMsg ) (const char*)"[ContextFw]: " ##aMsg

/**
* Define needed directories if TRACE_INTO_FILE macro in use
*/
#ifdef TRACE_INTO_FILE

    _LIT( KDir, "contextframework" );
    _LIT( KFile, "cf_log.txt" );
    _LIT( KFullPath, "c:\\logs\\contextframework\\" );

#endif

//-----------------------------------------------------------------------------
// Error trace macros
//-----------------------------------------------------------------------------
//
#ifdef ERROR_TRACE

    /**
    * Error trace definitions.
    */
    #ifdef TRACE_INTO_FILE

        #define ERROR( aErr, aMsg )\
            {\
            if( aErr < KErrNone )\
                {\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_ERROR( aMsg ), aErr );\
                }\
            }
        #define ERROR_1( aErr, aMsg, aP1 )\
            {\
            if( aErr < KErrNone )\
                {\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_ERROR( aMsg ), aErr, aP1 );\
                }\
            }
        #define ERROR_2( aErr, aMsg, aP1, aP2 )\
            {\
            if( aErr < KErrNone )\
                {\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_ERROR( aMsg ), aErr, aP1, aP2 );\
                }\
            }
        #define ERROR_3( aErr, aMsg, aP1, aP2, aP3 )\
            {\
            if( aErr < KErrNone )\
                {\
                RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_ERROR( aMsg ), aErr, aP1, aP2, aP3 );\
                }\
            }
            
    #else//TRACE_INTO_FILE not defined
    
        #define ERROR( aErr, aMsg )\
            {\
            if( aErr < KErrNone )\
                {\
                RDebug::Print( _PREFIX_ERROR( aMsg ), aErr );\
                }\
            }
        #define ERROR_1( aErr, aMsg, aP1 )\
            {\
            if( aErr < KErrNone )\
                {\
                RDebug::Print( _PREFIX_ERROR( aMsg ), aErr, aP1 );\
                }\
            }
        #define ERROR_2( aErr, aMsg, aP1, aP2 )\
            {\
            if( aErr < KErrNone )\
                {\
                RDebug::Print( _PREFIX_ERROR( aMsg ), aErr, aP1, aP2 );\
                }\
            }
        #define ERROR_3( aErr, aMsg, aP1, aP2, aP3 )\
            {\
            if( aErr < KErrNone )\
                {\
                RDebug::Print( _PREFIX_ERROR( aMsg ), aErr, aP1, aP2, aP3 );\
                }\
            }
    
    #endif//TRACE_INTO_FILE

    #define ERROR_GEN( aMsg ) ERROR( KErrGeneral, aMsg )
    #define ERROR_GEN_1( aMsg, aP1 ) ERROR_1( KErrGeneral, aMsg, aP1 )
    #define ERROR_GEN_2( aMsg, aP1, aP2 ) ERROR_2( KErrGeneral, aMsg, aP1, aP2 )
    #define ERROR_GEN_3( aMsg, aP1, aP2, aP3 ) ERROR_3( KErrGeneral, aMsg, aP1, aP3 )

#else//ERROR_TRACE not defined

    #define ERROR( aErr, aMsg )
    #define ERROR_1( aErr, aMsg, aP1 )
    #define ERROR_2( aErr, aMsg, aP1, aP2 )
    #define ERROR_3( aErr, aMsg, aP1, aP2, aP3 )
    #define ERROR_GEN( aMsg )
    #define ERROR_GEN_1( aMsg, aP1 )
    #define ERROR_GEN_2( aMsg, aP1, aP2 )
    #define ERROR_GEN_3( aMsg, aP1, aP2, aP3 )

#endif//ERROR_TRACE

//-----------------------------------------------------------------------------
// Info trace macros
//-----------------------------------------------------------------------------
//
#ifdef INFO_TRACE

    /**
    * Info log message definitions.
    */
    #ifdef TRACE_INTO_FILE
    
        #define INFO( aMsg )\
            {\
            RFileLogger::Write( KDir, KFile, EFileLoggingModeAppend, _PREFIX_INFO( aMsg ) );\
            }
        #define INFO_1( aMsg, aP1 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_INFO( aMsg ), aP1 );\
            }
        #define INFO_2( aMsg, aP1, aP2 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_INFO( aMsg ), aP1, aP2 );\
            }
        #define INFO_3( aMsg, aP1, aP2, aP3 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_INFO( aMsg ), aP1, aP2, aP3 );\
            }
        #define INFO_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_INFO( aMsg ), aP1, aP2, aP3, aP4 );\
            }

    #else//TRACE_INTO_FILE not defined

        #define INFO( aMsg )\
            {\
            RDebug::Print( _PREFIX_INFO( aMsg ) );\
            }
        #define INFO_1( aMsg, aP1 )\
            {\
            RDebug::Print( _PREFIX_INFO( aMsg ), aP1 );\
            }
        #define INFO_2( aMsg, aP1, aP2 )\
            {\
            RDebug::Print( _PREFIX_INFO( aMsg ), aP1, aP2 );\
            }
        #define INFO_3( aMsg, aP1, aP2, aP3 )\
            {\
            RDebug::Print( _PREFIX_INFO( aMsg ), aP1, aP2, aP3 );\
            }
        #define INFO_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            RDebug::Print( _PREFIX_INFO( aMsg ), aP1, aP2, aP3, aP4 );\
            }

    #endif//TRACE_INTO_FILE
        
#else//INFO_TRACE not defined

    #define INFO( aMsg )
    #define INFO_1( aMsg, aP1 )
    #define INFO_2( aMsg, aP1, aP2 )
    #define INFO_3( aMsg, aP1, aP2, aP3 )
    #define INFO_4( aMsg, aP1, aP2, aP3, aP4 )

#endif//INFO_TRACE

//-----------------------------------------------------------------------------
// Trace current client thread name and process id
//-----------------------------------------------------------------------------
//
#ifdef CLIENT_TRACE

    #define CLIENT( aMessage )\
        {\
        RThread thread;\
        TInt err = aMessage.Client( thread );\
        if( err == KErrNone )\
            {\
            RProcess process;\
            err = thread.Process( process );\
            if( err == KErrNone )\
                {\
                TPtrC thredName( thread.Name() );\
                TUid processUid( process.SecureId() );\
                INFO_2( "Current client process UID: [%x], thread name: [%S]",\
                    processUid,\
                    &thredName );\
                }\
            process.Close();\
            }\
        thread.Close();\
        }

#else

    #define CLIENT( aMessage )

#endif

//-----------------------------------------------------------------------------
// Function trace macros
//-----------------------------------------------------------------------------
//
#ifdef FUNC_TRACE

    /**
    * Function logging definitions.
    */
    #ifdef TRACE_INTO_FILE
    
        #define FUNC( aMsg, aP1 )\
            {\
            TPtrC8 trace( _S8( aMsg ) );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, trace, aP1 );\
            }\
    
    #else//TRACE_INTO_FILE not defined
    
        #define FUNC( aMsg, aP1 )\
            {\
            RDebug::Printf( aMsg, aP1 );\
            }\
    
    #endif//TRACE_INTO_FILE
        
    /**
    * Function trace helper class.
    * 
    * NOTE:
    * LC -methods cannot be trapped. Therefore if LC -method leaves
    * END trace is used instead of LEAVE trace.
    * If you have an idea how to round this problem please tell.
    */
    _LIT8( KFuncNameTerminator, "(" );
    _LIT8( KFuncLeavePatternL, "L" );
    class TFuncLog
        {
        public:
            static void Cleanup( TAny* aPtr )
                {
                TFuncLog* self = static_cast< TFuncLog* >( aPtr );
                self->iLeft = ETrue;
                FUNC( _PREFIX_CHAR("%S-LEAVE"), &self->iFunc ); // Leave detected
                }
            inline TFuncLog( const char* aFunc ) :
                    iFunc( aFunc ? _S8( aFunc ) : _S8("") ),
                    iLeft( EFalse ),
                    iCleanupItem( Cleanup, this ),
                    iCanLeave( EFalse )
                {
                TInt pos( iFunc.Find( KFuncNameTerminator ) );
                if( pos != KErrNotFound )
                    {
                    iFunc.Set( iFunc.Left( pos ) );
                    iCanLeave = !iFunc.Right( KFuncLeavePatternL().Length() ).Compare( KFuncLeavePatternL );
                    if ( iCanLeave )
                        {
                        CleanupStack::PushL( iCleanupItem ); // Ignore warnings
                        }
                    }
                FUNC( _PREFIX_CHAR("%S-START"), &iFunc );
                }

            inline ~TFuncLog()
                {
                if ( !iLeft )
                    {
                    if ( iCanLeave )
                        {
                        CleanupStack::Pop( this ); // Pop the cleanup item
                        }
                    FUNC( _PREFIX_CHAR("%S-END"), &iFunc ); // Normally finished
                    }
                }

        private: // Data
            TPtrC8 iFunc;
            TBool iLeft;
            TCleanupItem iCleanupItem;
            TBool iCanLeave;
        };
    #define FUNC_LOG TFuncLog _fl( __PRETTY_FUNCTION__ );
    
#else//FUNC_TRACE not defined

    #define FUNC_LOG

#endif//FUNC_TRACE

//-----------------------------------------------------------------------------
// Timestamp trace macros
//-----------------------------------------------------------------------------
//
#ifdef TIMESTAMP_TRACE

    #ifdef TRACE_INTO_FILE
    
        #define TIMESTAMP( aCaption )\
            {\
            TTime t;\
            t.HomeTime();\
            TDateTime dt = t.DateTime();\
            _LIT( KCaption, aCaption );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend,\
                _PREFIX_TRACE("[TIMESTAMP] %S %d:%02d:%02d.%d us"),\
                    &KCaption, dt.Hour(), dt.Minute(), dt.Second(), dt.MicroSecond() );\
            }

    #else//TRACE_INTO_FILE not defined
    
        #define TIMESTAMP( aCaption )\
            {\
            TTime t;\
            t.HomeTime();\
            TDateTime dt = t.DateTime();\
            _LIT( KCaption, aCaption );\
            RDebug::Print( _PREFIX_TRACE("[TIMESTAMP] %S %d:%02d:%02d.%d us"),\
                &KCaption, dt.Hour(), dt.Minute(), dt.Second(), dt.MicroSecond() );\
            }

    #endif//TRACE_INTO_FILE

#else//TIMESTAMP_TRACE not defined

    #define TIMESTAMP( aCaption )

#endif//TIMESTAMP_TRACE

#ifdef HEAP_TRACE

    #ifdef TRACE_INTO_FILE

        #define HEAP( aMsg )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_HEAP( aMsg ), totalAllocSpace );\
            }
        #define HEAP_1( aMsg, aP1 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1 );\
            }
        #define HEAP_2( aMsg, aP1, aP2 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2 );\
            }
        #define HEAP_3( aMsg, aP1, aP2, aP3 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2, aP3 );\
            }
        #define HEAP_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RFileLogger::WriteFormat( KDir, KFile, EFileLoggingModeAppend, _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2, aP3, aP4 );\
            }

    #else//TRACE_INTO_FILE not defined

        #define HEAP( aMsg )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RDebug::Print( _PREFIX_HEAP( aMsg ), totalAllocSpace );\
            }
        #define HEAP_1( aMsg, aP1 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RDebug::Print( _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1 );\
            }
        #define HEAP_2( aMsg, aP1, aP2 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RDebug::Print( _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2 );\
            }
        #define HEAP_3( aMsg, aP1, aP2, aP3 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RDebug::Print( _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2, aP3 );\
            }
        #define HEAP_4( aMsg, aP1, aP2, aP3, aP4 )\
            {\
            TInt totalAllocSpace = 0;\
            User::AllocSize( totalAllocSpace );\
            RDebug::Print( _PREFIX_HEAP( aMsg ), totalAllocSpace, aP1, aP2, aP3, aP4 );\
            }

    #endif//TRACE_INTO_FILE

#else//HEAP_TRACE not defined

    #define HEAP( aMsg )
    #define HEAP_1( aMsg, aP1 )
    #define HEAP_2( aMsg, aP1, aP2 )
    #define HEAP_3( aMsg, aP1, aP2, aP3 )
    #define HEAP_4( aMsg, aP1, aP2, aP3, aP4 )

#endif//HEAP_TRACE
        
#endif
