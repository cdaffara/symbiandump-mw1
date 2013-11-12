/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFDelay class declaration.
*
*/



#ifndef C_CFDELAY_H
#define C_CFDELAY_H

#include <e32base.h>
#include <cfcontextobject.h>

#include "cfdelayexpired.h"


/**
* Delays the call to expiration interface.
*
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFDelay ) : public CTimer
    {
public:

    /**
    * Two phased constructors.
    *
    * @since S60 5.0
    * @param
    * @return CCFDelay*
    */
    static CCFDelay* NewL( MCFDelayExpired& aDelayer );
    static CCFDelay* NewLC( MCFDelayExpired& aDelayer );

    // Destructor
    ~CCFDelay();

public: // New methods

    /**
    * Starts the delay.
    * If there is an ongoing delay it will be automatically cancelled and
    * new delay is started.
    *
    * @since S60 5.0
    * @param aDelay defines the delay in microseconds.
    * @return None.
    */
    void Delay( const TTimeIntervalMicroSeconds32& aDelay );

private: // From base classes

    // @see CActive
    void RunL();

    // @see CActive
    TInt RunError( TInt aError );

private:

    CCFDelay( MCFDelayExpired& aDelayer );
    void ConstructL();

private: // Data

    /** Interface to be called when the delay has expired. */
    MCFDelayExpired& iDelayer;

    /** Stores the required delay. */
    TInt iDelay;

    /** Stores the time when delay was started. */
    TTime iDelayStartTime;
    };

#endif // C_CFDELAY_H
