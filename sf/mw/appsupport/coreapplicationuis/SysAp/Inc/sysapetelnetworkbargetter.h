/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelNetworkBarGetter declaration.
*
*/


#ifndef SYSAPETELNETWORKBARGETTER_H
#define SYSAPETELNETWORKBARGETTER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class RMobilePhone;

/**
* MSysApEtelNetworkBarGetterCallback.
*
* Callback interface for notifying client about
* completion of get request.
*
* @lib None
* @since S60 3.2
*/
class MSysApEtelNetworkBarGetterCallback
    {
public:    
    virtual void GetBarCountCompleted( TInt8 aSignalBars ) = 0;
    };


/**
* CSysApEtelNetworkBarGetter
*  
* An object for making a one-shot request for retrieving count of network bars. 
*
* @lib None
* @since S60 3.2
*/
class CSysApEtelNetworkBarGetter : public CActive
    {
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApEtelNetworkBarGetter* NewL( MSysApEtelNetworkBarGetterCallback& aCallback,
                                             RMobilePhone& aPhone );

    /**
    * Destructor.
    */
    virtual ~CSysApEtelNetworkBarGetter();
    
    /**
    * Initiates retrieval of network bar count. The actual value is 
    * provided via MSysApEterNetworkBarGetterCallback interface.
    */
    void GetBarCountL();

protected:

    // From CActive
    void RunL();
    void DoCancel();

private:
    /**
    * C++ default constructor.
    */
    CSysApEtelNetworkBarGetter();
    
    /**
    * Constructor.
    */         
    CSysApEtelNetworkBarGetter( MSysApEtelNetworkBarGetterCallback& aCallback,
                                RMobilePhone& aPhone );
    
    /**
    * Symbian 2nd phase constructor
    */
    void ConstructL();

private:
    
    /**
    * Reference to client waiting for the request completion.
    */
    MSysApEtelNetworkBarGetterCallback& iCallback;
    
    /**
    * Reference to shared ETel mobile phone instance.
    */
    RMobilePhone& iPhone;
    
    /**
    * Count of network signal bars that should be displayed.
    */ 
    TInt8 iSignalBars;
    
    /**
    * Network signal strength value in dBm.
    */
    TInt32 iSignalStrength;
    };

#endif // SYSAPETELNETWORKBARGETTER_H

// End of File
