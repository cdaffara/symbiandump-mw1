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

#ifndef PRESENCETYPEHELPERS_H
#define PRESENCETYPEHELPERS_H

#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"
#include <e32base.h>

class CPresentityGroupInfoImp;
class CPresentityGroupMemberInfoImp;
class CPresenceWatcherInfoImp;
class CXIMPIdentityImp;
class CPresenceGrantRequestInfoImp;
class CXIMPApiDataObjBase;
class CPresenceBlockInfoImp;
class CPresenceInfoFieldImp;
class CServicePresenceInfoImp;
class CDevicePresenceInfoImp;

// Key for opening up the typedefs:
//
// R(Xyz)Array = RXIMPObjOwningPtrArray<xyz>
// Pr = Presentity
// GrpInfo = GroupInfo
// GrpMemInfo = GroupMemberInfo
// WatLstInfo = WatcherListInfo
//
// When adding new types, follow these conventions.
//

typedef RXIMPObjOwningPtrArray<CPresentityGroupInfoImp> RPrGrpInfoImpArray;
typedef RXIMPObjOwningPtrArray<CPresentityGroupMemberInfoImp> RPrGrpMemInfoImpArray;
typedef RXIMPObjOwningPtrArray<CPresenceWatcherInfoImp> RPrWatLstInfoImpArray;
typedef RXIMPObjOwningPtrArray<CXIMPIdentityImp> RPrIdImpArray;
typedef RXIMPObjOwningPtrArray<CPresenceGrantRequestInfoImp> RPrGrntReqInfoImpArray;
typedef RXIMPObjOwningPtrArray<CPresenceBlockInfoImp> RPrBlockInfoImpArray;
typedef RXIMPObjOwningPtrArray<CPresenceInfoFieldImp> RPrInfoFieldArray;
typedef RXIMPObjOwningPtrArray<CServicePresenceInfoImp> RPrServicePrInfoArray;
typedef RXIMPObjOwningPtrArray<CDevicePresenceInfoImp> RPrDevicePrInfoArray;

struct SPresenceReqParam
    {
    CXIMPApiDataObjBase* iParam;
    TInt iParamType;
    };

typedef RArray<SPresenceReqParam> RPresenceReqParamArray;

#endif // PRESENCETYPEHELPERS_H


