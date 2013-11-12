/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Debug Traces.
*
*/

#ifndef __DEBUGTRACE_H__
#define __DEBUGTRACE_H__

//  INCLUDES
#include  "e32std.h"

//  DEFINES
//Undefine: tracing disabled
//Define 1: Traces goes to file log
//Define 2: Traces goes to RDebug output
//Define 3: Trace goes both file and RDebug output
#undef TRACE_MODE
//#define TRACE_MODE 3



// -----------------------------------------------------------------------------
// Trace definitions
// -----------------------------------------------------------------------------
#ifdef TRACE_MODE

    //  INCLUDES
    #include <flogger.h>
    #include <e32svr.h>

    // CONSTANTS
    _LIT( KTraceLogDir, "BS" );
    _LIT( KTraceLogFile, "BS.log" );

     //TRACE INDIRECTION
    #define T_LIT(s) _L(s) // CSI: 78 # See above
    #define TRACE Trace::Emit
    #define IF_TRACE_ON( aStatement ) aStatement
#endif  // TRACE_MODE


//-----------------------------------------------------------------------------
// Empty trace definitions
//-----------------------------------------------------------------------------
#ifndef TRACE_MODE
    struct TEmptyTraceString { };
    #define T_LIT(s) TEmptyTraceString()
    #define IF_TRACE_ON( aStatement )

    inline void TRACE(TEmptyTraceString){}

    template<class T1>
    inline void TRACE(TEmptyTraceString,T1){}

    template<class T1,class T2>
    inline void TRACE(TEmptyTraceString,T1,T2){}

    template<class T1,class T2,class T3>
    inline void TRACE(TEmptyTraceString,T1,T2,T3){}

    template<class T1,class T2,class T3,class T4>
    inline void TRACE(TEmptyTraceString,T1,T2,T3,T4){}

    template<class T1,class T2,class T3,class T4,class T5>
    inline void TRACE(TEmptyTraceString,T1,T2,T3,T4,T5){}

    template<class T1,class T2,class T3,class T4,class T5,class T6>
    inline void TRACE(TEmptyTraceString,T1,T2,T3,T4,T5,T6){}

    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7>
    inline void TRACE(TEmptyTraceString,T1,T2,T3,T4,T5,T6,T7){}

    template<class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
    inline void TRACE(TEmptyTraceString,T1,T2,T3,T4,T5,T6,T7,T8){}

#endif  // TRACE_MODE




// -----------------------------------------------------------------------------
// Trace implementation
// -----------------------------------------------------------------------------
#ifdef TRACE_MODE
    #include <flogger.h>
    #include <e32svr.h>

    // TARGET WARNING
    #if !defined(_DEBUG)

    #if defined(__CW32__)
    #pragma message("Trace logging on.") // CSI: 68 # See above
    #else
    //ARMCC warning
    #endif
    #endif

    namespace Trace
        {
        class TOverflowHandler : public TDes16Overflow
            {
            inline void Overflow( TDes16& /*aDes*/ ) {}
            };

        inline void Emit( TRefByValue<const TDesC> aFmt, ... )
            {
            //Format the log line
            TBuf< 250 > buffer;
            buffer.Append( _L("[") ); // CSI: 78 # See above
            buffer.Append( RThread().Name() );
            buffer.Append( _L("] ") ); // CSI: 78 # See above

            TOverflowHandler overflowHandler;

            VA_LIST list;
            VA_START( list, aFmt );
            buffer.AppendFormatList( aFmt, list, &overflowHandler );

            TInt theTraceMode = TRACE_MODE;

            //Send trace to file
            if( ( theTraceMode == 1 ) ||
                ( theTraceMode == 3 ) )
                {
                RFileLogger logger;
                if( logger.Connect() == KErrNone )
                    {
                    logger.SetDateAndTime( EFalse, ETrue );
                    logger.CreateLog( KTraceLogDir,
                                      KTraceLogFile,
                                      EFileLoggingModeAppend );

                    logger.Write( buffer );
                    logger.CloseLog();
                    logger.Close();
                    }
                }

            //Send trace to default debug output
            if( ( theTraceMode == 2 ) ||
                ( theTraceMode == 3 ) )
                {
                RDebug::Print( _L("%S"), &buffer );
                }
            }
        }
#endif //TRACE_MODE

#endif //__DEBUGTRACE_H__

//  END OF FILE
