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
* Description:  Bind operation
 *
*/

#ifndef C_PSCOPERATIONDELETEPRESENTITYGROUP_H
#define C_PSCOPERATIONDELETEPRESENTITYGROUP_H

#include <e32base.h>
#include <ximpbase.h>

#include "ximpoperationbase.h"
#include "presenceoperationdefs.h"

class CPresentityGroupInfoImp;

/**
 * Create presentity group operation
 *
 * @lib presenceoperation.lib
 * @since S60 v4.0
 */
class COperationDeletePresentityGroup : public CXIMPOperationBase
    {

    public:
        
        IMPORT_C COperationDeletePresentityGroup();

        void ConstructL( const TDesC8& aParamPck );

        ~COperationDeletePresentityGroup();

    public: // from CXIMPOperationBase
        
        void ProcessL();

        void RequestCompletedL();
        
        TInt Type() const;
        
    private: // data
    
        /**
         * groupinfo
         */
        CPresentityGroupInfoImp* iGroupInfo;
    };


#endif // C_PSCOPERATIONDELETEPRESENTITYGROUP_H
