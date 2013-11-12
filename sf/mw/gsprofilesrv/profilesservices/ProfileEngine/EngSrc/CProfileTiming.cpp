/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CProfileTiming.
*
*/



// INCLUDE FILES
#include "CProfileTiming.h"
#include "ProfileEnginePrivateCRKeys.h"
#include "ProfilesDebug.h"
#include <ProfilesVariant.hrh>
#include <f32file.h> // TParse
#include <data_caging_path_literals.hrh>
#include <MProfilesLocalFeatures.h>
#include <ProfileEngineConstants.h>

// LOCAL CONSTANTS AND MACROS
namespace
    {
    _LIT( KProfileScheduledTask, "z:PROFILESCHEDULEEVENT.exe" );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProfileTiming::CProfileTiming
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CProfileTiming::CProfileTiming( CRepository& aProfileRepository,
                                MProfilesLocalFeatures& aFeatures )
    : iRepository( aProfileRepository ), iFeatures( aFeatures )
    {
    }

// -----------------------------------------------------------------------------
// CProfileTiming::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProfileTiming::ConstructL()
    {
    if( iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        iEntryList = new ( ELeave ) CArrayFixFlat<TScheduleEntryInfo2>( 1 );

        iTaskData = KNullDesC().AllocL();

        TParse* tp = new ( ELeave ) TParse();
        tp->Set( KProfileScheduledTask, &KDC_SHARED_LIB_DIR, NULL );
        iTaskDllPath.Copy( tp->FullName() );
        delete tp;
        }
    }

// -----------------------------------------------------------------------------
// CProfileTiming::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProfileTiming* CProfileTiming::NewL( CRepository& aProfileRepository,
                                      MProfilesLocalFeatures& aFeatures )
    {
    CProfileTiming* self = 
        new( ELeave ) CProfileTiming( aProfileRepository, aFeatures );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }


// Destructor
CProfileTiming::~CProfileTiming()
    {
    if( iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        iScheduler.Close();

        if( iEntryList )
            {
            iEntryList->Reset();
            }
        delete iEntryList;
        delete iTaskData;
        }
    }


// -----------------------------------------------------------------------------
// CProfileTiming::SetTimedProfileL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTiming::SetTimedProfileL( TInt aPreviousId, TTime aTime )
    {
    PRODEBUG1( "CProfileTiming:SetTimedProfile( %d )", aPreviousId );
    if( !iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        return;
        }

    ConnectToSchedulerL();

    const TTimeIntervalMinutes KProfileMaxTiming( 1440 );
    const TInt KProfileOnce( 1 );
    const TInt KProfileDontCare( 0 );
    _LIT( KProfileTaskName, "TimedProfileTask" );
    _LIT( KProfileSchedulerItemRefName, "ProfileItemRef" );
    iEntryList->Reset();

    // iStartTime contains the time when the timed profile deactivates and the
    // validity period makes sure that the timing will go off in the nearest
    // 24 hours.
    TTsTime ttsTime;
    ttsTime.SetLocalTime( aTime );
    TScheduleEntryInfo2 entryInfo( ttsTime, EHourly, KProfileOnce, KProfileMaxTiming );
    iEntryList->AppendL( entryInfo );

    TName name( KProfileTaskName() );
    TTaskInfo taskInfo( KProfileDontCare, name, CActive::EPriorityStandard,
                        KProfileOnce ) ;

    TName name2( KProfileSchedulerItemRefName() );
    iSchedulerItemRef.iName = name2;

    PRODEBUG( "CProfileTiming:SetTimedProfile before CreatePers" );
    TInt error( iScheduler.CreatePersistentSchedule( iSchedulerItemRef,
                                                     *iEntryList ) );
    PRODEBUG1( "CProfileTiming:SetTimedProfile after CreatePers, err=%d )", error );
    if( !error )
        {
        error = iScheduler.ScheduleTask( taskInfo, *iTaskData,
                                         iSchedulerItemRef.iHandle );
        PRODEBUG1( "CProfileTiming:SetTimedProfile after ScheduleTask, err=%d )", error );
        if( error )
            {
            iScheduler.DeleteSchedule( iSchedulerItemRef.iHandle );
            }
        else
            {
            iTaskId = taskInfo.iTaskId;
            PRODEBUG( "CProfileTiming:SetTimedProfile before SaveSchedulingData" );
            SaveSchedulingData( aPreviousId, iSchedulerItemRef.iHandle,
                                iTaskId );
            PRODEBUG( "CProfileTiming:SetTimedProfile after SaveSchedulingData" );
            }
        }

    PRODEBUG1( "CProfileTiming:SetTimedProfile before return:%d", error );
    User::LeaveIfError( error );
    }

