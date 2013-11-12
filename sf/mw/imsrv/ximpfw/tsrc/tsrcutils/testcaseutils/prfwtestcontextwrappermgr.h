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
* Description:  Testing context wrapper
 *
*/


#ifndef CPRFWTESTCONTEXTWRAPPERMGR_H
#define CPRFWTESTCONTEXTWRAPPERMGR_H 

#include <e32base.h>
#include <ximpbase.h>

#include "prfwteststatuseventlistener.h"

class MXIMPClient;
class CXIMPTestContextWrapper;
class MXIMPContext;
class CXIMPTestStatusEventListener;
class CXIMPTestMessenger;

/**
 * Manager for wrappers.
 * Owns the wrappers and provides convenience methods for
 * test code.
 *
 * @since S60 v4.0
 */
class CXIMPTestContextWrapperMgr : public CBase
    {
    public:

        IMPORT_C static CXIMPTestContextWrapperMgr* NewL();
        virtual ~CXIMPTestContextWrapperMgr();

    private:

        CXIMPTestContextWrapperMgr();
        void ConstructL();

    public:

        /**
         * Creates a wrapper. The presence client can be given for wrappers
         * which share a presence client.
         * @param aClientIndex Use the client from this wrapper
         * If aClientIndex is KErrNotFound, creates a new client.
         */
        IMPORT_C void CreateWrapperL( TInt aClientIndex = KErrNotFound );

        /**
         * Direct access to a wrapper. Ownership NOT transferred.
         * @param aIndex The index of desired wrapper
         */
        IMPORT_C CXIMPTestContextWrapper* GetWrapperL( TInt aIndex );

        /**
         * Count of wrappers
         */         
        IMPORT_C TInt WrapperCount() const;

    private: // data

        RXIMPObjOwningPtrArray<CXIMPTestContextWrapper> iWrappers;
        RXIMPObjOwningPtrArray<MXIMPClient> iPrClients;

        TInt iIndex;
    };

#endif // CPRFWTESTCONTEXTWRAPPERMGR_H 
