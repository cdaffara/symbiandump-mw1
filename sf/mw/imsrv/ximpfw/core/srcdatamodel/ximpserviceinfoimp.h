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
* Description:  MXIMPServiceInfo API object implementation.
*
*/

#ifndef CXIMPSERVICEINFOIMP_H
#define CXIMPSERVICEINFOIMP_H


#include "ximpapidataobjbase.h"
#include <ximpserviceinfo.h>
#include <s32strm.h>

/**
 * MXIMPServiceInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPServiceInfoImp ): public CXIMPApiDataObjBase,
                                          public MXIMPServiceInfo
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPSERVICEINFOIMP };


public:

    IMPORT_C static CXIMPServiceInfoImp* NewLC();
    IMPORT_C static CXIMPServiceInfoImp* NewL();
    IMPORT_C static CXIMPServiceInfoImp* NewLC( TUid aProtocolImplUid,
                                                const TDesC16& aServiceAddress,
                                                const TDesC16& aUsername,
                                                const TDesC16& aPassword,
                                                TInt32 aIapId );
    virtual ~CXIMPServiceInfoImp();

private:

    CXIMPServiceInfoImp();
    void ConstructL();
    void ConstructL( TUid aProtocolImplUid,
                     const TDesC16& aServiceAddress,
                     const TDesC16& aUsername,
                     const TDesC16& aPassword,
                     TInt32 aIapId );
    
    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_PRIV_METHODS

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS

public: // From MXIMPServiceInfo

    /**
     * Implementation of MXIMPServiceInfo interface methods
     * @see MXIMPServiceInfo
     */
    const TDesC16& ServiceAddress() const;
    const TDesC16& UserId() const;
    const TDesC16& Password() const;
    TInt32 IapId() const;
    TUid ProtocolImplUid() const;


public: // New functions


    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

private: // data

    /**
     * Service address
     * Own.
     */
    RBuf   iAddress;

    /**
     * User ID
     * Own.
     */
    RBuf   iUserId;

    /**
     * Password
     * Own.
     */
    RBuf   iPassword;

    /**
     * Protocol Uid
     */
    TUid iProtocolUid;

    /**
     * IAP id
     */
    TInt32 iIapId;

    };


#endif // CXIMPSERVICEINFOIMP_H
