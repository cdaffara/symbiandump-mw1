/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApTimer class definition.
*
*/


#ifndef SYSAPTIMER_H
#define SYSAPTIMER_H

// INCLUDES
#include <e32base.h>

// CLASS DECLARATION
/**
*  MSysApTimerResponder
*  
*  @lib   sysap
*  @since 2.6
*/

class MSysApTimerResponder
    {
public:
    virtual void TimerExpiredL() = 0;
    };

// CLASS DECLARATION
/**
*  CSysApTimer
*  
*  @lib   sysap
*  @since 2.6
*/

class CSysApTimer : public CActive
{
public:

    /**
    * Constructor
    */         
    CSysApTimer( MSysApTimerResponder& aSysApTimerResponder );

    /**
    * Destructor.
    */
    virtual ~CSysApTimer();

    /**
    * This method activates the timer.
    * @param TInt aTimeoutInSeconds
    * @return void
    */         
    void ActivateTimerL( TInt aTimeoutInSeconds );
    
    /**
    * This method activates the timer. 
    * Timeout parameter is given in milliseconds.
    *
    * @since S60 3.2
    * @param TInt   aTimeoutInMilliSeconds  timeout in milliseconds
    * @return None
    */         
    void ActivateMsTimerL( TInt aTimeoutInMilliSeconds );
    
    /**
    * From CActive. Cancels the active object from observing the list query
    * @param  none
    * @return void
    */
    void DoCancel();
    /**
    * From CActive. Gets called by active object when a selection is done.
    * @param  none
    * @return void
    */
    void RunL();

    /**
    * From CActive. Gets called by active object when leave happens inside RunL
    * @param none
    * @return void
    */
    TInt RunError( TInt aError );

private:

    // Prohibit default constructor
    CSysApTimer();         

    // Prohibit copy constructor
    CSysApTimer( const CSysApTimer& );
    
    // Prohibit assigment operator
    CSysApTimer& operator= ( const CSysApTimer& );

private:

    MSysApTimerResponder&   iTimerResponder;
    RTimer                  iTimer;
};

#endif // SYSAPTIMER_H

// End of File
