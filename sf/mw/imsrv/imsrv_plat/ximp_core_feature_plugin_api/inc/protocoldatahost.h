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
* Description:  interface for protocol hosts
 *
*/

#ifndef MPROTOCOLDATAHOST_H
#define MPROTOCOLDATAHOST_H

#include <ximpbase.h>
#include <ximpfeaturepluginifids.hrh>
#include <e32std.h>
#include <ecom/ecom.h>

class MXIMPHost;

/**
 * protocol host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
class MProtocolDataHost 

    {
public:

    enum { KInterfaceId = XIMP_ECOM_IF_UID_PROTOCOL_DATA_HOST_INTERFACE };

    
public:

    /**
     * Factory method to instantiate MImFeatures.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new protocol client object. Object
     *         ownership is returned to caller.
     */
    static inline MProtocolDataHost* NewL( MXIMPHost& aHost );
    
    static inline MProtocolDataHost* NewL(TUid aImpid, MXIMPHost& aHost );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual inline ~MProtocolDataHost();
    
    /**
     * To get list of all plugins which implements this interface
     */
    inline static void ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray) ;
    
    /**
     * This allows hosts to set themselves to protocol plugins 
     */
    virtual void SetHostToProtocol() = 0 ;

    /**
     * To get the host object pointer of this interface id
     */
    virtual TAny* GetInterface(TInt aInterfaceId) = 0;
    
    /**
     * Hosts can return the clean up id, to create cleanup operation
     */
    virtual TInt GetCleanupOperation() = 0;
    
protected:

    /**
     * Default constructor to zero initialize
     * the iEcomDtorID member.
     */
    inline MProtocolDataHost();
 
private: // Data

    //OWN: ECom destructor ID
    TUid iEcomDtorID;
    };

#include <protocoldatahost.inl> 
    

#endif // MPROTOCOLDATAHOST_H

