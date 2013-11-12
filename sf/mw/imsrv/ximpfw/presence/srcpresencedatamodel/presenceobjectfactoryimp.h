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
* Description:  Implementation of MPresenceObjectFactory
 *
*/

#ifndef CPRESENCEOBJECTFACTORYIMP_H
#define CPRESENCEOBJECTFACTORYIMP_H

#include <presenceobjectfactory.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"




/**
 * MPresenceObjectFactory API object implementation.
 *
 * @lib presencemanager.dll
 * @since S60 v3.2
 */
class CPresenceObjectFactoryImp: public CXIMPApiObjBase,
                                 public MPresenceObjectFactory
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEOBJECTFACTORYIMP };


public:

    IMPORT_C static CPresenceObjectFactoryImp* NewL();
    ~CPresenceObjectFactoryImp();

private:
    CPresenceObjectFactoryImp();



public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresenceObjectFactory

   
    MPresentityGroupInfo* NewPresentityGroupInfoLC();
    MPresentityGroupMemberInfo* NewPresentityGroupMemberInfoLC();
    MPresenceWatcherInfo* NewPresenceWatcherInfoLC();
    MPresenceGrantRequestInfo* NewPresenceGrantRequestInfoLC();
    MPresenceInfo* NewPresenceInfoLC();
    MPresenceInfoFilter* NewPresenceInfoFilterLC();
    MPersonPresenceInfo* NewPersonPresenceInfoLC();
    MServicePresenceInfo* NewServicePresenceInfoLC();
    MDevicePresenceInfo* NewDevicePresenceInfoLC();
    MPresenceInfoField* NewInfoFieldLC();
    MPresenceInfoFieldValueEnum* NewEnumInfoFieldLC();
    MPresenceInfoFieldValueText* NewTextInfoFieldLC();
    MPresenceInfoFieldValueBinary* NewBinaryInfoFieldLC();
    MPresenceBlockInfo* NewPresenceBlockInfoLC();
    };


#endif // CPRESENCEOBJECTFACTORYIMP_H
