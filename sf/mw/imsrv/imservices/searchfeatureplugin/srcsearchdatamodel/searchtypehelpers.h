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

#ifndef SEARCHTYPEHELPERS_H
#define SEARCHTYPEHELPERS_H

#include "ximpapiobjbase.h"
#include "searchapiobjbase.h"
#include <e32base.h>

class CXIMPApiDataObjBase;
class CSearchInfoImp;
class CSearchKeyInfoImp;

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

struct SSearchReqParam
    {
    CXIMPApiDataObjBase* iParam;
    TInt iParamType;
    };

typedef RArray<SSearchReqParam> RSearchReqParamArray;
typedef RXIMPObjOwningPtrArray<CSearchInfoImp> RSearchInfoImpArray;
typedef RXIMPObjOwningPtrArray<CSearchKeyInfoImp> RSearchKeyInfoImpArray;

#endif // SEARCHTYPEHELPERS_H


