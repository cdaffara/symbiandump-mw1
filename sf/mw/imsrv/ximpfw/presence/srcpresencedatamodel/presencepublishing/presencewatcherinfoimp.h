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
* Description:  MPresentityGroupInfo API object implementation.
*
*/

#ifndef CPRESENTITYWATCHERINFOIMP_H
#define CPRESENTITYWATCHERINFOIMP_H

#include <e32std.h>
#include <s32strm.h>
#include <presencewatcherinfo.h>
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"


class CXIMPIdentityImp;

/**
 * MPresenceInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceWatcherInfoImp ): public CXIMPApiDataObjBase,
                                                  public MPresenceWatcherInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEWATCHERINFOIMP };

public:

    IMPORT_C static CPresenceWatcherInfoImp* NewLC(
                    const MPresenceWatcherInfo::TWatcherType aType,
                    const MXIMPIdentity& aIdentity,
                    const TDesC16& aDisplayName );
    IMPORT_C static CPresenceWatcherInfoImp* NewLC();
    IMPORT_C static CPresenceWatcherInfoImp* NewL();
    virtual ~CPresenceWatcherInfoImp();

private:

    CPresenceWatcherInfoImp();
    void ConstructL();
    void ConstructL( const MPresenceWatcherInfo::TWatcherType aType,
                     const MXIMPIdentity& aIdentity,
                     const TDesC16& aDisplayName );
    
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

public: // From MPresenceWatcherInfo

    const MXIMPIdentity& WatcherId() const;
    const TDesC16& WatcherDisplayName() const;
    MPresenceWatcherInfo::TWatcherType WatcherType() const;
    void SetWatcherIdL( MXIMPIdentity* aIdentity );
    void SetWatcherDisplayNameL( const TDesC16& aDisplayName );
    void SetWatcherTypeL( MPresenceWatcherInfo::TWatcherType aType );    
    
    


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
     * Set identity
     */
    IMPORT_C void SetDisplayName( HBufC16* aDisplayName );

    /**
     * TLinearOrder for ordering based on stored group id,
     * when this class is used within an RPointerArray or derivatives.
     */
    IMPORT_C static TInt IdLinearOrder(
            const CPresenceWatcherInfoImp& aA,
            const CPresenceWatcherInfoImp& aB );

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

    /**
     * Watcher type
     */
    MPresenceWatcherInfo::TWatcherType iType;
    
    };


#endif // CPRESENTITYWATCHERINFOIMP_H
