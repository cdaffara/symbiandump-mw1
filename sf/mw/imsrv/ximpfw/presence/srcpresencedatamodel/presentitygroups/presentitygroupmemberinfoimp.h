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
* Description:  MPresentityGroupMemberInfo API object implementation.
*
*/

#ifndef CPRESENTITYGROUPMEMBERINFOIMP_H
#define CPRESENTITYGROUPMEMBERINFOIMP_H

#include <e32std.h>
#include <s32strm.h>

#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include <presentitygroupmemberinfo.h>

class CXIMPIdentityImp;

/**
 * MPresenceInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityGroupMemberInfoImp ): public CXIMPApiDataObjBase,
                                                  public MPresentityGroupMemberInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPMEMBERINFOIMP };

public:

    IMPORT_C static CPresentityGroupMemberInfoImp* NewLC(
                    const MXIMPIdentity& aIdentity,
                    const TDesC16& aDisplayName );
    IMPORT_C static CPresentityGroupMemberInfoImp* NewLC();
    IMPORT_C static CPresentityGroupMemberInfoImp* NewL();
    virtual ~CPresentityGroupMemberInfoImp();

private:

    CPresentityGroupMemberInfoImp();
    void ConstructL();
    void ConstructL( const MXIMPIdentity& aIdentity,
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

public: // From MPresentityGroupMemberInfo

    const MXIMPIdentity& GroupMemberId() const;
    const TDesC16& GroupMemberDisplayName() const;
    void SetGroupMemberIdL( MXIMPIdentity* aIdentity );
    void SetGroupMemberDisplayNameL( const TDesC16& aDisplayName );


public: // New functions

    /**
     * Extenalizes object data to given stream.
     * @param aStream Stream to write.
     */
    //IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;


    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


    /**
     * TLinearOrder for ordering based on stored group id,
     * when this class is used within an RPointerArray or derivatives.
     */
    IMPORT_C static TInt GroupIdLinearOrder(
            const CPresentityGroupMemberInfoImp& aA,
            const CPresentityGroupMemberInfoImp& aB );
            
    /**
     * Access to identity object
     * @return Identity
     */
    IMPORT_C const CXIMPIdentityImp& IdentityImp() const;            

private: // data

    /**
     * Identity
     * Own.
     */
    CXIMPIdentityImp* iIdentity;

    /**
     * Displayname
     * Own.
     */
    RBuf16 iDisplayName;

    };


#endif // CPRESENTITYGROUPMEMBERINFOIMP_H
