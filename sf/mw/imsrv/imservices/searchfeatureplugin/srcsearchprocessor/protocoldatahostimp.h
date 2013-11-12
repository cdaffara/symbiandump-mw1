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
* Description:  protcol datahost imp header
 *
*/

#ifndef CPROTOCOLDATAHOSTIMP_H
#define CPROTOCOLDATAHOSTIMP_H

#include <e32base.h>
#include <ximpbase.h>
#include <protocoldatahost.h>
#include "searchapiobjbase.h"
#include "ximpapiobjbase.h"

#include "ximphost.h"

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

class CProtocolSearchDataHostImp ;

/**
 * protocol data host for serach
 *
 * @lib searchprocessor.dll
 */

NONSHARABLE_CLASS(CProtocolDataHostImp): public MProtocolDataHost
    {
    
	public:
	    /** The class ID. */
	    enum { KClassId = IMP_CLSID_CPROTOCOLSEARCHDATAHOSTIMP };

	public:
        /**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CProtocolDataHostImp* NewL( MXIMPHost& aHost );
	    
		/**
    	 * Construction :NewL
     	 */
	    IMPORT_C static CProtocolDataHostImp* NewLC( MXIMPHost& aHost );
	    
	    /**
         * Destructor
         */
	    virtual ~CProtocolDataHostImp();

	private:
        /**
	     * constructor
	     */
	    CProtocolDataHostImp( MXIMPHost& aHost );
	    
	    /**
	     * 2nd phase constructor
	     */
	    void ConstructL();

	public: // MProtocolDataHost 
	    /**
	     * 
	     * @see MProtocolDataHost 
	     */
	    void SetHostToProtocol() ; 
	    /**
	     * 
	     * @see MProtocolDataHost 
	     */
	    TAny* GetInterface(TInt aInterfaceId) ;
	    /**
	     * 
	     * @see MProtocolDataHost 
	     */
	    
	    TInt GetCleanupOperation() ;


	private: 
		/**
	     * Access to host. Not owned
	     */
	    MXIMPHost& iHost;
	    /**
	     * CProtocolSearchDataHostImp : owned
	     */
	    CProtocolSearchDataHostImp *iSearchDataHost ;

    };


#endif // CPROTOCOLDATAHOSTIMP_H

