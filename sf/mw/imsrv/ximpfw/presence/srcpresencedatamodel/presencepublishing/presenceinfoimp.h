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
* Description:  MPresenceInfo API object implementation.
*
*/

#ifndef CPRESENCEINFOIMP_H
#define CPRESENCEINFOIMP_H

#include "ximpapidataobjbase.h"
#include "presenceapiobjbase.h"
#include <presenceinfo.h>
#include <s32strm.h>
#include "presencetypehelpers.h"

class CPersonPresenceInfoImp;
class CServicePresenceInfoImp;
class CDevicePresenceInfoImp;
class CPresenceInfoFilterImp;


/**
 * MPresenceInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceInfoImp ): public CXIMPApiDataObjBase,
                                           public MPresenceInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEINFOIMP };


public:

    IMPORT_C static CPresenceInfoImp* NewLC();
    IMPORT_C static CPresenceInfoImp* NewL();
    virtual ~CPresenceInfoImp();

private:

    CPresenceInfoImp();
    void ConstructL();

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: // From CXIMPApiDataObjBase

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS

public: // From MPresenceInfo

    const MPersonPresenceInfo* PersonPresence() const;
    MPersonPresenceInfo* PersonPresence();
    void SetPersonPresenceL( MPersonPresenceInfo* aPersonPresence );

    TInt ServicePresenceCount() const;
    const MServicePresenceInfo& ServicePresenceAt( TInt aIndex ) const;
    void LookupServicePresenceByServiceType( MServicePresenceInfo*& aServicePresence,
                                             const TDesC8& aMatch ) const;
    void AddServicePresenceL( MServicePresenceInfo* aServicePresence );

    TInt DevicePresenceCount() const;
    const MDevicePresenceInfo& DevicePresenceAt( TInt aIndex ) const;
    void AddDevicePresenceL( MDevicePresenceInfo* aDevicePresence );

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Creates and returns a filtered copy of this class, according
     * to the given filter.
     * Ownership transferred.
     * @param aFilter The filter to use
     * @return New filtered info imp instance
     */
    IMPORT_C CPresenceInfoImp* FilteredInfoLC(
            const CPresenceInfoFilterImp& aFilter
            ) const;

private: // data


    /**
     * OWN: 
     */
    CPersonPresenceInfoImp* iPersonPresence;

    /**
     * OWN: 
     */
    RPrServicePrInfoArray iServicePresences;

    /**
     * OWN: 
     */
    RPrDevicePrInfoArray iDevicePresences;

    };


#endif // CPRESENCEINFOIMP_H
