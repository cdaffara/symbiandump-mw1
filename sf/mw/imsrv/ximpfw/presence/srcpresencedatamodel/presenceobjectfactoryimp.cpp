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
* Description:  Implementation of MPresenceManagement
 *
*/

#include "presenceobjectfactoryimp.h"
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpstatusimp.h"
#include "ximprestrictedobjectcollectionimp.h"
#include "ximpobjectcollectionimp.h"
#include "ximpidentityimp.h"
#include "presentitygroupinfoimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presencewatcherinfoimp.h"
#include "presencegrantrequestinfoimp.h"
#include "ximpdatasubscriptionstateimp.h"
#include "personpresenceinfoimp.h"
#include "servicepresenceinfoimp.h"
#include "devicepresenceinfoimp.h"
#include "presenceinfofieldimp.h"
#include "presenceinfofieldvalueenumimp.h"
#include "presenceinfofieldvaluetextimp.h"
#include "presenceinfofieldvaluebinaryimp.h"
#include "presenceblockinfoimp.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceObjectFactoryImp* CPresenceObjectFactoryImp::NewL()
    {
    CPresenceObjectFactoryImp* self = new( ELeave ) CPresenceObjectFactoryImp;
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceObjectFactoryImp, 
                                     MPresenceObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceObjectFactoryImp, 
                                           MPresenceObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::~CPresenceObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CPresenceObjectFactoryImp::~CPresenceObjectFactoryImp()
    {
    #if _BullseyeCoverage
    cov_write();
    #endif
    }


// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::CPresenceObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CPresenceObjectFactoryImp::CPresenceObjectFactoryImp()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresentityGroupInfoLC()
// ---------------------------------------------------------------------------
//
MPresentityGroupInfo* CPresenceObjectFactoryImp::NewPresentityGroupInfoLC()
    {
    return CPresentityGroupInfoImp::NewLC();
    }
    

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresentityGroupMemberInfoLC()
// ---------------------------------------------------------------------------
//
MPresentityGroupMemberInfo* 
    CPresenceObjectFactoryImp::NewPresentityGroupMemberInfoLC()
    {
    return CPresentityGroupMemberInfoImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceWatcherInfoLC()
// ---------------------------------------------------------------------------
//
MPresenceWatcherInfo* CPresenceObjectFactoryImp::NewPresenceWatcherInfoLC()
    {
    return CPresenceWatcherInfoImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceGrantRequestInfoLC()
// ---------------------------------------------------------------------------
//
MPresenceGrantRequestInfo* 
    CPresenceObjectFactoryImp::NewPresenceGrantRequestInfoLC()
    {
    return CPresenceGrantRequestInfoImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceInfoLC()
// ---------------------------------------------------------------------------
//
MPresenceInfo* CPresenceObjectFactoryImp::NewPresenceInfoLC()
    {
    return CPresenceInfoImp::NewLC();
    }


// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceInfoFilterLC()
// ---------------------------------------------------------------------------
//
MPresenceInfoFilter* CPresenceObjectFactoryImp::NewPresenceInfoFilterLC()
    {
    return CPresenceInfoFilterImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceInfoFieldLC()
// ---------------------------------------------------------------------------
//
MPresenceInfoField* CPresenceObjectFactoryImp::NewInfoFieldLC()
    {
    return CPresenceInfoFieldImp::NewLC();
    }
    
// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPersonPresenceInfoLC()
// ---------------------------------------------------------------------------
//
MPersonPresenceInfo* CPresenceObjectFactoryImp::NewPersonPresenceInfoLC()
    {
    return CPersonPresenceInfoImp::NewLC();
    }
    
// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewServicePresenceInfoLC()
// ---------------------------------------------------------------------------
//
MServicePresenceInfo* CPresenceObjectFactoryImp::NewServicePresenceInfoLC()
    {
    return CServicePresenceInfoImp::NewLC();
    }
    
// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewDevicePresenceInfoLC()
// ---------------------------------------------------------------------------
//
MDevicePresenceInfo* CPresenceObjectFactoryImp::NewDevicePresenceInfoLC()
    {
    return CDevicePresenceInfoImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewEnumInfoFieldLC()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldValueEnum* CPresenceObjectFactoryImp::NewEnumInfoFieldLC()
    {
    return CPresenceInfoFieldValueEnumImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewTextInfoFieldLC()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldValueText* CPresenceObjectFactoryImp::NewTextInfoFieldLC()
    {
    return CPresenceInfoFieldValueTextImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewBinaryInfoFieldLC()
// ---------------------------------------------------------------------------
//
MPresenceInfoFieldValueBinary* CPresenceObjectFactoryImp::NewBinaryInfoFieldLC()
    {
    return CPresenceInfoFieldValueBinaryImp::NewLC();
    }

// ---------------------------------------------------------------------------
// CPresenceObjectFactoryImp::NewPresenceBlockInfoLC()
// ---------------------------------------------------------------------------
//
MPresenceBlockInfo* CPresenceObjectFactoryImp::NewPresenceBlockInfoLC()
    {
    return CPresenceBlockInfoImp::NewLC();
    }


