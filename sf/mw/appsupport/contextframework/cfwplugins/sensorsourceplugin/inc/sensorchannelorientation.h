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
* Description:  CSensorChannelOrientation class declaration.
 *
*/


#ifndef C_SENSORCHANNELORIENTATION_H
#define C_SENSORCHANNELORIENTATION_H

#include <sensrvtypes.h>
#include <sensrvorientationsensor.h>
#include "sensorchannelbase.h"

/**
 * Sensor channel listener for double tap events.
 *
 * @lib -
 * @since S60 4.0
 */
NONSHARABLE_CLASS( CSensorChannelOrientation ): public CSensorChannelBase
    {
public:

    /**
     * Two phased constrcutors.
     *
     * @since S60 4.0
     * @param aListener Channel listener.
     * @param aChannelInfo Channel info related to the opened channel.
     * @return CSensorChannelOrientation*
     */
    static CSensorChannelOrientation* NewL( const TDesC8& aLocation,
        const TDesC8& aVendor,
        MCFContextInterface& aCF );
    static CSensorChannelOrientation* NewLC( const TDesC8& aLocation,
        const TDesC8& aVendor,
        MCFContextInterface& aCF );

    // Destructor
    ~CSensorChannelOrientation();

private: // From base classes

    // @see CSensorChannelBase
    void DefineContextsL();

    // @see CSensorChannelBase
    void HandleDataReceivedL( CSensrvChannel& aChannel,
        TInt aCount,
        TInt aDataLost );
    
    // @see CSensorChannelBase
    void StopListening();
    
private: // New methods

    // Publish current orientation contexts
    void PublishOrientation();

    // Publish current orientation contexts, can leave
    void PublishOrientationL();

private:

    /**
     * Constrcutor.
     * 
     * @since S60 4.0
     * @param aChannelInfo Info related to channel to be opened.
     * @return None.
     */
    CSensorChannelOrientation( MCFContextInterface& aCF );

    // 2nd phase constrcuctor
    void ConstructL( const TDesC8& aLocation, const TDesC8& aVendor );
    
private: // Data

    /** Current orientation */
    TInt iCurrentOrientation;

    /** Previous orientation */
    TInt iPreviousOrientation;
    };

#endif
