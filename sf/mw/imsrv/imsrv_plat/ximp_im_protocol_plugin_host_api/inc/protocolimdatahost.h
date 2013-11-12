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
* Description:  Interface for Im protocol connection host
 *
*/

#ifndef MPROTOCOLIMDATAHOST_H
#define MPROTOCOLIMDATAHOST_H

#include <ximpbase.h>
#include <improtocolpluginhostifids.hrh>
#include <e32std.h>

class MXIMPStatus;
class MImObjectFactory;
class MXIMPProtocolConnectionHostObserver;
class MProtocolImConversationDataHost;
class MProtocolImGroupDataHost;
class MProtocolImInvitationDataHost;
class MProtocolImSearchDataHost;
class MXIMPHost;
class CImDataCache ;

/**
 * Im connection host.
 *
 * @lib ximpprocessor.lib
 * @since S60 v3.2
 */
class MProtocolImDataHost //: public MXIMPBase

    {
public:

    
    /** Interface ID for the MProtocolImDataHost. */
    enum { KInterfaceId = XIMP_ECOM_IF_UID_IM_PROCESSOR_INTERFACE };

    
public:

    /**
     * Factory method to instantiate MImFeatures.
     *
     * Factory method to instantiate platform default
     * MXIMPClient implementation through the ECom.
     * If the default IM Feature isn't supported
     * in the platform, leaves with errorcode signalled
     * from ECom.
     *
     * @return The new Im client object. Object
     *         ownership is returned to caller.
     */
    //static MProtocolImDataHost* NewL( MXIMPHost& aHost );

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    //virtual ~MProtocolImDataHost();
    
//protected:

//    MProtocolImDataHost();

public: // Access to sub-interfaces


    /**
     * Gets reference to object factory interface.
     *
     * Im protocol connection implementation
     * uses factory interface to instantiate Im
     * objects.
     *
     * @return Object factory interface.
     *         No ownership is returned caller.
     */
    virtual MImObjectFactory& ImObjectFactory() = 0;



    /**
     * Gets reference to
     * MProtocolImConversationDataHost interface.
     *
     * Im protocol connection implementation uses returned
     * data host interface to manage the Im connections
     * Im conversation data.
     *
     * @return MProtocolImConversationDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    virtual MProtocolImConversationDataHost& ConversationDataHost() = 0;



    /**
     * Gets reference to
     * MProtocolImGroupDataHost interface.
     *
     * Im protocol connection implementation uses returned
     * data host interface to manage the Im connections
     * Im group data.
     *
     * @return MProtocolImGroupDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    //virtual MProtocolImGroupDataHost& GroupDataHost() = 0;



    /**
     * Gets reference to
     * MImProtocolInvitationDataHost interface.
     *
     * Im protocol connection implementation uses returned
     * data host interface to manage the im connections
     * im invitation data.
     *
     * @return MImProtocolInvitationDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    //virtual MProtocolImInvitationDataHost& InvitationDataHost() = 0;



    /**
     * Gets reference to
     * MProtocolImSearchDataHost interface.
     *
     * Im protocol connection implementation uses returned
     * data host interface to manage the Im connections
     * search data.
     *
     * @return MProtocolImSearchDataHost interface.
     *         Interface ownership is not returned caller.
     *         Returned interface is guaranteed to remain valid (usable)
     *         as long as this source interface.
     */
    //virtual MProtocolImSearchDataHost& SearchDataHost() = 0;
    
    virtual CImDataCache& ImDataCache() = 0;

    };


#endif // MPROTOCOLIMDATAHOST_H

