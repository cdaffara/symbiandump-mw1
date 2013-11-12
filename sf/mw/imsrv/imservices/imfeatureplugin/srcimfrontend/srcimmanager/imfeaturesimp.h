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
* Description:  Implementation of MXIMPContext
*
*/

#ifndef CIMFEATURESIMP_H
#define CIMFEATURESIMP_H

#include <imfeatures.h>
#include "ximpapiobjbase.h"
#include "imapiobjbase.h"

class CImConversationImp;
class CImObjectFactoryImp;
class MXIMPContext;
class MXIMPContextInternal;


/**
 * MXIMPContext API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CImFeaturesImp ):  public MImFeatures,
                                            public CXIMPApiObjBase
    {
    public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CIMFEATURESIMP };


public:

    IMPORT_C static CImFeaturesImp* NewL( MXIMPContext* aContext );
    virtual ~CImFeaturesImp();


private:

    CImFeaturesImp();
    void SetCtxL(MXIMPContext* aContext);
    void ConstructL(MXIMPContext* aContext);


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MImFeatures

   
    MImObjectFactory& ImObjectFactory() const;
    MImConversation& ImConversation() const;
    //MImGroups& ImGroups() const;
    //MImInvitation& ImInvtation() const;
    //MImSearch& ImSearch() const;



private: // data

    /**
     * Context for having a connection context server.
     */
    MXIMPContextInternal* iCtxInternal;
    

    /**
     * Object factory sub interface.
     */
    CImObjectFactoryImp* iImObjFactory;

    /**
     * Im Conversation management sub interface.
     */
    CImConversationImp* iImConv;
    };


#endif // CIMFEATURESIMP_H
