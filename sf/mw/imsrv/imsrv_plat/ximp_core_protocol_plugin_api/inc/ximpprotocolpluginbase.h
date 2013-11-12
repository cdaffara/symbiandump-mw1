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
* Description:  Base class for XIMP Framework protocol plug-ins.
*
*/

#ifndef CXIMPFWPROTOCOLPLUGINBASE_H
#define CXIMPFWPROTOCOLPLUGINBASE_H

#include <e32base.h>
#include <ecom/ecom.h>
#include <ximpprotocolplugin.h>
#include <ximpprotocolpluginhost.h>
#include <ximpprotocolpluginifids.hrh>



/**
 * Base class for XIMP Framework protocol plug-ins.
 *
 * Base class for XIMP Framework protocol plug-ins.
 * This class must be implemented by a XIMP Framework
 * protocol plug-ins so that XIMP Framework is able to
 * load a instance of it through ECom architecture and
 * can call into it. XIMP Framework manages loaded
 * CXIMPProtocolPluginBase object as thread singleton.
 *
 * Protocol plug-in must also implement MXIMPProtocolPlugin
 * interface, so XIMP Framework can call and issue
 * protocol and connection management requests to protocol
 * plug-in.
 *
 * @see MXIMPProtocolPlugin
 * @see MXIMPProtocolPluginHost
 * @ingroup ximpprotocolpluginapi
 * @since S60 v3.2
 */
class CXIMPProtocolPluginBase : public CBase,
                                public MXIMPProtocolPlugin
    {
public:

    /**
     * ECom interface UID for the CXIMPProtocolPluginBase.
     *
     * This ECom interface UID is used:
     * -# In protocol adaptation ECom registration resource file,
     *    to identify the implemented interface. Interface UID symbol
     *    @c XIMP_ECOM_IF_UID_PROTOCOL_PLUGIN_BASE_INTERFACE from
     *    ximpprotocolpluginifids.hrh can be directly included
     *    to protocol plug-in ECom registration resource file.
     *
     * -# Interface UID is used internally in XIMP Framework when
     *    resolving and instantiating available protocol implementations.
     */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_PROTOCOL_PLUGIN_BASE_INTERFACE };


public:


    /**
     * Public destructor.
     * Plugin can be deleted through this interface.
     */
    inline ~CXIMPProtocolPluginBase();



public:

    /**
     * Sets the plug-in destructor id.
     *
     * @param [in] aId
     *        Destructor id for the plug-in instance.
     */
    inline void SetDestructorId( TUid aId );



protected:

    /**
     * C++ default constructor.
     */
    inline CXIMPProtocolPluginBase();



private: //Data

    /**
     * OWN: ECom destructor ID for Presence Protocol plug-in.
     * XIMPFw assignes the destructor ID during plug-in
     * object construction.
     */
    TUid iEcomDtorID;


    /**
     * OWN: Reserved member for future extension.
     */
    TAny* iReserved1;


    /**
     * OWN: Reserved member for future extension.
     */
    TAny* iReserved2;


    /**
     * OWN: Reserved member for future extension.
     */
    TAny* iReserved3;
    };

#include "ximpprotocolpluginbase.inl"

#endif // CXIMPFWPROTOCOLPLUGINBASE_H
