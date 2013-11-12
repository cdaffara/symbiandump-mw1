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
* Description:  MPresenceBlockInfo API object implementation.
*
*/

#ifndef CPRESENCEBLOCKINFOIMP_H
#define CPRESENCEBLOCKINFOIMP_H

#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include <s32strm.h>
#include <presenceblockinfo.h>

class CXIMPIdentityImp;

/**
 * MPresenceBlockInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceBlockInfoImp ): public CXIMPApiDataObjBase,
                                                public MPresenceBlockInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEBLOCKINFOIMP };

public:

    IMPORT_C static CPresenceBlockInfoImp* NewLC(
                    const MXIMPIdentity& aIdentity,
                    const TDesC16& aDisplayName );
    IMPORT_C static CPresenceBlockInfoImp* NewLC();
    IMPORT_C static CPresenceBlockInfoImp* NewL();
    virtual ~CPresenceBlockInfoImp();

private:

    CPresenceBlockInfoImp();
    void ConstructL();
    void ConstructL( const MXIMPIdentity& aIdentity,
                     const TDesC16& aDisplayName );

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS
    
public: // From MPresenceBlockInfo

    const MXIMPIdentity& BlockedEntityId() const;
    const TDesC16& BlockedEntityDisplayName() const;
    void SetBlockedEntityIdL( MXIMPIdentity* aIdentity );
    void SetBlockedEntityDisplayNameL( const TDesC16& aDisplayName );
    
    
public: // From CXIMPApiDataObjBase

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS
    
public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Set identity
     */
    IMPORT_C void SetIdentity( CXIMPIdentityImp* aIdentity );

    /**
     * Set display name
     */
    IMPORT_C void SetDisplayName( HBufC16* aDisplayName );

    /**
     * TLinearOrder for ordering based on stored group id,
     * when this class is used within an RPointerArray or derivatives.
     */
    IMPORT_C static TInt IdLinearOrder(
            const CPresenceBlockInfoImp& aA,
            const CPresenceBlockInfoImp& aB );

    /**
     * Access to identity object
     * @return Identity
     */
    IMPORT_C const CXIMPIdentityImp& IdentityImp() const;

private: // data

    /**
     * Identity
     */
    CXIMPIdentityImp* iIdentity;

    /**
     * Displayname
     */
    RBuf16 iDisplayName;

    };


#endif // CPRESENCEBLOCKINFOIMP_H
