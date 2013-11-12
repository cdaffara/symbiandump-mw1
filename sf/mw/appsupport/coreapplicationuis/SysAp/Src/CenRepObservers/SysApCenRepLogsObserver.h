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
* Description:  CSysApCenRepLogsObserver class definition. 
*
*/


#ifndef SYSAPCENREPLOGSOBSERVER_H
#define SYSAPCENREPLOGSOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
//#include <LogsDomainCRKeys.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION

/**
*  CSysApCenRepLogsObserver
*
*  @lib   sysap
*  @since 3.0
*/

class CSysApCenRepLogsObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApCenRepLogsObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApCenRepLogsObserver();
        
        /*
        * Called when indicators can be updated, and updates
        * missed call indicator if SIM check has been done.
        */ 
        void HandleUiReadyL();
            
        /**
        * Called after SIM check has been finished. Updates
        * missed call indicator if UI is ready.
        */
        void HandleSimChangedCheckDoneL();

    private:

        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */         
        CSysApCenRepLogsObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApCenRepLogsObserver();
        
        /**
        * Updates the missed calls indicator.
        */
        void UpdateMissedCallsIndicatorL();
        
    public: // From MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );

    private:
        // By default, prohibit copy constructor
        CSysApCenRepLogsObserver( const CSysApCenRepLogsObserver& );
    
        // Prohibit assigment operator
        CSysApCenRepLogsObserver& operator= ( const CSysApCenRepLogsObserver& );
    
    private:
        CSysApAppUi&          iSysApAppUi;
        
        CRepository*          iSession;
        CCenRepNotifyHandler* iNewMissedCallsHandler;
        
        TInt                  iMissedCallsValue;
        
        // Whether indicators can be updated to status pane. 
        TBool iUiReady;
        
        // Whether SIM check has been done.
        TBool iSimChangedCheckDone;
    };

#endif      // SYSAPCENREPLOGSOBSERVER_H
            
// End of File
