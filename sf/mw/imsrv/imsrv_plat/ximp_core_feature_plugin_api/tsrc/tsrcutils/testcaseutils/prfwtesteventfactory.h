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
* Description:
*
*/


#ifndef PRFWTESTEVENTFACTORY_H__
#define PRFWTESTEVENTFACTORY_H__

#include <e32base.h>
#include <ximpdatasubscriptionstate.h>

// CLASS DESCRIPTION

class CPresentityGroupListEventImp;
class CPresentityGroupContentEventImp;
class CXIMPDataSubscriptionStateImp;

/**
 * Presence event factory for use in tests.
 */
class XIMPTestEventFactory
    {
    public:

        // @see below  
        enum TTestPGLArraySpecifier 
            { 
            ETestPGLCreated = 0,    // group info is created
            ETestPGLUpdated,        // group info is updated
            ETestPGLDeleted,        // group info is deleted
            ETestPGLEmpty,          // make an empty array for empty event
            };

        /**
         * Places an identity (uri-displayname pair) inside a group info into the
         * array defined by aArraySpec, and returns the created event.
         *
         * In cleanupstack there are 5 items, FIFO:
         *   - created list
         *   - updated list
         *   - deleted list
         *   - current list
         *   - event imp
         *
         * This is because this event won't take ownership to the arrays
         * when created this way.
         *
         * @param aUri URI of group
         * @param aDispName Displayname
         * @param aArraySpec Defines into which array to put the group info
         * @param aUriCurrent The URI of group in "current"-list
         * @param aDispNameCurrent The display name of the group in
         * "current"-list
         * @param aStatus The subscription status 
         * @return The suitably filled event.
         */
        IMPORT_C static CPresentityGroupListEventImp* CreateGroupListEventLCX( 
                const TDesC& aUri,
                const TDesC& aDispName,

                TTestPGLArraySpecifier aArraySpec,

                const TDesC& aUriCurrent,
                const TDesC& aDispNameCurrent,
                
                MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
                MXIMPDataSubscriptionState::TDataState aDataState
                );

        // @see below  
        enum TTestPGLContentArraySpecifier 
            { 
            ETestPGLCntAdded = 0,    // group content is added
            ETestPGLCntUpdated,      // group content is updated
            ETestPGLCntRemoved,      // group content is deleted
            ETestPGLCntEmpty,        // make an empty array for empty event
            };

        /**
         * Places an group member (identity-displayname pair) inside 
         * a group content info into the delta array defined by aArraySpec, 
         * and returns the created event.
         *
         * The current member list will contain only the group member
         * specified by parameters aIdentityCurrent and aDispNameCurrent.
         * To support more members than 1 this method needs to be changed,
         * but for now, it suffices for our testing.
         *
         * In cleanupstack there are 5 items, FIFO:
         *   - added list
         *   - updated list
         *   - removed list
         *   - current member list
         *   - event imp
         *
         * This is because this event won't take ownership to the arrays
         * when created this way.
         *
         * @param aGroupId The group id
         * @param aUri Uri
         * @param aDispName Displayname
         * @param aArraySpec Defines into which delta array to put the member
         * @param aIdentityCurrent Identity part of current member
         * @param aDispNameCurrent Display name part of current member
         * @param aStatus The subscription status
         * @return The suitably filled event.
         */
        IMPORT_C static CPresentityGroupContentEventImp* 
            CreateGroupContentEventLCX( 
                    const TDesC& aGroupId,
                    
                    const TDesC& aIdentityForDelta,
                    const TDesC& aDispNameForDelta,
                    
                    TTestPGLContentArraySpecifier aArraySpec,
         
                    const TDesC& aIdentityCurrent,
                    const TDesC& aDispNameCurrent,
                    
                    MXIMPDataSubscriptionState::TSubscriptionState aSubscriptionState,
                    MXIMPDataSubscriptionState::TDataState aDataState
                    );
    };

#endif //PRFWTESTEVENTFACTORY_H__


