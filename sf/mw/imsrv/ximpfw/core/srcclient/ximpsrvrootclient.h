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
* Description:  XIMP Framework server root session.
*
*/

#ifndef XIMPSRVROOTCLIENT_H__
#define XIMPSRVROOTCLIENT_H__

#include <ximpbase.h>
#include <e32std.h>


/**
 * XIMP Framework server root session.
 *
 * @lib ximpsrvclient.dll
 * @since S60 v3.2
 */
class RXIMPSrvRootClient : public RSessionBase
    {
public:

    /**
     * Default constructor.
     */
    IMPORT_C RXIMPSrvRootClient();


    /**
     * Session connection method.
     *
     * @return Error code from creating server connection.
     */
    IMPORT_C TInt Connect();


    /**
     * Closes server connection.
     */
    IMPORT_C void Close();


    /**
     * Retrieves a list of known protocols and their details.
     *
     * @return Externalized version of
     * RXIMPObjOwningPtrArray< CXIMPProtocolInfoImp >, containing the
     * protocols. Ownership transferred to caller.
     */
    IMPORT_C HBufC8* GetKnownProtocolsL();


    /**
     * Gets supported framework features.
     *
     * @return Supported framework features, externalized
     * CXIMPFeatureInfoImp. Ownership transferred to caller.
     */
    IMPORT_C HBufC8* GetSupportedFrameworkFeaturesL();

private:

    /**
     * Helper to get data from server side.
     * @return Externalized object to be passed onward
     */
    HBufC8* GetDataL( TInt aMsg );


    };


#endif      //  XIMPSRVROOTCLIENT_H__



