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
* Description:  search operation
 *
*/

#ifndef C_OPERATIONSEARCH_H
#define C_OPERATIONSEARCH_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "searchoperationdefs.h"

#include <searchelement.h>

class MsearchElement;
class CSearchElementImp;
class CXIMPIdentityImp;
/**
 * search operation
 *
 * @lib searchoperation.dll
 */
class COperationSearch : public CXIMPOperationBase
    {

    public:
        /**
	     * constructor
	     */
        IMPORT_C COperationSearch();
     
        /**
	     * 2nd phase constructor
	     */
        void ConstructL( const TDesC8& aParamPck );
        
       /**
         * Destructor
         */
        ~COperationSearch();

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
         * array of MSearchElement : Owned
         */
        RPointerArray<MSearchElement> iSearchList;
        /**
         * CXIMPIdentityImp : Owned
         */
        CXIMPIdentityImp* iSearchId;
        
        /**
         * serachlimit
         */
        TInt iSearchLimit;
    };


#endif // C_OPERATIONSEARCH_H
