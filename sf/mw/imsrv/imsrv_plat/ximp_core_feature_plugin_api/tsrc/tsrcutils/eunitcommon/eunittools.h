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
* Description:
*
*/

#ifndef EUNITTOOLS_H
#define EUNITTOOLS_H

#include <e32std.h>
#include <eunitmacros.h>
#include <ceunittestcasedecorator.h>
#include <ceunitalloctestcasedecorator.h>
#include <ceunitemptytestcasedecorator.h>
//#include "cglobalwaitingnote.h"



// -----------------------------------------------------------------------------
// ASSERTION HELPER
// Leaves if the provided error code is decorator error.
// -----------------------------------------------------------------------------
//
#define LEAVE_IF_DECORATOR_ERROR( aErr ) \
    { \
    if( aErr == KErrNoMemory ) \
        { \
        User::Leave( KErrNoMemory ); \
        } \
    }



// -----------------------------------------------------------------------------
// ASSERTION HELPER
// Returns from current function if the provided error code is decorator error.
// -----------------------------------------------------------------------------
//
#define RETURN_IF_DECORATOR_ERROR( aErr ) \
    { \
    if( aErr == KErrNoMemory ) \
        { \
        return KErrNoMemory; \
        } \
    }



// -----------------------------------------------------------------------------
// MEMORY LEAK TRACKING HELPER
// Performs __UHEAP_MARK and shows visual notify.
// (Visual notify gives time to reset HookLogger heap checks.)
// -----------------------------------------------------------------------------
//
#define __UHEAP_MARK_WITH_VISUAL_NOTIFY \
    { \
    __UHEAP_MARK; \
    EUnitTools::NotifyFromTestEnterL(); \
    }


// -----------------------------------------------------------------------------
// MEMORY LEAK TRACKING HELPER
// Performs __UHEAP_MARKEND with JIT debugger on.
// -----------------------------------------------------------------------------
//
#define __UHEAP_MARKEND_WITH_DEBUGGER \
    { \
    TBool __justInTime = User::JustInTime(); \
    User::SetJustInTime( ETrue ); \
    __UHEAP_MARKEND; \
    User::SetJustInTime( __justInTime ); \
    }





// -----------------------------------------------------------------------------
// TEST CASE DECLARATION
// Defines not decorated test case.
// -----------------------------------------------------------------------------
//
#define PRFW_NOT_DECORATED_TEST( aDescription, aClass, aMethods, aCategory, aSetupMethod, aTestMethod, aTeardownMethod )\
EUNIT_NOT_DECORATED_TEST( aDescription, aClass, aMethods, aCategory, aSetupMethod, aTestMethod, aTeardownMethod )

// -----------------------------------------------------------------------------
// TEST CASE DECLARATION
// Defines decorated test case.
// -----------------------------------------------------------------------------
//
#define PRFW_DECORATED_TEST( aDescription, aClass, aMethods, aCategory, aSetupMethod, aTestMethod, aTeardownMethod )\
EUNIT_TEST( aDescription, aClass, aMethods, aCategory, aSetupMethod, aTestMethod, aTeardownMethod )

#define PRFW_ASSERT_DESC( aCondition, aDescription ) \
    if ( aCondition ) { } else { \
        TEUnitAssertionInfo __eunit_assert_info = { \
			__EUNIT_INTERNAL_ASSERT_BASE( aCondition, aDescription ), \
        	NULL }; \
        EUnit::AssertionFailedL( __eunit_assert_info ); }


/**
 * Collection of EUnit Tools
 */
class EUnitTools
    {
    public:
        static inline void NotifyFromTestEnterL();

    };



inline void EUnitTools::NotifyFromTestEnterL()
    {
    __UHEAP_MARK;

    TInt decoratorfailCount = -1;
    CEUnitTestCaseDecorator* decorator = CEUnitTestCaseDecorator::ActiveTestCaseDecorator( KNullDesC ); //Any decorator
    if( decorator )
        {
        decorator->GetParameter( _L("ErrorCount"), //Run loop counter value
                                 decoratorfailCount );
        }

    if( !decorator ||
        ( decoratorfailCount == 1 ) )
        {
        EUNIT_DISABLE_ANY_DECORATOR;

        TFileName processImage = RProcess().FileName();
        TBuf<350> msg;
        msg.Append( _L("Entering to test case. Test process:") );
        msg.Append( processImage );

     //   CGlobalWaitingNote::ShowMsgL( msg, 15000000, 15 );  //15 seconds, 1 second steps

        EUNIT_ENABLE_ANY_DECORATOR;
        }
    }



#endif // EUNITTOOLS_H



//  end of file




