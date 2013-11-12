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
* Description:  XIMP Framework server root session
 *
*/

#ifndef XIMPROOTSESSION_H
#define XIMPROOTSESSION_H

#include <e32base.h>
#include "ximpsrvsession.h"

class MXIMPSrvMessage;


/**
 * Root session implementation.
 */
class CXIMPRootSession : public CBase,
                         public MXIMPSrvSession
    {
public:     // Constructors and destructors
    static CXIMPRootSession* NewL();
    virtual ~CXIMPRootSession();

protected:  // C++ constructors
    CXIMPRootSession();


public: // From MXIMPSrvSession

    /**
     * @see MXIMPSrvSession
     */
    void TryHandleMessageL( MXIMPSrvMessage& aMessage );



private:

    void DoPrepareProtocolListL( const MXIMPSrvMessage& aMessage );
    void DoPrepareSupportedFeaturesL( const MXIMPSrvMessage& aMessage );
    void DoFetchServerDataL( const MXIMPSrvMessage& aMessage );


private:    // Data

    /**
     * Common buffer for server operations
     */
    HBufC8* iBuffer;

    };

#endif      //  XIMPROOTSESSION_H_


