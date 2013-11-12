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


#ifndef STARTUPSUBSCRIBER_H
#define STARTUPSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MStartupPropertyResponder.h"

// CLASS DECLARATION
/**
*  CStartupSubscriber
*  
*  @lib   Startup
*  @since 3.0
*/

class CStartupSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static CStartupSubscriber* NewL( MStartupPropertyResponder& aStartupPropertyResponder, 
                                     const TUid& aCategory, 
                                     TUint aKey );

    /**
    * Destructor.
    */
     ~CStartupSubscriber();

     void Subscribe();

private:
     CStartupSubscriber( MStartupPropertyResponder& aStartupPropertyResponder, 
                         const TUid& aCategory, TUint 
                         aKey );
     void ConstructL();

public: // from CActive
    /**
    *	@param none
    *	@return none
    */    
    void RunL();
    
    /**
    *	@param aError the error returned
    *	@return error
    */
    TInt RunError( TInt aError );

    /**
    *	@param none
    *	@return none
    */    
    void DoCancel();
	
private: // Methods not implemented
    CStartupSubscriber( const CStartupSubscriber& );           // Copy constructor
    CStartupSubscriber& operator=( const CStartupSubscriber& );// Assigment operator

private:

    MStartupPropertyResponder&  iStartupPropertyResponder;
    RProperty                   iProperty;
    TUid                        iCategory;
    TUint                       iKey;
};


#endif // STARTUPSUBSCRIBER_H

// End of File
