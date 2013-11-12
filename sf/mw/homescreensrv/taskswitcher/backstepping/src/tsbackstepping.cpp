/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tsbackstepping.h"

#include <w32std.h>// key event
#include <apgwgnam.h>// for CApaWindowGroupName
#include <apgtask.h>
#include <coedef.h>
#include <e32property.h>
#include <e32debug.h>

#include <homescreendomainpskeys.h>
#include <afactivitylauncher.h>

#include "tsbacksteppingfilter.h"
#include "tsrunningapp.h"
#include "tsrunningappstorage.h"
#include "tsresourcemanager.h"
/**
 * String to switch hsapplication to IDLE state using activity framework
 */
_LIT(KHsActivactionUri, "appto://20022F35?activityname=HsIdleView&activityinbackground=true");

// -----------------------------------------------------------------------------
/** 
 *  Symbian two-phases constructor. Allocate create and initialize backstepping engine
 *  Instance is pushed int cleanup stack.
 *  @param aMonitor - window group changes monitor
 *  @return address to backstepping engine
 */
CTsBackstepping* CTsBackstepping::NewL( MTsWindowGroupsMonitor& aMonitor )
    {
    CTsBackstepping* self = CTsBackstepping::NewLC( aMonitor );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
/** 
 *  Symbian two-phases constructor. Allocate create and initialize backstepping engine
 *  @param aMonitor - window group changes monitor
 *  @return address to backstepping engine
 */
CTsBackstepping* CTsBackstepping::NewLC( MTsWindowGroupsMonitor& aMonitor )
    { 
    CTsBackstepping* self = new (ELeave) CTsBackstepping( aMonitor );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
/** 
 * Constructor.
 * @param aMonitor - window group changes monitor
 */
CTsBackstepping::CTsBackstepping( MTsWindowGroupsMonitor &aMonitor )
:
CTsWindowGroupsObserverBase( aMonitor ), iHsWasFirst(ETrue)
    {
    }

// -----------------------------------------------------------------------------
/** 
 *  CTsBackstepping::~CTsBackstepping
 *  deconstructor
 */
CTsBackstepping::~CTsBackstepping()
    {
    delete iFilter;
    }

// -----------------------------------------------------------------------------
/** 
 *  Symbian second-phase constructor. 
 *  Subscribe to window group monitor events (using BaseConstructL) and allocate
 *  filtering list
 */
void CTsBackstepping::ConstructL ()
    {
    BaseConstructL();
    iFilter = CTsBacksteppingFilter::NewL();
    }

// -----------------------------------------------------------------------------
/**
 * Analyzes window stack and move homescreen to proper position 
 * Interface implementation.
 * @see MTsWindowGroupsObserver::HandleWindowGroupChanged
 */
void CTsBackstepping::HandleWindowGroupChanged( 
                                MTsResourceManager &aResources, 
                                const MTsRunningApplicationStorage& aStorage )

    {
    RDebug::Print(_L("[Backstepping] started"));
    TRAP_IGNORE( HandleWindowGroupChangedL( aResources, aStorage ) );
    RDebug::Print(_L("[Backstepping] finished"));
    }

// -----------------------------------------------------------------------------
/** 
 * Analyzes window stack and move homescreen to proper position
 * @see MTsWindowGroupsObserver::HandleWindowGroupChanged from param info
 */
void CTsBackstepping::HandleWindowGroupChangedL( 
                                 MTsResourceManager &aResources, 
                                 const MTsRunningApplicationStorage& aStorage )
    {
    // calculate the desired position of Homescreen
    const TInt currentHsOffset( HomescreenOffsetL( aStorage ) );
    TInt optimalOffset(1);
    TInt targetHsOffset(currentHsOffset);
    TBool isFirst(ETrue);
    for( TInt offset(0); offset < currentHsOffset; ++offset ) 
        {
        RDebug::Print( _L("[Backstepping] application: %08x"), 
                       aStorage[offset].UidL().iUid );
        if( TUid::Null() != aStorage[offset].UidL() )
            {
            isFirst = EFalse;
            if( aStorage[offset].IsEmbeded())
                {
                targetHsOffset = aStorage.ParentIndex(aStorage[offset])+ 1;
                } 
            else if (!iFilter->isBlocked( aStorage[offset].UidL() ))
                {
                if(offset + 1 < currentHsOffset && 
                   aStorage[offset+1].UidL() == TUid::Null() ) 
                    {
                    ++optimalOffset;
                    }
                targetHsOffset = optimalOffset;
                }
            break;
            } 
        else
            {
            ++optimalOffset;
            }
        }
    SwitchToIdleStateL(aResources, isFirst);
    // change windows order if necessary
    if(targetHsOffset != currentHsOffset)
        {
        RDebug::Print(_L("[Backstepping] about to change window groups order"));
        aResources.WsSession().SetWindowGroupOrdinalPosition( 
                                    aStorage[currentHsOffset].WindowGroupId(), 
                                    targetHsOffset );
        }
    }

// -----------------------------------------------------------------------------
/** 
 * Find and return current position of hsapplication on window server stack
 * @param aStorage - storage with running app info
 * @return position of hsapplication on window server stack
 */
TInt CTsBackstepping::HomescreenOffsetL( 
                        const MTsRunningApplicationStorage& aStorage ) const 
    {
    TInt offset( KErrNotFound );
    for( TInt iter(0); KErrNotFound == offset && iter < aStorage.Count(); ++iter )
        {
        if( KHsCategoryUid == aStorage[iter].UidL() )
            {
            offset = iter;
            }
        }
    User::LeaveIfError( offset );
    return offset;
    }

// -----------------------------------------------------------------------------
/**
 * Change hsapplication state to IDLE if moves to background
 * @param aResources - OS resources manager
 * @param aIsFrst - flag to inform if hsapplication is in foreground
 */
void CTsBackstepping::SwitchToIdleStateL(MTsResourceManager& aResources, 
                                         TBool aIsFirst)
    {
    // switch Homescreen to Idle state if Homescreen is not in foreground and is in different state
    if( iHsWasFirst && !aIsFirst )
        {
        TInt hsState( EHomeScreenIdleState );
        User::LeaveIfError(RProperty::Get( KHsCategoryUid, 
                                           KHsCategoryStateKey, 
                                           hsState ) );
        if(!(hsState & EHomeScreenWidgetViewActive ) )
            {
            RDebug::Print(_L("[Backstepping] about to switch HS activity"));
            CAfActivityLauncher *activityEnabler = 
                CAfActivityLauncher::NewLC( aResources.ApaSession(), 
                                            aResources.WsSession() );
            activityEnabler->launchActivityL( KHsActivactionUri );
            CleanupStack::PopAndDestroy( activityEnabler );
            }
        }
    iHsWasFirst = aIsFirst;
    }

// end of file
