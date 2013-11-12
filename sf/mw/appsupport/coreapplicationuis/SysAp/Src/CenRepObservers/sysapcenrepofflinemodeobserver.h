/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * CSysApCenRepOfflineModeObserver is a an observer for the Offline Mode central Repository key.
 * This class handles the changes to the offline mode changes done from Power Menu or Control Panel 
 *
 */


#ifndef SYSAPCENREPOFFLINEMODEOBSERVER_H
#define SYSAPCENREPOFFLINEMODEOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

#include "MSysApOfflineModeController.h"

NONSHARABLE_CLASS(CSysApCenRepOfflineModeObserver) : public CBase, 
                                                     public MCenRepNotifyHandlerCallback
    {
public: // Constructors and destructor
    /**
    * Default constructor
    */
    static CSysApCenRepOfflineModeObserver* NewL( 
            MSysApOfflineModeController& aSysApOfflineModeController );
    
    /**
    * Destructor.
    */
    ~CSysApCenRepOfflineModeObserver();
    
private:    
    /**
    * Two phase ConstructL
    */
    void ConstructL( );
    
    /**
    * Constructor 
    * @param aSysApOfflineModeController is the instance of Offline mode controller
    */
    CSysApCenRepOfflineModeObserver( MSysApOfflineModeController& aSysApOfflineModeController );
    
    /**
    * default constructor.
    */
    CSysApCenRepOfflineModeObserver();
    
public: // From MCenRepNotifyHandlerCallback
    /**
    * Handles the changes to offline mode state
    * @param aId is the cen rep key which was changed
    * @param aNewValue is the new value		
    */
    void HandleNotifyInt( TUint32 aId, TInt aNewValue );
    
    /**
    * Handles the error in changing offline mode state
    * @param aId is the cen rep key which was changed
    * @param aError is the error id		
    * @param aHandler is Offline cep rep handler same as iOfflineModeHandler		
    */
    void HandleNotifyError( TUint32 aId, TInt aError, CCenRepNotifyHandler* aHandler );
    
    /**
    * Returns whether Offline mode is currently active or not
    * @return ETrue if Offline mode is active, else EFalse
    */		
    TBool IsOffline() const;
    
private:
    /**
    * Instance of Offline mode controller. Used for enabling or disabling offline mode
    */	
    MSysApOfflineModeController& iSysApOfflineModeController;
    
    /**
    * Central Repository for gettting the Offline Mode
    */	
    CRepository*          iOfflineCenRep;
    
    /**
    * Handler to the cen rep for listening to the offline changes
    */
    CCenRepNotifyHandler* iOfflineModeHandler;
    };

#endif      // SYSAPCENREPBTOBSERVER_H
