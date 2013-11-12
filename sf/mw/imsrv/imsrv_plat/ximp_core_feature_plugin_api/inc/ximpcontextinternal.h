/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence context.
*
*/

#ifndef MXIMPCONTEXTINTERNAL_H
#define MXIMPCONTEXTINTERNAL_H

#include <e32std.h>
//#include <ximpbase.h>
#include <ximpmanagementifids.hrh>

/**
 * Internal interface for presence context.
 * This interface is implemented by the XIMP Framework.
 *
 * @ingroup ximpmanagementapi
 * @since S60 v3.2
 */
class MXIMPContextInternal
    {
public:

    /** Interface ID for the MXIMPContext. */
    enum { KInterfaceId = XIMP_IF_ID_CONTEXT_INTERNAL };
    
public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPContextInternal() {}
    
    /**
     * Issues context operation request to server.
     *
     * @param aOperationType In parameter identifying the operation.
     * @param aOperationParams In parameter package for operation.
     * @return Request ID identifying started request.
     */
    virtual TXIMPRequestId QueueOperationL( TInt aOperationType, 
                                const TDesC8& aOperationParams ) = 0;
    
    };


#endif // MXIMPCONTEXTINTERNAL_H



