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
* Description:  CVibraAction class declaration.
 *
*/


#ifndef C_VIBRAACTION_H
#define C_VIBRAACTION_H

#include <hwrmvibra.h>
#include <cfactionplugin.h>

class MVibraActionObserver;

/**
 * Helper class to start vibra.
 */
NONSHARABLE_CLASS( TVibraActionInfo )
    {
public:

    // Constructor
    TVibraActionInfo():
        iDelay( 0 ),
        iDuration( 0 ),
        iIntensity( 0 ),
        iRepeats( 0 ),
        iRepeatInterval( 0 )
            {

            }

public:

    /** Delay before vibra is started */
    TInt iDelay;

    /** Duration of vibra feedback */
    TInt iDuration;

    /** Vibra intensity */
    TInt iIntensity;

    /** How many times vibra is repeated */
    TInt iRepeats;

    /** Vibra repeat interval */
    TInt iRepeatInterval;
    };

/**
 * Vibra action is a wrapper to vibra client.
 * Vibra action can be configured via following parameters:
 * - Delay: Delay when the action notification is received to actually start vibra
 * - Duration: Vibra duration
 * - Intensity: Vibra intensity
 * - Repeats: How many times vibra is repeated with the same parameters
 * - Repeat interval: Interval between repeats
 *
 * @lib -
 * @since S60 4.0
 */
NONSHARABLE_CLASS( CVibraAction ): public CTimer,
public MHWRMVibraObserver
    {
public:

    /**
     * Two phased constructors.
     *
     * @since S60 4.0
     * @param
     * @return CVibraAction*
     */
    static CVibraAction* NewL( MVibraActionObserver& aObserver );
    static CVibraAction* NewLC( MVibraActionObserver& aObserver );

    // Destructor
    ~CVibraAction();

public: // New methods

    /**
     * Starts vibra.
     * If there is ongoing vibra it will be automatically cancelled and
     * new vibra is started. Vibra feedbacks are not cumulative.
     * 
     * @since S60 4.0
     * @param aVibraInfo Vibra information.
     * @return CCFActionPlugIn::TExecutionTime.
     */
    CCFActionPlugIn::TExecutionTime StartVibraL(
        const TVibraActionInfo& aVibraInfo );

    /**
     * Stops ongoing vibra.
     *
     * @since S60 4.0
     * @param None.
     * @return None.
     */
    void StopVibraL();

private: // From base classes

    // @see CActive
    void RunL();

    // @see CActive
    TInt RunError( TInt aError );

    // @see CActive
    void DoCancel();
    
    // @see MHWRMVibraObserver
    void VibraModeChanged( CHWRMVibra::TVibraModeState aStatus );

    // @see MHWRMVibraObserver
    void VibraStatusChanged( CHWRMVibra::TVibraStatus aStatus );

private: // New methods

    // Start vibra
    CCFActionPlugIn::TExecutionTime DoRunVibraL();

    // Stop vibra
    void DoStopVibraL();

    // Converts milliseconds to microseconds
    TTimeIntervalMicroSeconds32 MicroSeconds( TInt aMilliSeconds ) const;

private:

    CVibraAction( MVibraActionObserver& aObserver );
    void ConstructL();

private: // Data

    /** Vibra controller */
    CHWRMVibra* iVibra;

    /** Current vibra mode */
    CHWRMVibra::TVibraModeState iVibraModeState;

    /** Vibra info */
    TVibraActionInfo iInfo;

    /** Repeats info */
    TInt iRepeats;

    /** Observer */
    MVibraActionObserver& iObserver;
    };

#endif
