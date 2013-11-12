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
* Description:  Im connection host
 *
*/

#ifndef CPROTOCOLIMDATAHOSTIMP_H
#define CPROTOCOLIMDATAHOSTIMP_H

#include <e32base.h>
#include <ximpprotocolconnectionhost.h>
#include <ximpcontext.h>
#include <ximpbase.h>
#include <protocolimdatahost.h>
#include "imapiobjbase.h"
#include "ximpapiobjbase.h"
//#include "ximpitemparent.h"
#include "ximphost.h"
//#include "ximpoperationdefs.h"
//#include "ximprequestidbuilder.h"


class MXIMPHost;
class MImObjectFactory;
class CImObjectFactoryImp;

class CXIMPPscContext;
class MImDataCache;
class CImDataCache;
class CXIMPContextStateEventImp;
class CPeriodic;
class MXIMPProtocolConnection;
class CProtocolImConversationDataHostImp;
class MProtocolImConversationDataHost;
class CProtocolImGroupDataHostImp;
class CProtocolImInvitationDataHostImp;
class CProtocolImSearchDataHostImp;


/**
 * Im connection host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
//class CProtocolImDataHostImp : public CXIMPApiObjBase,
//                                     public MProtocolImDataHost
NONSHARABLE_CLASS(CProtocolImDataHostImp): public MProtocolImDataHost
    {
public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CPROTOCOLIMDATAHOSTIMP };

public:

    static CProtocolImDataHostImp* NewL( MXIMPHost& aHost );

    static CProtocolImDataHostImp* NewLC( MXIMPHost& aHost );

    virtual ~CProtocolImDataHostImp();

private:

    CProtocolImDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    //XIMPIMP_DECLARE_IF_BASE_METHODS


public: // MProtocolImDataHost
   
    MImObjectFactory& ImObjectFactory();

    MProtocolImConversationDataHost& ConversationDataHost();

    //MProtocolImGroupDataHost& GroupDataHost();

    //MProtocolImInvitationDataHost& InvitationDataHost();

    //MProtocolImSearchDataHost& SearchDataHost();
    
    CProtocolImConversationDataHostImp& ConversationDataAccess() ;
    
    
    CImDataCache& ImDataCache();
   

private: // data

	//not owned
    MXIMPHost& iHost;

    /**
     * Data cache
     */
 	CImDataCache* iImDataCache;

    /**
     * Data hosts
     */
    CImObjectFactoryImp* iImObjFactory;
    CProtocolImConversationDataHostImp* iConvDataHost;
    CProtocolImGroupDataHostImp* iGroupDataHost;
    CProtocolImInvitationDataHostImp* iInvitationDataHost;
    CProtocolImSearchDataHostImp* iSearchDataHost;


    };


#endif // CPROTOCOLIMDATAHOSTIMP_H

