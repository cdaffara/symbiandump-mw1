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
* Description:  handle search result operation
 *
*/

#ifndef C_OPERATIONHANDLESEARCHRESULT_H
#define C_OPERATIONHANDLESEARCHRESULT_H

#include <e32base.h>
#include <ximpbase.h>

#include <ximpoperationbase.h>
#include "searchoperationdefs.h"

#include "searchinfoimp.h"

#include "searchtypehelpers.h"
/**
 * handle search result operation
 *
 * @lib searchoperation.dll
 */
class COperationHandleSearchResult : public CXIMPOperationBase
    {

    public:
        /**
	     * constructor
	     */
        IMPORT_C COperationHandleSearchResult();
        
        /**
	     * 2nd phase constructor
	     */
        void ConstructL( const TDesC8& aParamPck );
        
        /**
         * Destructor
         */
        ~COperationHandleSearchResult();

    public: // from CXIMPOperationBase
        /**
	     * 
	     * @see CXIMPOperationBase 
	     */
        void ProcessL();
        
        /**
	     * 
	     * @see CXIMPOperationBase 
	     */
        void RequestCompletedL();
        
        /**
	     * 
	     * @see CXIMPOperationBase 
	     */
        TInt Type() const;
  
    private: // data
    
        /**
         * CXIMPIdentityImp : Not owned
         */
 
        CXIMPIdentityImp* iSearchId;
         
        /**
         * RSearchInfoImpArray : Not owned
         */
  
        RSearchInfoImpArray* iSearchResults;
        
        
    };


#endif // C_OPERATIONHANDLESEARCHRESULT_H
