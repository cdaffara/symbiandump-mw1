/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorChannelProximity class declaration.
*
*/



#ifndef C_SENSORCHANNELPROXIMITY_H
#define C_SENSORCHANNELPROXIMITY_H

#include <sensrvtypes.h>
#include "sensorchannelbase.h"

/**
* Sensor channel listener for double tap events.
*
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( CSensorChannelProximity ): public CSensorChannelBase
    {
    public:

        /**
        * Two phased constrcutors.
        *
        * @since S60 5.0
        * @param aListener Channel listener.
        * @param aChannelInfo Channel info related to the opened channel.
        * @return CSensorChannelProximity*
        */
        static CSensorChannelProximity* NewL( const TDesC8& aLocation,
            const TDesC8& aVendor,
            MCFContextInterface& aCF );
        static CSensorChannelProximity* NewLC( const TDesC8& aLocation,
            const TDesC8& aVendor,
            MCFContextInterface& aCF );
    
        // Destructor
        ~CSensorChannelProximity();
        
    private: // From base classes
    
        // @see CSensorChannelBase
        void DefineContextsL();
    
        // @see CSensorChannelBase
        void HandleDataReceivedL( CSensrvChannel& aChannel, 
            TInt aCount, 
            TInt aDataLost );
        
    private:
    
        /**
        * Constrcutor.
        * 
        * @since S60 5.0
        * @param aChannelInfo Info related to channel to be opened.
        * @return None.
        */        
        CSensorChannelProximity( MCFContextInterface& aCF );
        
        // 2nd phase constrcuctor
        void ConstructL( const TDesC8& aLocation, const TDesC8& aVendor );
    };
    
#endif // C_SENSORCHANNELPROXIMITY_H
