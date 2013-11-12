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
* Description:  MXIMPContextState API object implementation.
*
*/

#ifndef CXIMPPRESENCECONTEXTSTATEIMP_H
#define CXIMPPRESENCECONTEXTSTATEIMP_H

#include "ximpapidataobjbase.h"
#include <ximpcontextstate.h>
#include <s32strm.h>


/**
 * MXIMPContextState API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextStateImp ): public CXIMPApiDataObjBase,
                                                   public MXIMPContextState
    {
public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPCONTEXTSTATEIMP };

public:

    IMPORT_C static CXIMPContextStateImp* NewLC();
    IMPORT_C static CXIMPContextStateImp* NewL();
    virtual ~CXIMPContextStateImp();


private:

    CXIMPContextStateImp();
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


public: // From MXIMPContextState

    /**
     * Implementation of MXIMPContextState interface methods
     * @see MXIMPContextState
     */
    MXIMPContextState::TState ContextState() const;



public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


    /**
     * 
     * 
     */
    IMPORT_C void SetStateValue( MXIMPContextState::TState aState );


private: // data


    /**
     * Result code.
     * Own.
     */
    TState iContextState;

    };


#endif // CXIMPPRESENCECONTEXTSTATEIMP_H


