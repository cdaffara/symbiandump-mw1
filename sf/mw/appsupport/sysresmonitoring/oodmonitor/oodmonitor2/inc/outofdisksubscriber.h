/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  OutOfDisk Subscriber (Publish & Subscribe).
*
*/


#ifndef OUTOFDISKSUBSCRIBER_H
#define OUTOFDISKSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "moutofdiskpropertyresponder.h"

// CLASS DECLARATION
/**
*  COutOfDiskSubscriber
*  
*  @lib
*  @since 3.0
*/

class COutOfDiskSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static COutOfDiskSubscriber* NewL( MOutOfDiskPropertyResponder& aOutOfDiskPropertyResponder, 
                                                   const TUid& aCategory, 
                                                   TUint aKey );

    /**
    * Destructor.
    */
     ~COutOfDiskSubscriber();

     void Subscribe();

private:
     COutOfDiskSubscriber( MOutOfDiskPropertyResponder& aOutOfDiskPropertyResponder, 
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
    COutOfDiskSubscriber( const COutOfDiskSubscriber& );           // Copy constructor
    COutOfDiskSubscriber& operator=( const COutOfDiskSubscriber& );// Assigment operator

private:

    MOutOfDiskPropertyResponder&                iOutOfDiskPropertyResponder;
    RProperty                                   iProperty;
    TUid                                        iCategory;
    TUint                                       iKey;
};


#endif // OUTOFDISKSUBSCRIBER_H

// End of File
