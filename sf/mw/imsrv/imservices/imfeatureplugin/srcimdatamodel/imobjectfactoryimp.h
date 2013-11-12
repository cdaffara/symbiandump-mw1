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
* Description:  Implementation of MImObjectFactory
 *
*/

#ifndef CIMOBJECTFACTORYIMP_H
#define CIMOBJECTFACTORYIMP_H

#include <imobjectfactory.h>
#include "ximpapiobjbase.h"
#include "imapiobjbase.h"

class MImGroupInfo;
class MImConversationInfo;
class MImInvitationInfo;
class MImSearchInfo;

/**
 * MImObjectFactory API object implementation.
 *
 * @lib immanager.dll
 * @since S60 v3.2
 */
class CImObjectFactoryImp: public CXIMPApiObjBase,
                                 public MImObjectFactory
    {
    public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CIMOBJECTFACTORYIMP };


public:

    IMPORT_C static CImObjectFactoryImp* NewL();
    ~CImObjectFactoryImp();

private:
    CImObjectFactoryImp();



public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MImObjectFactory

	MImConversationInfo* NewImConversationInfoLC();
    //MImGroupInfo* NewImGroupInfoLC();    
    //MImInvitationInfo* NewImInvitationInfoLC();
    //MImSearchInfo* NewImSearchInfoLC();
   
    };


#endif // CIMOBJECTFACTORYIMP_H
