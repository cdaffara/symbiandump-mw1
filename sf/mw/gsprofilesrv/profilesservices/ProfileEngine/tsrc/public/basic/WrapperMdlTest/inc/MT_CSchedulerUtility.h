/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This singleton class takes care of creation of different other
*                classes needed in PECProfiles.
*
*/



#ifndef MT_CSCHEDULERUTILITY_H
#define MT_CSCHEDULERUTILITY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* A singleton class used to handle starting and stopping a new scheduler-loop
* This stops the scheduler loop if it was started by this class.
*
*  @lib Profiles_PEC.lib
*  @since 3.0
*/
class MT_CSchedulerUtility : public CBase
    {
    public: // New functions
        
        /**
        * Create and return instance of MT_CSchedulerUtility.
        * @since 3.0
        * @return instance of MT_CSchedulerUtility
        */
        //?type ?member_function( ?type ?arg1 );
        static MT_CSchedulerUtility* InstanceL();

        /**
        * Destroys the scheduler utility instance.
        * @since 3.0
        */
        static void Release();

        /**
         * Starts a new scheduler loop if one isn't already started.
         * @since 3.0
         */
        void Start();

        /**
         * Stops a scheduler loop if it was started by this class.
         * @since 3.0
         */
        void Stop();

    private:

        /**
        * Two-phased constructor.
        */
        static MT_CSchedulerUtility* NewL();
        
        /**
        * Destructor.
        */
        virtual ~MT_CSchedulerUtility();

        /**
        * C++ default constructor.
        */
        MT_CSchedulerUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
         
        CActiveSchedulerWait* iWait;
    };

#endif      // MT_CSCHEDULERUTILITY_H
            
// End of File

