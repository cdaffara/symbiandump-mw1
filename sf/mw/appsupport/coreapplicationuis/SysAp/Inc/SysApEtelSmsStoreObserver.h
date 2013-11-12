/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelSmsStoreObserver handles SysAp SMS store 
*              : related observing functionality.
*
*/


#ifndef SYSAPETELSMSSTOREOBSERVER_H
#define SYSAPETELSMSSTOREOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <etelmm.h>

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApEtelSmsStoreObserver
*  
*  @lib   sysap
*  @since 3.0
*/

class CSysApEtelSmsStoreObserver : 
              public CActive
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApEtelSmsStoreObserver* NewL( CSysApAppUi& aSysApAppUi, RMobileSmsStore& aStore );

    /**
    * Destructor.
    */
    ~CSysApEtelSmsStoreObserver();
    
    /**
    * Returns current state
    */
    TBool IsFull() const;

    /**
    * Reads status from Etel
    */
    void ReadStatusInStartup();

    protected:

    // From CActive
    void RunL();
    void DoCancel();
    
private:
    class CStoreStatusReader : public CActive
        {
        public:
            CStoreStatusReader(
                CSysApEtelSmsStoreObserver& aObserver );
            ~CStoreStatusReader();
            void Start();

            // From CActive
            void RunL();
            void DoCancel();
        private:
            CSysApEtelSmsStoreObserver& iObserver;
            RMobilePhoneStore::TMobilePhoneStoreInfoV1Pckg iInfoPckg;
        };

private:

    /**
    * Constructors.
    */         
    CSysApEtelSmsStoreObserver( CSysApAppUi& aSysApAppUi, RMobileSmsStore& aStore );
    void ConstructL();

    /*
    * Order notification
    */
    void OrderNotification();

private:
    
    CSysApAppUi&        iSysApAppUi;  // not owned
    
    RMobileSmsStore&    iStore;       // not owned
    
    TBool               iStoreFull;     
    
    TUint32             iNewEvent;           // Receives the notified event
    TInt                iChangedIndex;       // Holds the changed index of notified event
    CStoreStatusReader* iStatusReader;       // owned
    RMobilePhoneStore::TMobilePhoneStoreInfoV1 iStoreInfo;
};

#endif // SYSAPETELSMSSTOREOBSERVER_H

// End of File
