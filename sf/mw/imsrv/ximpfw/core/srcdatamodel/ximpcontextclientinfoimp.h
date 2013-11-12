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
* Description:  MXIMPContextClientInfo API object implementation.
*
*/

#ifndef CXIMPPRESENCECONTEXTCLIENTINFOIMP_H
#define CXIMPPRESENCECONTEXTCLIENTINFOIMP_H

#include <s32strm.h>

#include "ximpapidataobjbase.h"
#include <ximpcontextclientinfo.h>


class CXIMPIdentityImp;


/**
 * MXIMPContextClientInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextClientInfoImp ): public CXIMPApiDataObjBase,
                                                        public MXIMPContextClientInfo
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPCONTEXTCLIENTINFOIMP };

public:

    IMPORT_C static CXIMPContextClientInfoImp* NewLC();
    IMPORT_C static CXIMPContextClientInfoImp* NewL();

    virtual ~CXIMPContextClientInfoImp();

private:

    CXIMPContextClientInfoImp();
    void ConstructL( const TDesC16& aClientId );
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


public: // From MXIMPContextClientInfo

    const MXIMPIdentity& ClientId() const;
    void SetClientIdL( MXIMPIdentity* aIdentity );
    

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


    /**
     * Tests if the given CXIMPContextClientInfoImp object matches
     * this one.
     * @param aServiceInfoImp An Object to match
     */
    IMPORT_C TBool Match( const CXIMPContextClientInfoImp* aClientInfoImp ) const;

private: // data

    /**
     * ClientId
     */
    CXIMPIdentityImp* iClientId;
    };


#endif // CXIMPPRESENCECONTEXTCLIENTINFOIMP_H
