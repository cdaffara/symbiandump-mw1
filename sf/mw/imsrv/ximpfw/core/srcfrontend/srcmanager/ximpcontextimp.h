/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CXIMPPRESENCECONTEXTIMP_H
#define CXIMPPRESENCECONTEXTIMP_H

#include <ximpcontext.h>
#include "ximpcontextinternal.h"

#include "ximpapiobjbase.h"


class MXIMPContextObserver;
class CXIMPContextEventPump;
class MXIMPFeatureInfo;
class CXIMPObjectFactoryImp;
class CXIMPContextClientProxy;


/**
 * MXIMPContext API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextImp ): public CXIMPApiObjBase,
                                      public MXIMPContext,
                                      public MXIMPContextInternal
    {
    public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPCONTEXTIMP };


public:

    static CXIMPContextImp* NewLC();
    virtual ~CXIMPContextImp();


private:

    CXIMPContextImp();
    void ConstructL();


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MXIMPContext

    /**
     * Implementation of MXIMPContext interface methods
     * @see MXIMPContext
     */
    void RegisterObserverL( MXIMPContextObserver& aObserver,
                            const TArray<TInt32>* aEventFilter );

    void UnregisterObserver( MXIMPContextObserver& aObserver );

    TXIMPRequestId BindToL( TUid aProtocolImpUid,
                            const TDesC16& aServiceAddress,
                            const TDesC16& aUsername,
                            const TDesC16& aPassword,
                            TInt32 aIapId );
    TXIMPRequestId BindToL( TUid aProtocolImpUid,
                            const TDesC16& aServiceAddress,
                            const TDesC16& aUsername,
                            const TDesC16& aPassword,
                            const TDesC16& aClientId,
                            TInt32 aIapId );
    TXIMPRequestId BindToL( TUid aProtocolImpUid,
                            TInt aSettingsId );
    TXIMPRequestId BindToL( TUid aProtocolImpUid,
			                const TDesC16& aUsername,
			                const TDesC16& aPassword,
			                TInt aSettingsId ) ;                        

    TXIMPRequestId UnbindL();

    MXIMPFeatureInfo* GetContextFeaturesLC() const;

    MXIMPObjectFactory& ObjectFactory() const;
    
public: // From MXIMPContextInternal

    TXIMPRequestId QueueOperationL( TInt aOperationType, 
                                const TDesC8& aOperationParams );


private: // Helpers

    /**
     * Converts a string to a uid.
     * String must be in Hex and begin with "0x".
     */
    void ConvertStringToUidL( const TDesC8& aData, TUid& aUid );


private: // data

    /**
     * Context session proxy for having a connection context server.
     */
    CXIMPContextClientProxy* iCtxProxy;

    /**
     * Object factory sub interface.
     */
    CXIMPObjectFactoryImp* iObjFactory;

    /**
     * Event pump.
     * Own.
     */
    CXIMPContextEventPump* iEventPump;
    };


#endif // CXIMPPRESENCECONTEXTIMP_H
