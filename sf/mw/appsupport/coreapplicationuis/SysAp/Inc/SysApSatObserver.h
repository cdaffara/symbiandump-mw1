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
* Description:  Definition of CSysApSatObserver.
*
*/



#ifndef CSYSAPSATOBSERVER_H
#define CSYSAPSATOBSERVER_H

//  INCLUDES
#include <msatrefreshobserver.h>

// FORWARD DECLARATIONS
class RSatSession;
class RSatRefresh;

// CLASS DECLARATIONS

/**
*  Callback class for refresh events.
*
*/
class MSysApSatObserver
    {
    public:

    /**
    * Updates the call forwarding indicators on EL2 line change.
    *
    */ 
    virtual void SimFileChangedL() = 0;
    };

/**
*  Observer class for sat refresh events
*
*/
class CSysApSatObserver : public CBase, public MSatRefreshObserver
    {
public:  // Constructors and destructor
    
    /**
    * Two-phased constructor.
    *
    * @param aObserver reference to observer class
    * @param aFile Elementary file to be observed
    */
    static CSysApSatObserver* NewL( MSysApSatObserver& aObserver, TInt aFile );
    
    /**
    * Destructor.
    */
    virtual ~CSysApSatObserver();

public: 

    // From MSatRefreshObserver
    TBool AllowRefresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );
    void Refresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );
          
private:

    /**
    * C++ default constructor.
    */
    CSysApSatObserver( MSysApSatObserver& aObserver );
                              
    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( TInt aFile );
    
private:    // Data

    // Sat session connection.
    RSatSession* iSatSession;
    // Sat refresh connection
    RSatRefresh* iSatRefresh;
    // Reference to observer
    MSysApSatObserver& iObserver;
    };

#endif      // CSYSAPSATOBSERVER_H   



// End of File
