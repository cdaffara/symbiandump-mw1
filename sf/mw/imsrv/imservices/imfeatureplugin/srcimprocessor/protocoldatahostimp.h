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

#ifndef CPROTOCOLDATAHOSTIMP_H
#define CPROTOCOLDATAHOSTIMP_H

#include <e32base.h>
//#include <ximpprotocolconnectionhost.h>
//#include <ximpcontext.h>
#include <ximpbase.h>
#include <protocoldatahost.h>
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
class CXIMPOperationBase ;

class CProtocolImDataHostImp ;

/**
 * Im connection host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
//class CProtocolImDataHostImp : public CXIMPApiObjBase,
//                                     public MProtocolImDataHost
NONSHARABLE_CLASS(CProtocolDataHostImp): public MProtocolDataHost
    {
public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CPROTOCOLIMDATAHOSTIMP };

public:

    IMPORT_C static CProtocolDataHostImp* NewL( MXIMPHost& aHost );

    IMPORT_C static CProtocolDataHostImp* NewLC( MXIMPHost& aHost );

    virtual ~CProtocolDataHostImp();

private:

    CProtocolDataHostImp( MXIMPHost& aHost );
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    //XIMPIMP_DECLARE_IF_BASE_METHODS


public: // MProtocolImDataHost
   
    void SetHostToProtocol() ; 
    TAny* GetInterface(TInt aInterfaceId) ;
    TInt GetCleanupOperation() ;


private: // data
	// not owned
    MXIMPHost& iHost;
    
    //CImObjectFactoryImp* iImObjFactory;
    
    //CProtocolImConversationDataHostImp* iConvDataHost;
    
    CProtocolImDataHostImp *iImDataHost ;

    };


#endif // CPROTOCOLDATAHOSTIMP_H

