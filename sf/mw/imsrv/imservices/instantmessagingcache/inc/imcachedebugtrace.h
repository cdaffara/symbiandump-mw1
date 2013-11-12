/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  logs generation file
*
*/



#ifndef __IMCACHEDEBUGTRACE_H__
#define __IMCACHEDEBUGTRACE_H__

//  INCLUDES
#include  "e32std.h"
#include <flogger.h>
//  DEFINES
// enable logs printing
// into  c:\\logs\\instantmessagingcache\\instantmessagingcache.txt
//#define IMCHACHE_ENABLE_DEBUG_LOGS
#undef IMCHACHE_ENABLE_DEBUG_LOGS

/**   
* Usage of Log MACRO'S
* _LIT( KExample, "Example" );
*  TXT(s) _L(s)	
*  IM_CV_LOGS(TXT("Some text.") );
*  IM_CV_LOGS(TXT("Some text: %d"), 100 );
*  IM_CV_LOGS(TXT("Some text: %S"), &KExample );
*/

_LIT( KTAdaptDebugOutputDir, "imcache" );
_LIT( KTAdaptDebugOutputFileName, "imcache.txt" );
const TInt KTAdaptMaxLogLineLength = 250 ;
#define T_LIT(s) _L(s)
#define TRACE TIMChacheLogger::WriteLog
#define PLUGIN_UNUSED_PARAM(p) (void) p


/**
 * IM cache debug logger.
 */
class TIMChacheLogger
    {
    public: //Logging functions
    	/**
        * WriteLog, write the message into c:\\logs\\instantmessagingcache\\instantmessagingcache.txt
        * need to create imcv folder into c:\\logs
        * @param aFmt, list of messges to print
        */
		static void WriteLog( TRefByValue<const TDesC> aFmt,... );
		
    private: //Prohibited
    	/**
        * construtor
        */
        TIMChacheLogger();
        /**
        * destructor
        */
        ~TIMChacheLogger();
    };

#endif // __IMCACHEDEBUGTRACE_H__



/**
 * Handler used by logger to truncate the string
 * rather than panic in case of buffer overflow.
*/

NONSHARABLE_CLASS ( TAdaptOverflowTruncate ) : public TDes16Overflow
	{

public:
	void Overflow ( TDes16& /*aDes*/ ) {}
	};


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TIMChacheLogger::WriteLog()
// -----------------------------------------------------------------------------
//
inline void TIMChacheLogger::WriteLog ( TRefByValue<const TDesC> aFmt, ... )
	{
	#ifdef IMCHACHE_ENABLE_DEBUG_LOGS
		( void ) aFmt;//Suppress unused formal parameter warning
		TBuf< KTAdaptMaxLogLineLength > buffer;
		buffer.Append ( _L ( "[" ) );           // CSI: 78 #
		buffer.Append ( RThread().Name() );
		buffer.Append ( _L ( "] " ) );          // CSI: 78 #
		TAdaptOverflowTruncate overflow;
		VA_LIST list;
		VA_START ( list, aFmt );
		buffer.AppendFormatList ( aFmt, list, &overflow );
		RFileLogger logger;

		if ( logger.Connect() == KErrNone )
			{
			logger.SetDateAndTime ( ETrue, ETrue );
			logger.CreateLog ( KTAdaptDebugOutputDir, KTAdaptDebugOutputFileName,
			                   EFileLoggingModeAppend );
			logger.Write ( buffer );
			logger.CloseLog();
			logger.Close();
			}
	#endif

	}


// End of File

 