// -----------------------------------------------------------------------------
// CProfileTiming::RollbackTimedProfile
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTiming::RollbackTimedProfile()
    {
    PRODEBUG( "CProfileTiming:RollbackTimedProfile" );
    if( !iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        return;
        }

    // Ignore errors here, there's already an error occurred because  this
    // method is called
    iScheduler.DeleteTask( iTaskId );
    iScheduler.DeleteSchedule( iSchedulerItemRef.iHandle );
    PRODEBUG( "CProfileTiming:RollbackTimedProfile end" );
    }

// -----------------------------------------------------------------------------
// CProfileTiming::CancelTimedProfile
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CProfileTiming::CancelTimedProfileL()
    {
    PRODEBUG( "CProfileTiming:CancelTimedProfileL" );
    if( !iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        return KErrNone;
        }

    TInt previousId( KErrNotFound );
    TInt taskId( KErrNotFound );
    GetSchedulingDataL( previousId, iSchedulerItemRef.iHandle, taskId );
    PRODEBUG1( "CProfileTiming:CancelTimedProfileL previousid=%d", previousId );
    if( previousId != KErrNotFound )
        {
        ConnectToSchedulerL();
        TInt error( iScheduler.DeleteTask( taskId ) );
        PRODEBUG1( "CProfileTiming:CancelTimedProfileL after deletetask err=%d", error );
        if( ( error != KErrNone ) && ( error != KErrNotFound ) )
            {
            User::Leave( error );
            }

        error = iScheduler.DeleteSchedule( iSchedulerItemRef.iHandle );
        PRODEBUG1( "CProfileTiming:CancelTimedProfileL after deletesched err=%d", error );
        if( ( error != KErrNone ) && ( error != KErrNotFound ) )
            {
            User::Leave( error );
            }
                            
        SaveSchedulingData(); // resets timing data in cenrep
        PRODEBUG( "CProfileTiming:CancelTimedProfileL after SaveSchedulingData" );
        }
    PRODEBUG( "CProfileTiming:CancelTimedProfileL end" );
    return previousId;
    }

// -----------------------------------------------------------------------------
// CProfileTiming::IsTimingActiveL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CProfileTiming::IsTimingActiveL()
    {
    PRODEBUG( "CProfileTiming:IsTimingActiveL" );
    TBool result( EFalse );

    if( iFeatures.IsFeatureSupported( KProEngFeatureIdTimedProfiles ) )
        {
        User::LeaveIfError( iRepository.Get( KProEngSchedulerHandleId,
                            iSchedulerItemRef.iHandle ) );
        result = ( iSchedulerItemRef.iHandle != KErrNotFound );
        }

    PRODEBUG1( "CProfileTiming:IsTimingActiveL returning:%d", result );
    return result;
    }

// -----------------------------------------------------------------------------
// CProfileTiming::SaveSchedulingData
// The errors are ignored inside this method, because this class is used only
// inside CentralRepository transactions.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTiming::SaveSchedulingData( TInt aPreviousId, TInt aHandleId,
                                         TInt aTaskId )
    {
    iRepository.Set( KProEngSchedulerHandleId, aHandleId );
    iRepository.Set( KProEngPreviousActiveId, aPreviousId );
    iRepository.Set( KProEngSchedulerTaskId, aTaskId );
    }

// -----------------------------------------------------------------------------
// CProfileTiming::GetSchedulingData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTiming::GetSchedulingDataL( TInt& aPreviousId, TInt& aHandleId,
                                         TInt& aTaskId )
    {
    PRODEBUG( "CProfileTiming:GetSchedulingDataL" );
    User::LeaveIfError(
            iRepository.Get( KProEngPreviousActiveId, aPreviousId ) );
    User::LeaveIfError(
            iRepository.Get( KProEngSchedulerHandleId, aHandleId ) );
    User::LeaveIfError( iRepository.Get( KProEngSchedulerTaskId, aTaskId ) );
    PRODEBUG( "CProfileTiming:GetSchedulingDataL end" );
    }

// -----------------------------------------------------------------------------
// CProfileTiming::ConnectToSchedulerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CProfileTiming::ConnectToSchedulerL()
    {
    // Feature support is not tested here because it must have been done already
    // in the calling method.
    if( !iConnected )
        {
        User::LeaveIfError( iScheduler.Connect() );

        User::LeaveIfError( iScheduler.Register( iTaskDllPath,
                                                 CActive::EPriorityStandard ) );
        iConnected = ETrue;
        }
    }

//  End of File

