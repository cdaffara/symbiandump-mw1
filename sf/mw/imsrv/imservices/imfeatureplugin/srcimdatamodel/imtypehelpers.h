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
* Description:  Typedef helpers for really long typenames.
 *
*/

#ifndef IMTYPEHELPERS_H
#define IMTYPEHELPERS_H

#include "ximpapiobjbase.h"
#include "imapiobjbase.h"
#include <e32base.h>

class CXIMPApiDataObjBase;

// Key for opening up the typedefs:
//
// R(Xyz)Array = RXIMPObjOwningPtrArray<xyz>
// Im = IM
// GrpInfo = GroupInfo
// GrpMemInfo = GroupMemberInfo
// WatLstInfo = WatcherListInfo
//
// When adding new types, follow these conventions.
//

struct SImReqParam
    {
    CXIMPApiDataObjBase* iParam;
    TInt iParamType;
    };

typedef RArray<SImReqParam> RImReqParamArray;

#endif // IMTYPEHELPERS_H


