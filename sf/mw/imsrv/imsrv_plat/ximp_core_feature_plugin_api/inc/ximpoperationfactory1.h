/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence Service Connection operation factory
 *
*/


#ifndef XIMPCPSCOPERATIONFACTORY1_H
#define XIMPCPSCOPERATIONFACTORY1_H

#include <e32base.h>
#include <ecom/ecom.h>
#include <ximpfeaturepluginifids.hrh>

class CXIMPOperationBase;

//#include <imdlluids.hrh>  
 
/**
 * Operation factory interface
 *
 * @lib ximpoperation.dll
 * @since S60 v4.0
 */
class MXIMPOperationFactory1
    {
    
    public:

    /**
     * ECom interface UID for the MXIMPOperationFactory1.
     *
     * This interface ID is used:
     * 1) In XIMPFw ECom registration file, to declare
     *    implemented interface. It is used also by
     *    the MXIMPClient::NewClientL() instantiation
     *    method, when creating MXIMPOperationFactory instance
     *    through the ECom.
     *
     * 2) By clients when resolving MXIMPOperationFactory interface
     *    through MXIMPBase::GetInterface().
     */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_OPERATIONS_INTERFACE };
    
    /**
    * To get list of implementation plugins which supports this interface
    */
    inline static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray) ;
    
public:

    /**
    * Object creation functions
    */
	inline static MXIMPOperationFactory1* NewL();

    /**
    * Object creation functions
    */
	inline static MXIMPOperationFactory1* NewL( TUid aImplUid );

    /**
     * Public destructor.
     * Object can be deleted through this interface.
     */
    virtual inline ~MXIMPOperationFactory1();



protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MXIMPOperationFactory1();

public: // From MXIMPOperationFactory1

		/**
		 * Create operation by operation id.
		 * @param aOperation. Identifies operation needed to create.
		 * @return Operation as base object.
	 	 */
		virtual CXIMPOperationBase* CreateOperationL( TInt aOperation ) = 0;
		
private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };
#include <ximpoperationfactory1.inl>

#endif      // XIMPCPSCOPERATIONFACTORY1_H


