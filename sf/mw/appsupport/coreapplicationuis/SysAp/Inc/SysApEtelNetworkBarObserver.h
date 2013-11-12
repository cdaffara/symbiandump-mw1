/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelNetworkBarObserver handles SysAp CTelephony 
*              : related observing functionality.
*
*/


#ifndef SYSAPETELNETWORKBAROBSERVER_H
#define SYSAPETELNETWORKBAROBSERVER_H

// INCLUDES
#include <e32base.h>
#include <etelmm.h>
#include "sysapetelnetworkbargetter.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApEtelNetworkBarObserver
*  
*  @lib   sysap
*  @since 3.0
*/

class CSysApEtelNetworkBarObserver : 
              public CActive,
              public MSysApEtelNetworkBarGetterCallback
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApEtelNetworkBarObserver* NewL( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone );

    /**
    * Destructor.
    */
    virtual ~CSysApEtelNetworkBarObserver();
    
    /**
    * Returns current data
    */
    TInt GetBarCount() const;

    protected:

    // From CActive
    void RunL();
    void DoCancel();

private:

    /**
    * Constructors.
    */         
    CSysApEtelNetworkBarObserver( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone );
    void ConstructL();

    /*
    * Order notification
    */
    void OrderNotification();
    
    // from MSysApEtelNetworkBarGetterCallback
    virtual void GetBarCountCompleted( TInt8 aSignalBars );

private:
    
    CSysApAppUi&        iSysApAppUi;  // not owned
    
    RMobilePhone&       iPhone;       // not owned
    
    TInt8               iSignalBars;
    TInt32              iSignalStrength;
    TInt8               iPreviousNetworkBarValue;
    
    /**
    * Boot-time getter object for signal strength.
    * Owned.
    */
    CSysApEtelNetworkBarGetter* iNetworkBarGetter;
    
    /**
    * Stores signal bar value fetched by iNetworkBarGetter.
    */
    TInt8 iSignalBarsFromGetter;
    
};

#endif // SYSAPETELNETWORKBAROBSERVER_H

// End of File
