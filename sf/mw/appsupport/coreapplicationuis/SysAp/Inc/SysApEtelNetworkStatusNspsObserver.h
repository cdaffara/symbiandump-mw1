/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelNetworkStatusNspsObserver class definition.
*
*/


#ifndef SYSAPETELNETWORKSTATUSNSPSOBSERVER_H
#define SYSAPETELNETWORKSTATUSNSPSOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <rmmcustomapi.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApEtelNetworkStatusNspsObserver
*
*  @lib   sysap
*  @since 1.0* 
*/

class CSysApEtelNetworkStatusNspsObserver : public CActive
    {
    public: // Constructors and destructor
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @param RCtmGsmPhone& aCustomPhone
        * @return void
        */      
        CSysApEtelNetworkStatusNspsObserver( CSysApAppUi& aSysApAppUi, RMmCustomAPI& aCustomAPI );

        /**
        * Destructor.
        */
        virtual ~CSysApEtelNetworkStatusNspsObserver();

    public:
        void CommandNetCsWakeupOnNsps();
    
    private:
        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );

        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApEtelNetworkStatusNspsObserver();
        
        // By default, prohibit copy constructor
        CSysApEtelNetworkStatusNspsObserver( const CSysApEtelNetworkStatusNspsObserver& );
    
        // Prohibit assigment operator
        CSysApEtelNetworkStatusNspsObserver& operator= ( const CSysApEtelNetworkStatusNspsObserver& );
        
    private:
        /**
        * From CActive. Cancels the active object from observing System Agent events. .
        * @param None
        * @return void
        */
        void DoCancel();

        /**
        * From CActive. Gets called by active object when a System Agent event occurs.
        * @param None
        * @return void
        */
        void RunL();

        /**
        * From CActive. Gets called by active object when leave happens inside RunL
        * @param TInt aError
        * @return void
        */
        TInt RunError( TInt aError );
    
    private:
        /**
        * Issues the sms storage notification to Etel Custom 
        * @param None
        * @return void
        */
        void IssueNetworkStatusNspsNotification();

    private:
        RMmCustomAPI::TNspsStatus iNspsStatus;
        RMmCustomAPI&   iCustomAPI;
        CSysApAppUi&    iSysApAppUi;
        TBool           iNetWakeUpSent;
    };  

#endif      // SYSAPSETELSMSSTORAGEOBSERVER_H
            
// End of File
