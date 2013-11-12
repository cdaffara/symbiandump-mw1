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
* Description:  MXIMPIdentity API object implementation.
*
*/

#ifndef CXIMPIDENTITYIMP_H
#define CXIMPIDENTITYIMP_H

#include "ximpapidataobjbase.h"
//#include "presenceinfoimp.h"
#include "ximpidentity.h"
#include <s32strm.h>//needed?


/**
 * MXIMPIdentity API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPIdentityImp ): public CXIMPApiDataObjBase,
                                         public MXIMPIdentity
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPIDENTITYIMP };

public:

    IMPORT_C static CXIMPIdentityImp* NewLC();
    IMPORT_C static CXIMPIdentityImp* NewL();
    IMPORT_C static CXIMPIdentityImp* NewLC( const TDesC16& aIdentity );
    virtual ~CXIMPIdentityImp();

private:

    CXIMPIdentityImp();
    void ConstructL();
    void ConstructL( const TDesC16& aIdentity );
    
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

public: // From MXIMPIdentity

    /**
     * Implementation of MXIMPIdentity interface methods
     * @see MXIMPIdentity
     */
    /*const TDesC8& PresentityUri() const;
    const TDesC16& DisplayName() const;
    const TDesC16& ProtocolAddress() const;
    MPresenceInfo& PresenceInfo();*/

    void SetIdentityL( const TDesC16& aIdentity );
    const TDesC16& Identity() const;

public: // New functions

    /**
     * Extenalizes object data to given stream.
     * @param aStream Stream to write.
     */
    /* IMPORT_C void ExternalizeL( RWriteStream& aStream ) const; */


    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Compare identities.
     * @param aOther Compared identity
     * @return ETrue if identities are different. EFalse if same.
     */
    IMPORT_C TBool operator!=( const CXIMPIdentityImp& aOther ) const;

    /**
     * Compare identities.
     * @param aOther Compared identity
     * @return -1 if aOther is greater than this
     *          0 if they are same
     *          1 if this if greater than other.
     */
    IMPORT_C TInt Compare( const CXIMPIdentityImp& aOther ) const;

private: // data

    /**
     *
     */
    RBuf16 iIdentity;

    };


#endif // CXIMPIDENTITYIMP_H
