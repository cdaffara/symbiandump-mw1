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
* Description:        
*
*/









#ifndef CSCHEDULERUTILITY_H
#define CSCHEDULERUTILITY_H

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
class CSchedulerUtility : public CBase
    {
    public: // New functions
        
        /**
        * Create and return instance of CSchedulerUtility.
        * @since 3.0
        * @return instance of CSchedulerUtility
        */
        //static CSchedulerUtility* InstanceL();
    	
        /** 
        * Two-phased constructor.
        */
        static CSchedulerUtility* NewL();
                
        /**
        * Destroys the scheduler utility instance.
        * @since 3.0
        */
       // static void Release();

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
        
        /**
        * Destructor.
        */
        virtual ~CSchedulerUtility();
        
    private:  

        /**
        * C++ default constructor.
        */
        CSchedulerUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    private:    // Data
         
        CActiveSchedulerWait* iWait;      

    };

#endif      // CSCHEDULERUTILITY_H
            
// End of File

