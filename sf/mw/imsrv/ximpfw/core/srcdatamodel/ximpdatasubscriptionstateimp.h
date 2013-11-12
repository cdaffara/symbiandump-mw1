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
* Description:  MXIMPDataSubscriptionState API object implementation.
*
*/

#ifndef CXIMPDATASUBSCRIPTIONSTATEIMP_H
#define CXIMPDATASUBSCRIPTIONSTATEIMP_H

#include "ximpapidataobjbase.h"
#include <ximpdatasubscriptionstate.h>
#include <s32strm.h>

/**
 * MXIMPDataSubscriptionState API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPDataSubscriptionStateImp ): public CXIMPApiDataObjBase,
                                                    public MXIMPDataSubscriptionState
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPDATASUBSCRIPTIONSTATEIMP };

public:

    /**
     * Construction methods.
     */
    IMPORT_C static CXIMPDataSubscriptionStateImp* NewLC();
    IMPORT_C static CXIMPDataSubscriptionStateImp* NewL();
    
    /**
     * Destruction
     */
    virtual ~CXIMPDataSubscriptionStateImp();

private:

    /**
     * Construction
     */
    CXIMPDataSubscriptionStateImp();
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


public: // From MXIMPDataSubscriptionState

    /**
     * Implementation of MXIMPDataSubscriptionState interface methods
     * @see MXIMPDataSubscriptionState
     */
    MXIMPDataSubscriptionState::TSubscriptionState SubscriptionState() const;
    
    /**
     * Implementation of MXIMPDataSubscriptionState interface methods
     * @see MXIMPDataSubscriptionState
     */
    MXIMPDataSubscriptionState::TDataState DataState() const;

    /**
     * Implementation of MXIMPDataSubscriptionState interface methods
     * @see MXIMPDataSubscriptionState
     */
    void SetSubscriptionStateL( MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState );

    /**
     * Implementation of MXIMPDataSubscriptionState interface methods
     * @see MXIMPDataSubscriptionState
     */
    void SetDataStateL( MXIMPDataSubscriptionState::TDataState aDataState );

public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Set subscriptionstate for internal usage.
     * Created as non leaving version of method.
     * @param aSubscriptionState New subscription state for state object.
     */
    void SetSubscriptionState( MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState );
    
    /**
     * Set data state for internal use.
     * Created as non leaving version of method.
     * @param aDataState New data state for state object.
     */
    void SetDataState( MXIMPDataSubscriptionState::TDataState aDataState );

private: // data


    /**
     * Subscription state.
     * Own.
     */
    TSubscriptionState iSubscriptionState;


    /**
     * Data state.
     * Own.
     */
    TDataState iDataState;
    };


#endif // CXIMPDATASUBSCRIPTIONSTATEIMP_H


