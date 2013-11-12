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
* Description:  MXIMPClient API object implementation.
*
*/

#ifndef CXIMPCLIENTIMP_H
#define CXIMPCLIENTIMP_H

#include <ximpclient.h>
#include "ximpapiobjbase.h"


class CXIMPClientIfSingleton;
class MXIMPFeatureInfo;


/**
 * MXIMPClient API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPClientImp ): public CXIMPApiObjBase,
                                     public MXIMPClient
    {
    public:
    /** The class ID. */
    enum { KClassId = XIMPIMP_CLSID_CXIMPCLIENTIMP };


public:

    IMPORT_C static MXIMPClient* NewMXIMPClientL();
    virtual ~CXIMPClientImp();

private:

    CXIMPClientImp();
    void ConstructL();



public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MXIMPClient


    /**
     * Implementation of MXIMPClient interface methods
     * @see MXIMPClient
     */
    void GetProtocolsL( RXIMPObjOwningPtrArray< MXIMPProtocolInfo >& aProtocols );
    MXIMPFeatureInfo* GetFrameworkFeaturesLC();
    MXIMPContext* NewPresenceContextLC();    



private: // data

    /**
     * XIMP client side singleton.
     * Own.
     */
    CXIMPClientIfSingleton* iSingleton;

    };


#endif // CXIMPCLIENTIMP_H
