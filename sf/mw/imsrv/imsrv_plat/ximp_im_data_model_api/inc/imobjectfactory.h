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
* Description:  Interface for IM Feature Plugin object factory.
*
*/

#ifndef MIMOBJECTFACTORY_H
#define MIMOBJECTFACTORY_H

#include <ximpbase.h>
#include <ximpidentity.h>
#include <bamdesca.h>
#include <imdatamodelifids.hrh>

class MImGroupInfo;
class MImConversationInfo;
class MImInvitationInfo;
class MImSearchInfo;

/**
 * Interface for IM Feature Plugin IM object factory.
 *
 * IM object factory is used to instantiate
 * data objects declared in data model API.
 *
 * MImObjectFactory interface can be obtained from
 * MXIMPContext or MXIMPProtocolConnectionHost
 * interfaces.
 *
 * @ingroup imdatamodelapi
 * @since S60 
 */
class MImObjectFactory : public MXIMPBase
    {
public:

    /** Interface ID for the MImObjectFactory. */
    enum { KInterfaceId = IM_IF_ID_IM_OBJECT_FACTORY };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MImObjectFactory() {}




public: // Group and similar info object types


    /**
     * Instantiates new IM group info object.
     *
     * @return New IM group info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    //virtual MImGroupInfo* NewImGroupInfoLC() = 0;


    /**
     * Instantiates new im conversation info object.
     *
     * @return new im conversation info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MImConversationInfo* NewImConversationInfoLC() = 0;


    /**
     * Instantiates new im invitation info object.
     *
     * @return new im invitation info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    //virtual MImInvitationInfo* NewImInvitationInfoLC() = 0;


    /**
     * Instantiates new im search info object.
     *
     * @return im search info info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    //virtual MImSearchInfo* NewImSearchInfoLC() = 0;

    };



#endif // MIMOBJECTFACTORY_H



