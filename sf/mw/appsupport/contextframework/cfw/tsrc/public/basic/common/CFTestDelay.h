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
* Description:  ?Description
*
*/

#ifndef C_CFTESTDELAY_H
#define C_CFTESTDELAY_H

#include <e32base.h>

NONSHARABLE_CLASS( CCFTestDelay ): public CTimer
    {
    public:  // Constructors and destructor

        // Two-phased constructor.
        static CCFTestDelay* NewL();
        static CCFTestDelay* NewLC();
               
        // Destructor.
        virtual ~CCFTestDelay();
        
    public: // New methods
   
        void Start( const TTimeIntervalMicroSeconds32& aInterval );

        void Stop();
        
    private: // From base classes
    
        // @see CActive
        void RunL();
        
        // @see CActive
        void DoCancel();
        
    private:
    
        CCFTestDelay();
        void ConstructL();

    private: // Data
    
        // Own: Active scheduler wait
        CActiveSchedulerWait* iWait;
    };

#endif  //C_CFTESTDELAY_H
