/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Startup Subscriber (Publish & Subscribe).
*
*/


#ifndef STARTUPMEDIATORPLUGINSUBSCRIBER_H
#define STARTUPMEDIATORPLUGINSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MStartupMediatorPluginPropertyResponder.h"

// CLASS DECLARATION
/**
*  CStartupMediatorPluginSubscriber
*  
*  @lib
*  @since 3.0
*/

class CStartupMediatorPluginSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static CStartupMediatorPluginSubscriber* NewL( MStartupMediatorPluginPropertyResponder& aStartupMediatorPluginPropertyResponder, 
                                                   const TUid& aCategory, 
                                                   TUint aKey );

    /**
    * Destructor.
    */
     ~CStartupMediatorPluginSubscriber();

     void Subscribe();

private:
     CStartupMediatorPluginSubscriber( MStartupMediatorPluginPropertyResponder& aStartupMediatorPluginPropertyResponder, 
                                       const TUid& aCategory, TUint 
                                       aKey );
     void ConstructL();

public: // from CActive
    /**
    *    @param none
    *    @return none
    */    
    void RunL();
    
    /**
    *    @param aError the error returned
    *    @return error
    */
    TInt RunError( TInt aError );

    /**
    *    @param none
    *    @return none
    */    
    void DoCancel();

private: // Methods not implemented
    CStartupMediatorPluginSubscriber( const CStartupMediatorPluginSubscriber& );           // Copy constructor
    CStartupMediatorPluginSubscriber& operator=( const CStartupMediatorPluginSubscriber& );// Assigment operator

private:

    MStartupMediatorPluginPropertyResponder&    iStartupMediatorPluginPropertyResponder;
    RProperty                                   iProperty;
    TUid                                        iCategory;
    TUint                                       iKey;
};


#endif // STARTUPMEDIATORPLUGINSUBSCRIBER_H

// End of File
