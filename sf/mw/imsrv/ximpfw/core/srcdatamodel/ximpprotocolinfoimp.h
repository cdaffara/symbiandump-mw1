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
* Description:  MXIMPProtocolInfo API object implementation.
*
*/

#ifndef CXIMPPROTOCOLINFOIMP_H
#define CXIMPPROTOCOLINFOIMP_H

#include "ximpapidataobjbase.h"
#include <ximpprotocolinfo.h>
#include <s32strm.h>


class MXIMPFeatureInfo;
class CXIMPFeatureInfoImp;


/**
 * MXIMPProtocolInfo API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPProtocolInfoImp ): public CXIMPApiDataObjBase,
                                           public MXIMPProtocolInfo
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPPROTOCOLINFOIMP };

public:

    IMPORT_C static CXIMPProtocolInfoImp* NewLC();

    IMPORT_C static CXIMPProtocolInfoImp* NewLC(
                const TDesC16& aDisplayName,
                TUid aImplementationUid,
                const CXIMPFeatureInfoImp& aFeatures );

    virtual ~CXIMPProtocolInfoImp();

private:

    CXIMPProtocolInfoImp();
    void ConstructL( const TDesC16& aDisplayName,
                     TUid aImplementationUid,
                     const CXIMPFeatureInfoImp& aFeatures );
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

public: // From MXIMPProtocolInfo

    /**
     * Implementation of MXIMPProtocolInfo interface methods
     * @see MXIMPProtocolInfo
     */
    const TDesC16& DisplayName() const;
    TUid ImplementationUid() const;
    MXIMPFeatureInfo* GetProtocolFeaturesLC() const;



public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


private: // data

    /**
     * Protocol plug-in display name.
     * Own.
     */
    RBuf iDisplayName;


    /**
     * Protocol plug-in implementation uid.
     * Own.
     */
    TUid iImplementationUid;


    /**
     * Protocol plug-in feature info.
     * Own.
     */
    CXIMPFeatureInfoImp* iFeatures;

    };


#endif // CXIMPPROTOCOLINFOIMP_H


