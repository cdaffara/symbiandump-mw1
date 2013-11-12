/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApCenRepBtObserver class definition. 
*
*/


#ifndef SYSAPCENREPBTOBSERVER_H
#define SYSAPCENREPBTOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include "SysAp.hrh"
#include <btserversdkcrkeys.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION

/**
*  CSysApCenRepBtObserver
*
*  @lib   sysap
*  @since 3.0
*/

class CSysApCenRepBtObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApCenRepBtObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApCenRepBtObserver();

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
        CSysApCenRepBtObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApCenRepBtObserver();
        
    public: // From MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );
        
    public: // Other functions
        
        /**
        * Returns the current bluetooth power mode setting
        *
        * @since S60 3.1
        * @param None
        * @return TInt
        */               
        TInt BluetoothPowerMode() const;

    private:
        // By default, prohibit copy constructor
        CSysApCenRepBtObserver( const CSysApCenRepBtObserver& );
    
        // Prohibit assigment operator
        CSysApCenRepBtObserver& operator= ( const CSysApCenRepBtObserver& );
    
    private:
        CSysApAppUi&          iSysApAppUi;
        
        CRepository*          iSession;
        CCenRepNotifyHandler* iBTPowerStateHandler;
    };

#endif      // SYSAPCENREPBTOBSERVER_H
            
// End of File
