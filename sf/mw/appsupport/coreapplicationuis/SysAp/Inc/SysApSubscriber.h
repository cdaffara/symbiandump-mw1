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
* Description:  SysAp Subrciber (Publish & Subscribe).
*
*/


#ifndef SYSAPSUBSCRIBER_H
#define SYSAPSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "MSysApPropertyResponder.h"

// CLASS DECLARATION
/**
*  CSysApSubscriber
*  
*  @lib   sysap
*  @since 2.5 
*/

class CSysApSubscriber : public CActive
{
public:
    /**
    * Two-phased constructor.
    */         
    static CSysApSubscriber* NewL( MSysApPropertyResponder& aSysApPropertyResponder, const TUid& aCategory, const TUint aKey );

    /**
    * Destructor.
    */
     ~CSysApSubscriber();

     void Subscribe();

private:
     CSysApSubscriber( MSysApPropertyResponder& aSysApPropertyResponder, const TUid& aCategory, const TUint aKey );
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
    CSysApSubscriber( const CSysApSubscriber& );           // Copy constructor
    CSysApSubscriber& operator=( const CSysApSubscriber& );// Assigment operator

private:

    MSysApPropertyResponder&  iSysApPropertyResponder;
    RProperty                 iProperty;
    TUid                      iCategory;
    TUint                     iKey;
};


#endif // SYSAPSUBSCRIBER_H

// End of File
