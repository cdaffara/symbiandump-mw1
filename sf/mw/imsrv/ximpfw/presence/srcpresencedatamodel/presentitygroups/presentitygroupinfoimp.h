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

#ifndef CPRESENTITYGROUPINFOIMP_H
#define CPRESENTITYGROUPINFOIMP_H

#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include <presentitygroupinfo.h>

#include <e32std.h>
#include <s32strm.h>

class CXIMPIdentityImp;

/**
 * MPresenceInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresentityGroupInfoImp ): public CXIMPApiDataObjBase,
                                                  public MPresentityGroupInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENTITYGROUPINFOIMP };

public:

    IMPORT_C static CPresentityGroupInfoImp* NewLC( 
                    const MXIMPIdentity& aIdentity, 
                    const TDesC16& aDisplayName );
    IMPORT_C static CPresentityGroupInfoImp* NewLC();
    IMPORT_C static CPresentityGroupInfoImp* NewL();
    virtual ~CPresentityGroupInfoImp();

private:

    CPresentityGroupInfoImp();
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


public: // From MPresentityGroupInfo

    const MXIMPIdentity& GroupId() const;
    const TDesC16& GroupDisplayName() const;
    void SetGroupIdL( MXIMPIdentity* aIdentity );
    void SetGroupDisplayNameL( const TDesC16& aDisplayName );    
    
    
public: // New functions

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
            const CPresentityGroupInfoImp& aA,
            const CPresentityGroupInfoImp& aB );
            
    /**
     * Access to identity object
     * @return Identity
     */
    IMPORT_C const CXIMPIdentityImp& Identity() const;

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


#endif // CPRESENTITYGROUPINFOIMP_H
