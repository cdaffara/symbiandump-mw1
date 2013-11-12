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
* Description:  Implementation of MXIMPObjectFactory
 *
*/

#ifndef CXIMPOBJECTFACTORYIMP_H
#define CXIMPOBJECTFACTORYIMP_H

#include <ximpobjectfactory.h>
#include "ximpapiobjbase.h"




/**
 * MXIMPObjectFactory API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
class CXIMPObjectFactoryImp: public CXIMPApiObjBase,
                             public MXIMPObjectFactory
    {
    public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPOBJECTFACTORYIMP };


public:

    IMPORT_C static CXIMPObjectFactoryImp* NewL();
    ~CXIMPObjectFactoryImp();

private:
    CXIMPObjectFactoryImp();



public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MXIMPObjectFactory


    MXIMPStatus* NewStatusLC();
    MXIMPIdentity* NewIdentityLC();
    MXIMPRestrictedObjectCollection* NewRestrictedObjectCollectionLC();
    MXIMPObjectCollection* NewObjectCollectionLC();
    MXIMPDataSubscriptionState* NewDataSubscriptionStateLC();    
    
    };


#endif // CXIMPOBJECTFACTORYIMP_H
