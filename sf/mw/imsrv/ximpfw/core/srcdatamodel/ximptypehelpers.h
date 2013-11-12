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
* Description:  Typedef helpers for really long typenames.
 *
*/

#ifndef XIMPTYPEHELPERS_H
#define XIMPTYPEHELPERS_H

#include "ximpapiobjbase.h"
#include <e32base.h>


class CXIMPIdentityImp;
class CXIMPApiDataObjBase;

// Key for opening up the typedefs:
//
// R(Xyz)Array = RXIMPObjOwningPtrArray<xyz>
//
// When adding new types, follow these conventions.
//

typedef RXIMPObjOwningPtrArray<CXIMPIdentityImp> RPrIdImpArray;


struct SXIMPReqParam
    {
    CXIMPApiDataObjBase* iParam;
    TInt iParamType;
    };

typedef RArray<SXIMPReqParam> RPrReqParamArray;

#endif // XIMPTYPEHELPERS_H


