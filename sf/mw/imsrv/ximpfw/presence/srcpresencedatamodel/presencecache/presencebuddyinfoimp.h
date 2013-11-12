/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPresenceBuddyInfo API object implementation.
*
*/

#ifndef CXIMPPRESENCEBUDDYINFOIMP_H
#define CXIMPPRESENCEBUDDYINFOIMP_H

#include <presencebuddyinfo.h>

#include "presenceapiobjbase.h"
#include "presencetypehelpers.h"
#include "ximpapiobjbase.h"

class CPresenceInfoImp;
class MPresenceInfo;
class CXIMPIdentityImp;

/**
 * MPresenceBuddyInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceBuddyInfoImp ): public CXIMPApiDataObjBase,
                                            public MPresenceBuddyInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CXIMPPRESENCEBUDDYINFOIMP };


public:

    /**
     * Symbian constructors and c++ destructor
     */
    IMPORT_C static CPresenceBuddyInfoImp* NewLC();
    IMPORT_C static CPresenceBuddyInfoImp* NewL();
    virtual ~CPresenceBuddyInfoImp();

private:

    /**
     * c++ constructor
     */
    CPresenceBuddyInfoImp();
    
    /**
     * Symbian second phase constructor
     */
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

public: // From MPresenceBuddyInfo

    TInt Set( MXIMPIdentity* aIdentity,
              MPresenceInfo* aPresenceInfo);
    TInt SetPresenceInfo(MPresenceInfo* aPresenceInfo);
        
    TInt SetBuddyId(MXIMPIdentity* aIdentity);
                                
    TInt SetPersonPresenceL(MPersonPresenceInfo* aPersonPresenceInfo);                                                        
                                
    const MPresenceInfo* PresenceInfo() const;
    
    const MXIMPIdentity* BuddyId() const;
        
    TBool EqualsIdentity(
                    const MPresenceBuddyInfo* aOtherInstance ) const;

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
private: // data


    /**
     * OWN: 
     */
    CPresenceInfoImp* iPresenceInfo;

    /**
     * OWN: 
     */
    CXIMPIdentityImp* iIdentity;

    };


#endif // CXIMPPRESENCEBUDDYINFOIMP_H
