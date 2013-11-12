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
* Description:  get search keys operation header
 *
*/

#ifndef C_OPERATIONGETSEARCHKEYS_H
#define C_OPERATIONGETSEARCHKEYS_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "searchoperationdefs.h"


/**
 * get search keys operation class 
 *
 * @lib searchoperation.dll
 */
class COperationGetSearchKeys : public CXIMPOperationBase
    {

    public:
        /**
	     * constructor
	     */
        IMPORT_C COperationGetSearchKeys();
     
        /**
	     * 2nd phase constructor
	     */
        void ConstructL( const TDesC8& aParamPck );
        
       /**
         * Destructor
         */
        ~COperationGetSearchKeys();

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
        
    };


#endif // C_OPERATIONGETSEARCHKEYS_H
