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
* Description:  Implementation of MServicePresenceInfo object.
 *
*/


#ifndef CSERVICEPRESENCEINFOIMP_H
#define CSERVICEPRESENCEINFOIMP_H

#include <e32std.h>
#include <s32strm.h>
#include <ximpbase.h>
#include <servicepresenceinfo.h>
#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"

class CPresenceInfoFieldCollectionImp;
class CPresenceInfoFilterImp;

/**
 * Implementation of MServicePresenceInfo object.
 *
 *
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CServicePresenceInfoImp ): public CXIMPApiDataObjBase,
                                                  public MServicePresenceInfo
    {
    public:

    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPERSONPRESENCEINFOIMP };

public:

    IMPORT_C static CServicePresenceInfoImp* NewLC();

    virtual ~CServicePresenceInfoImp();

private:

    CServicePresenceInfoImp();
    void ConstructL();

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

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


public: // from MServicePresenceInfo

    /**
     * @see MServicePresenceInfo
     */
    const TPtrC8 ServiceType() const;
    void SetServiceTypeL( const TDesC8& aServiceType );
    const MPresenceInfoFieldCollection& Fields() const;
    MPresenceInfoFieldCollection& Fields();

public: // new methods

    /**
     * Remove from this info field implementation the fields which do not match
     * the given filter.
     * @param aFilter The filter implementation
     */
    void FilterWithL( const CPresenceInfoFilterImp& aFilter );

private: // data

    // owned
    CPresenceInfoFieldCollectionImp* iFields;

    RBuf8 iServiceType;

    };


#endif // CSERVICEPRESENCEINFOIMP_H



