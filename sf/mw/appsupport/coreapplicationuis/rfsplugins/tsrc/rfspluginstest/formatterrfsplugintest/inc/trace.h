/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Tracing definitions for test project
*
*/


#ifndef TRACE_H
#define TRACE_H

#include <e32debug.h> // RDebug

//-----------------------------------------------------------------------------
// Enable / disable traces here
//-----------------------------------------------------------------------------
//

/**
* Error trace enabled
*/
#ifdef _DEBUG
    #define ERROR_TRACE
#else
    #undef ERROR_TRACE
#endif

/**
* Info trace enabled
*/
#if defined _DEBUG
    #define INFO_TRACE
#else
    #undef INFO_TRACE
#endif

/**
* Function trace enabled
*/
#if defined _DEBUG && defined __FUNC_TRACE__
    #define FUNC_TRACE
#else
    #undef FUNC_TRACE
#endif

/**
* Tracing current client process and thread
*/
#ifdef _DEBUG
    #define CLIENT_TRACE
#else
    #undef CLIENT_TRACE
#endif

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
//

/**
* Prefix trace macro to complete tracing with component name.
* Returns TDesC which can be used directly with RDebug or RFileLogger.
*/
#define _PREFIX_TRACE( aMsg ) TPtrC( (const TText*)L"[TEST]: " L##aMsg )

/**
* Prefix error trace
*/
#define _PREFIX_ERROR( aMsg ) _PREFIX_TRACE( "[ERROR: %d]: " L##aMsg )

/**
* Prefix info trace.
*/
#define _PREFIX_INFO( aMsg ) _PREFIX_TRACE( "[INFO]: " L##aMsg )

/**
* Prefix macro for strings
*/
#define _PREFIX_CHAR( aMsg ) (const char*)"[TEST]: " ##aMsg

//-----------------------------------------------------------------------------
// Error trace macros
//-----------------------------------------------------------------------------
//
#ifdef ERROR_TRACE

    /**
    * Error trace definitions.
    */
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

#else//INFO_TRACE not defined

    #define INFO( aMsg )
    #define INFO_1( aMsg, aP1 )
    #define INFO_2( aMsg, aP1, aP2 )
    #define INFO_3( aMsg, aP1, aP2, aP3 )

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
    #define FUNC( aMsg, aP1 )\
        {\
        RDebug::Printf( aMsg, aP1 );\
        }\

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
                    iCanLeave = !iFunc.Right(
                        KFuncLeavePatternL().Length() ).Compare( KFuncLeavePatternL );
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

#ifdef _DEBUG

    static void Panic( const TDesC8& aFileName, const TInt aLineNum )
        {
        TPath name;
        name.Copy( aFileName );
        RDebug::Print( _L( "Assertion failed in file=%S, line=%d" ), &name, aLineNum );
        User::Invariant();
        }

    #define ASSERT_ALWAYS_TRACE Panic( _L8(__FILE__), __LINE__ );
    #define ASSERT_TRACE( _s ) if ( !( _s ) ) { ASSERT_ALWAYS_TRACE; }

#else // _DEBUG

    #define ASSERT_ALWAYS_TRACE
    #define ASSERT_TRACE( _s )

#endif // _DEBUG

#endif
