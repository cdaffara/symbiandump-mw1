/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for XIMP Framework object factory.
*
*/

#ifndef MPRESENCEOBJECTFACTORY_H
#define MPRESENCEOBJECTFACTORY_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>

class MPresenceInfo;
class MPresenceInfoFilter;
class MPresentityGroupInfo;
class MPresenceWatcherInfo;
class MPresenceGrantRequestInfo;
class MPresentityGroupMemberInfo;
class MServicePresenceInfo;
class MPersonPresenceInfo;
class MDevicePresenceInfo;
class MPresenceInfoField;
class MPresenceInfoFieldValueEnum;
class MPresenceInfoFieldValueText;
class MPresenceInfoFieldValueBinary;
class MPresenceBlockInfo;

/**
 * Interface for XIMP Framework presence object factory.
 *
 * Presence object factory is used to instantiate
 * data objects declared in Data Model API.
 *
 * MPresenceObjectFactory interface can be obtained from
 * MPresenceContext or MXIMPProtocolConnectionHost
 * interfaces.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceObjectFactory : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceObjectFactory. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_OBJECT_FACTORY };


protected:

    /**
     * Protected destructor.
     * Object instancies can't be deleted via this interface.
     */
    virtual ~MPresenceObjectFactory() {}




public: // Group and similar info object types


    /**
     * Instantiates new presentity group info object.
     *
     * @return New group info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresentityGroupInfo* NewPresentityGroupInfoLC() = 0;


    /**
     * Instantiates new presentity group member info object.
     *
     * @return New group member info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresentityGroupMemberInfo* NewPresentityGroupMemberInfoLC() = 0;


    /**
     * Instantiates new presence watcher info object.
     *
     * @return New watcher info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceWatcherInfo* NewPresenceWatcherInfoLC() = 0;


    /**
     * Instantiates new presence grant request info object.
     *
     * @return New grant request info object instance.
     *         Object ownership is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceGrantRequestInfo* NewPresenceGrantRequestInfoLC() = 0;



public: // Presence information object types


    /**
     * Instantiates new presence info object.
     *
     * @return New presence info instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfo* NewPresenceInfoLC() = 0;


    /**
     * Instantiates new presence info filter object.
     *
     * @return New presence info filter instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfoFilter* NewPresenceInfoFilterLC() = 0;

    /**
     * Instantiates new presence person info object.
     *
     * @return New presence person info instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPersonPresenceInfo* NewPersonPresenceInfoLC() = 0;

    /**
     * Instantiates new presence service info object.
     *
     * @return New presence service info instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MServicePresenceInfo* NewServicePresenceInfoLC() = 0;

    /**
     * Instantiates new presence device info object.
     *
     * @return New presence device info instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MDevicePresenceInfo* NewDevicePresenceInfoLC() = 0;

    /**
     * Instantiates new info field object.
     *
     * @return New info field instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfoField* NewInfoFieldLC() = 0;

    /**
     * Instantiates new enumerated info field value object.
     *
     * @return New info field value instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfoFieldValueEnum* NewEnumInfoFieldLC() = 0;

    /**
     * Instantiates new textual info field value object.
     *
     * @return New info field value instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfoFieldValueText* NewTextInfoFieldLC() = 0;

    /**
     * Instantiates new binary info field value object.
     *
     * @return New info field value instance. Object ownership
     *         is returned to caller.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual MPresenceInfoFieldValueBinary* NewBinaryInfoFieldLC() = 0;
    virtual MPresenceBlockInfo* NewPresenceBlockInfoLC() = 0;
    };



#endif // MPRESENCEOBJECTFACTORY_H



