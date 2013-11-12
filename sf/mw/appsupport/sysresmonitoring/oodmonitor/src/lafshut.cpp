/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


// SYSTEM INCLUDES
#include <uiklaf/private/lafshut.h>
#include <w32std.h>
#include <coemain.h>
#include <lafmemorywatcher.rsg>
#include <bautils.h>
#include <e32property.h>
#include <UikonInternalPSKeys.h>
#include <data_caging_path_literals.hrh>
#include <coreapplicationuisdomainpskeys.h>

// USER INCLUDES
#include "UiklafInternalCRKeys.h"
#include "OodTraces.h"
#include "Ood.h"

// CONSTANTS

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C CLafShutdownManagerBase* LafShutdown::CreateShutdownManager(
                MShutdownEventObserver& aObserver)
    { // static
    TRACES("LafShutdown::CreateShutdownManager");
    CLafShutdownManagerBase* lafShutdownManager(NULL);
    TInt err(KErrNone);
    TRAP(err, lafShutdownManager = CLafShutdownManager::NewL(aObserver));
    return lafShutdownManager;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C MSaveObserver::TSaveType LafShutdown::SaveFromViewSwitch()
    { // static
    TRACES("LafShutdown::SaveFromViewSwitch");
    return MSaveObserver::ESaveNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
EXPORT_C MSaveObserver::TSaveType LafShutdown::SaveFromAppSwitch()
    { // static
    TRACES("LafShutdown::SaveFromAppSwitch");
    return MSaveObserver::ESaveNone;
    }


// ======================================================================
// class CLafShutdownManager
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager* CLafShutdownManager::NewL(MShutdownEventObserver& aObserver)
    { // static
    TRACES("LafShutdown::NewL");
    CLafShutdownManager* self = new(ELeave) CLafShutdownManager(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::~CLafShutdownManager()
    {
    TRACES("LafShutdown::~CLafShutdownManager");
    delete iOODWarningWatcher;
    delete iOODCriticalWatcher;

    iDiskStatusProperty.Close();
    iMMCStatusProperty.Close();

    if (iDisplayNotesSubscriber)
        {
        iDisplayNotesSubscriber->StopSubscribe();
        }
    iCanDisplayNotesProperty.Close();
    delete iDisplayNotesSubscriber;

    if (iAutolockStatusSubscriber)
        {
        iAutolockStatusSubscriber->StopSubscribe();
        }
    iAutolockStatusProperty.Close();
    delete iAutolockStatusSubscriber;

    if (iMMCWatchSubscriber)
        {
        iMMCWatchSubscriber->StopSubscribe();
        }
    iEnableMMCWatchProperty.Close();
    delete iMMCWatchSubscriber;

    delete iOOMMCWarningWatcher;
    delete iOOMMCCriticalWatcher;

    delete iGlobalQueryActive;

    iWs.Close();
    iFs.Close();
    TRACES("LafShutdown::~CLafShutdownManager: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CLafShutdownManager(MShutdownEventObserver& aObserver)
    : CLafShutdownManagerBase(aObserver)
    {
    TRACES("CLafShutdownManager::CLafShutdownManager");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::ConstructL()
    {
    TRACES("CLafShutdownManager::ConstructL");
    User::LeaveIfError(iWs.Connect());

    User::LeaveIfError(iFs.Connect());

    RResourceFile resFile;
    TFileName filename(KDriveZ);
    filename.Append(KDC_RESOURCE_FILES_DIR);
    filename.Append(KOOMWatcherResourceFileName);
    BaflUtils::NearestLanguageFile(iFs, filename);
    resFile.OpenL(iFs, filename);
    CleanupClosePushL(resFile);

    TResourceReader theReader;

        HBufC8* thresholds = resFile.AllocReadLC(R_APP_OOD_THRESHOLDS);

        theReader.SetBuffer(thresholds);
        iOODWarningThreshold = theReader.ReadInt32();
        iOODCriticalThreshold = theReader.ReadInt32();
        CleanupStack::PopAndDestroy(thresholds);
        // Global Query
        iGlobalQueryActive = CGlobalQueryActive::NewL(resFile,theReader);

    CleanupStack::PopAndDestroy(); // resFile.Close()

        TVolumeInfo volInfo;

        User::LeaveIfError(iDiskStatusProperty.Attach(KPSUidUikon, KUikFFSFreeLevel));
        User::LeaveIfError(iMMCStatusProperty.Attach (KPSUidUikon, KUikMmcFFSFreeLevel));

        iDiskStatusProperty.Set(GetDiskStatusL(volInfo, EDriveC));
        iMMCStatusProperty.Set(DISK_SPACE_OK);

        User::LeaveIfError(iCanDisplayNotesProperty.Attach (KPSUidUikon, KUikGlobalNotesAllowed));
        iDisplayNotesSubscriber = new (ELeave) CSubscriber(TCallBack(DisplayNotesCallBackL, this),
                                                           iCanDisplayNotesProperty);
        iDisplayNotesSubscriber->SubscribeL();


        User::LeaveIfError(iAutolockStatusProperty.Attach (KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus));
        iAutolockStatusSubscriber = new (ELeave) CSubscriber(TCallBack(AutolockStatusCallBackL, this),
                                                           iAutolockStatusProperty);
        iAutolockStatusProperty.Get(iAutolockStatus);
        
        iAutolockStatusSubscriber->SubscribeL();

        // Start OOD watchers.
        iOODWarningWatcher = CLafShutdownManager::COutOfDiskWatcher::NewL(*this,
                                                                          iOODWarningThreshold,
                                                                          EDriveC,
                                                                          iFs);
        iOODWarningWatcher->Start();

        iOODCriticalWatcher = CLafShutdownManager::COutOfDiskWatcher::NewL(*this,
                                                                           iOODCriticalThreshold,
                                                                           EDriveC,
                                                                           iFs);
        iOODCriticalWatcher->Start();

        User::LeaveIfError(iEnableMMCWatchProperty.Attach (KPSUidUikon, KUikMMCInserted));
        iMMCWatchSubscriber = new (ELeave) CSubscriber(TCallBack(MMCStatusChanged, this),
                                                       iEnableMMCWatchProperty);
        iMMCWatchSubscriber->SubscribeL();
    TRACES("CLafShutdownManager::ConstructL: End");        
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt CLafShutdownManager::MMCStatusChanged(TAny* aPtr)
    {
    TRACES("CLafShutdownManager::MMCStatusChanged");
    CLafShutdownManager* self = STATIC_CAST(CLafShutdownManager*,aPtr);
    if (self)
        {
        TInt err(KErrNone);
        TRAP(err, self->HandleMMCCallBackL());
        }
    TRACES("CLafShutdownManager::MMCStatusChanged: End");        
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::HandleMMCCallBackL()
    {
    TRACES("CLafShutdownManager::HandleMMCCallBackL");        
    TInt value = 0;
    iEnableMMCWatchProperty.Get(value);
    if (value)
        {
        if (!iOOMMCWarningWatcher)
            {
            iOOMMCWarningWatcher = COutOfDiskWatcher::NewL(*this,iOODWarningThreshold,EDriveE,iFs);
            }
        if (!iOOMMCCriticalWatcher)
            {
            iOOMMCCriticalWatcher = COutOfDiskWatcher::NewL(*this,
                                                            iOODCriticalThreshold,
                                                            EDriveE,
                                                            iFs);
            }

        iOOMMCWarningWatcher->Start();
        iOOMMCCriticalWatcher->Start();

#ifdef _DEBUG
    RDebug::Print(_L("OOD WATCHER: Started MMC watch"));
#endif

        }
    else
        {
        iMMCStatusProperty.Set( DISK_SPACE_OK );
        if (iOOMMCWarningWatcher)
            {
            iOOMMCWarningWatcher->Cancel();
            }
        if (iOOMMCCriticalWatcher)
            {
            iOOMMCCriticalWatcher->Cancel();
            }
#ifdef _DEBUG
    RDebug::Print(_L("OOD WATCHER: Cancelled MMC watch"));
#endif
        }
    TRACES("CLafShutdownManager::HandleMMCCallBackL: End");        
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt CLafShutdownManager::DisplayNotesCallBackL(TAny* aPtr)
    {
    TRACES("CLafShutdownManager::DisplayNotesCallBackL");
    CLafShutdownManager* self = STATIC_CAST(CLafShutdownManager*,aPtr);
    if (self)
        self->HandleDisplayNotesCallBackL();
    TRACES("CLafShutdownManager::DisplayNotesCallBackL: End");
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::HandleDisplayNotesCallBackL()
    {
    TRACES("CLafShutdownManager::HandleDisplayNotesCallBackL");
    TInt value(0);
    iCanDisplayNotesProperty.Get(value);
    if (value == 1)
        {
        TRACES("CLafShutdownManager::HandleAutolockStatusCallBack: Can display notes");
        FreeDiskThresholdCrossedL(EDriveC, ETrue);
        iEnableMMCWatchProperty.Get(value);
        if (value)
            {
            TRACES("CLafShutdownManager::HandleAutolockStatusCallBack: Memory card inserted");
            FreeDiskThresholdCrossedL(EDriveE, ETrue);
            }
        }
    TRACES("CLafShutdownManager::HandleDisplayNotesCallBackL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt CLafShutdownManager::AutolockStatusCallBackL(TAny* aPtr)
    {
    TRACES("CLafShutdownManager::AutolockStatusCallBackL");
    CLafShutdownManager* self = STATIC_CAST(CLafShutdownManager*,aPtr);
    if (self)
        self->HandleAutolockStatusCallBackL();
    TRACES("CLafShutdownManager::DisplayNotesCallBackL: End");
    return KErrNone;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::HandleAutolockStatusCallBackL()
    {        
    TRACES("CLafShutdownManager::HandleAutolockStatusCallBackL");
    
    TInt autolockStatus(0);
    iAutolockStatusProperty.Get(autolockStatus);

    if (autolockStatus == EAutolockOff && 
        iAutolockStatus != EAutolockStatusUninitialized && 
        iAutolockStatus != EAutolockOff )
        {
        TRACES("CLafShutdownManager::HandleAutolockStatusCallBack: Autolock off");
        TInt value(0);
        iCanDisplayNotesProperty.Get(value);
        if (value == 1)
            {
            TRACES("CLafShutdownManager::HandleAutolockStatusCallBack: Can display notes");
            FreeDiskThresholdCrossedL(EDriveC, ETrue, ETrue);
            iEnableMMCWatchProperty.Get(value);
            if (value)
                {
                TRACES("CLafShutdownManager::HandleAutolockStatusCallBack: Memory card inserted");
                FreeDiskThresholdCrossedL(EDriveE, ETrue, ETrue);
                }
            }            
        }
    iAutolockStatus = autolockStatus;
    TRACES("CLafShutdownManager::HandleAutolockStatusCallBackL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::ReportEventL(MSaveObserver::TSaveType aAction,TBool aPowerOff)
    {
    TRACES("CLafShutdownManager::ReportEventL");
    iObserver.HandleShutdownEventL(aAction,aPowerOff);
    TRACES("CLafShutdownManager::ReportEventL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::GetShutdownState(TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest)
    {
    TRACES("CLafShutdownManager::GetShutdownState");
    iObserver.GetShutdownState(aPowerOff, aAllSessionsHavePendingRequest);
    TRACES("CLafShutdownManager::GetShutdownState: End");
    }
// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::FreeDiskThresholdCrossedL(TDriveNumber aDrive, TBool aIsCallBack, TBool aForcedNote)
    {
    TRACES("CLafShutdownManager::FreeDiskThresholdCrossedL");
    TVolumeInfo volInfo;
    TInt oldDiskStatus;
    TInt oldMmcStatus;

    iDiskStatusProperty.Get(oldDiskStatus);
    iMMCStatusProperty.Get(oldMmcStatus);
    TInt status = GetDiskStatusL(volInfo, aDrive);
    if (aDrive == EDriveC)
        {
        iDiskStatusProperty.Set(status);
        }
    else
        {
        iMMCStatusProperty.Set(status);
        }

#ifdef _DEBUG
    RDebug::Print(_L("OOD WATCHER: Notified on drive %c, Free: 0x%X%X"),aDrive+KAlphaStartPoint,I64HIGH(volInfo.iFree),I64LOW(volInfo.iFree));
#endif

    TInt diskStatus = DISK_SPACE_OK;
    TInt mmcStatus  = DISK_SPACE_OK;

    iDiskStatusProperty.Get(diskStatus);
    iMMCStatusProperty.Get (mmcStatus);
    switch (((aDrive == EDriveC) ? diskStatus : mmcStatus))
        {
    case DISK_SPACE_CRITICAL:
        {
        if (aDrive == EDriveC)
            {
            if (aIsCallBack || oldDiskStatus != DISK_SPACE_CRITICAL)
                {
                ShowGlobalQueryL(CGlobalQueryActive::ECritical, aForcedNote);
                }
            }
        else
            {
            if (aIsCallBack || oldMmcStatus != DISK_SPACE_CRITICAL)
                {
                ShowGlobalQueryL(CGlobalQueryActive::ECriticalMMC, aForcedNote);
                }
            }
#ifdef _DEBUG
    TBuf<KMaxMsgSize> msg;
    msg.AppendFormat( KCriticalMode, aDrive+KAlphaStartPoint );
    RDebug::Print(_L("OOD WATCHER: Entering critical mode on drive %c"),aDrive+KAlphaStartPoint);
    User::InfoPrint(msg);
#endif
        break;
        }
    case DISK_SPACE_WARNING:
        {
        // this is bit awkward but necessary:
        ShowGlobalQueryL( aDrive!=EDriveC ?
            CGlobalQueryActive::EClearNotesMMC :
            CGlobalQueryActive::EClearNotesC, aForcedNote );
        // Display note only if disk is getting more full.
        if (oldDiskStatus != DISK_SPACE_CRITICAL||aDrive != EDriveC)
            {
            if (aDrive == EDriveC)
                {
                if (aIsCallBack || oldDiskStatus != DISK_SPACE_WARNING)
                    {
                    ShowGlobalQueryL(CGlobalQueryActive::EWarning, aForcedNote);
                    }
                }
            else if (oldMmcStatus != DISK_SPACE_CRITICAL)
                {
                if (aIsCallBack || oldMmcStatus != DISK_SPACE_WARNING)
                    {
                    ShowGlobalQueryL(CGlobalQueryActive::EWarningMMC, aForcedNote);
                    }
                }
            }
#ifdef _DEBUG
    TBuf<KMaxMsgSize> msg;
    msg.AppendFormat( KWarningMode, aDrive+KAlphaStartPoint );
    RDebug::Print(_L("OOD WATCHER: Entering warning mode on drive %c"),aDrive+KAlphaStartPoint);
    User::InfoPrint(msg);
#endif
        break;
        }
    case DISK_SPACE_OK:
    default:
        {
#ifdef _DEBUG
        TBuf<KMaxMsgSize> msg;
        msg.AppendFormat( KNormalMode, aDrive+KAlphaStartPoint );
        RDebug::Print(_L("OOD WATCHER: Entering normal   mode on drive %c"),aDrive+KAlphaStartPoint);
        User::InfoPrint(msg);
#endif

        if (aDrive == EDriveC)
            {
            ShowGlobalQueryL(CGlobalQueryActive::EClearNotesC, aForcedNote);
            }
        else
            {
            ShowGlobalQueryL(CGlobalQueryActive::EClearNotesMMC, aForcedNote);
            }
        break;
        }
        
        } // end of switch
    TRACES("CLafShutdownManager::FreeDiskThresholdCrossedL: End");
    }

// ---------------------------------------------------------
// Use a global query to display the message but make sure that eiksrv is
// there (state must not be initialising (AVKON + UIKON)
// ---------------------------------------------------------
//
void CLafShutdownManager::ShowGlobalQueryL(const CGlobalQueryActive::TMessageType& aMessageType, TBool aForcedNote)
    {
    TRACES("CLafShutdownManager::ShowGlobalQueryL");
    TInt value = 0;
    iCanDisplayNotesProperty.Get(value);
    if (value == 1)
        {
        iGlobalQueryActive->DisplayL(aMessageType, aForcedNote);
        }
    TRACES("CLafShutdownManager::ShowGlobalQueryL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
TInt CLafShutdownManager::GetDiskStatusL(TVolumeInfo& aVolInfo, TDriveNumber aDrive)
    {
    TRACES("CLafShutdownManager::GetDiskStatusL");
    // MMC not always present: By returning disk space ok, we won't trigger any alarms.
    TDriveInfo info;
    if ( aDrive != EDriveC &&
        (iFs.Drive(info,aDrive) != KErrNone ||
         iFs.Volume(aVolInfo,aDrive) != KErrNone) )
        {
        TRACES("CLafShutdownManager::GetDiskStatusL: End(1): return DISK_SPACE_OK");
        return DISK_SPACE_OK;
        }

    if (aDrive == EDriveC) // For other drives we already have valid data.
        {
        User::LeaveIfError(iFs.Volume(aVolInfo,aDrive));
        }

    if (I64HIGH(aVolInfo.iFree) != 0)
        { // If the high bytes are non zero, by definition we are OK (thresholds are 32bit).
        TRACES("CLafShutdownManager::GetDiskStatusL: End(2): return DISK_SPACE_OK");
        return DISK_SPACE_OK;
        }
    TUint freeSpace32 = I64LOW(aVolInfo.iFree);
    if (freeSpace32 < iOODCriticalThreshold)
        {
        TRACES("CLafShutdownManager::GetDiskStatusL: End: return DISK_SPACE_CRITICAL");
        return DISK_SPACE_CRITICAL;
        }
    else if (freeSpace32 < iOODWarningThreshold)
        {
        TRACES("CLafShutdownManager::GetDiskStatusL: End: return DISK_SPACE_WARNING");
        return DISK_SPACE_WARNING;
        }
    else
        {
        TRACES("CLafShutdownManager::GetDiskStatusL: End(3): return DISK_SPACE_OK");
        return DISK_SPACE_OK;
        }
    }


// ======================================================================
// class CLafShutdownManager::COutOfDiskWatcher
// - notifies when free disk crosses preset threshold
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::COutOfDiskWatcher* CLafShutdownManager::COutOfDiskWatcher::NewL(
                                                             CLafShutdownManager& aLafShutdown,
                                                             TInt64 aThreshold,
                                                             TDriveNumber aDrive,
                                                             RFs& aFs)
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::NewL");        
    COutOfDiskWatcher* self = new (ELeave) COutOfDiskWatcher(aLafShutdown,aThreshold,aDrive,aFs);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    TRACES("CLafShutdownManager::COutOfDiskWatcher::NewL: End");
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::COutOfDiskWatcher::~COutOfDiskWatcher()
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::~COutOfDiskWatcher");
    Cancel();
    TRACES("CLafShutdownManager::COutOfDiskWatcher::~COutOfDiskWatcher: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::COutOfDiskWatcher::COutOfDiskWatcher(CLafShutdownManager& aLafShutdown,
                                                          TInt64 aThreshold,
                                                          TDriveNumber aDrive,
                                                          RFs& aFs)
:   CActive(CActive::EPriorityStandard),
    iFs(aFs),
    iLafShutdown(aLafShutdown),
    iThreshold(aThreshold),
    iDrive(aDrive)
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::COutOfDiskWatcher");
    CActiveScheduler::Add(this);
    TRACES("CLafShutdownManager::COutOfDiskWatcher::COutOfDiskWatcher: End");
    }

/**
 * Monitor startup
 * states vis SD so that as soon as we've completed started up we display
 * a note if needed
 */

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::COutOfDiskWatcher::ConstructL()
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::ConstructL");
    iLafShutdown.FreeDiskThresholdCrossedL(iDrive);
    TRACES("CLafShutdownManager::COutOfDiskWatcher::ConstructL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::COutOfDiskWatcher::Start()
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::Start");
    if (!IsActive())
        {
        if ( iDrive == EDriveE )
            {
            TInt err(KErrNone);
            TRAP(err, iLafShutdown.FreeDiskThresholdCrossedL(iDrive));
            }
        iFs.NotifyDiskSpace(iThreshold,iDrive,iStatus);
        SetActive();
        }
    TRACES("CLafShutdownManager::COutOfDiskWatcher::Start: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::COutOfDiskWatcher::DoCancel()
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::DoCancel");
    iFs.NotifyDiskSpaceCancel(iStatus);
    TRACES("CLafShutdownManager::COutOfDiskWatcher::DoCancel: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::COutOfDiskWatcher::RunL()
    {
    TRACES("CLafShutdownManager::COutOfDiskWatcher::RunL");
    TInt status = iStatus.Int();
    if (status < 0)
        {
        User::After(KBreatheTime); // just breathe 5 seconds 
#ifdef _DEBUG
        RDebug::Print(_L("#####NotifyDiskSpace returned error %D"),status);
#endif        
        }

    Start();
    if (status >= 0)
        {
#ifdef _DEBUG
    	TRAPD(err, iLafShutdown.FreeDiskThresholdCrossedL(iDrive));        
        RDebug::Print(_L("#####FreeDiskThresholdCrossedL returned error %D"),err);
#else
        TRAP_IGNORE(iLafShutdown.FreeDiskThresholdCrossedL(iDrive));
#endif          
        }
    TRACES("CLafShutdownManager::COutOfDiskWatcher::RunL: End");
    }

// ======================================================================
// class CLafShutdownManager::CGlobalQueryActive
// ======================================================================

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CGlobalQueryActive* CLafShutdownManager::CGlobalQueryActive::NewL
    (RResourceFile& aResFile, TResourceReader& aResReader)
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::NewL");
    CGlobalQueryActive* self = new (ELeave) CGlobalQueryActive;
    CleanupStack::PushL(self);
    self->ConstructL(aResFile,aResReader);
    CleanupStack::Pop(self);
    TRACES("CLafShutdownManager::CGlobalQueryActive::NewL: End");
    return self;
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CGlobalQueryActive::~CGlobalQueryActive()
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::~CGlobalQueryActive");        
    delete iMessageInfo[ECritical];
    delete iMessageInfo[EWarning];
    delete iMessageInfo[EWarningMMC];
    delete iMessageInfo[ECriticalMMC];
    TRACES("CLafShutdownManager::CGlobalQueryActive::~CGlobalQueryActive: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CGlobalQueryActive::CGlobalQueryActive()
    :iMessageType(ENone)
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::CGlobalQueryActive");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CGlobalQueryActive::ConstructL(RResourceFile& aResFile,
                                                         TResourceReader& aResReader)
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::ConstructL");
    HBufC8* message = aResFile.AllocReadLC(R_QTN_MEMLO_MEMORY_FULL);
    aResReader.SetBuffer(message);

    iMessageInfo[ECritical] = new (ELeave) CMessageInfo(aResReader.ReadHBufCL());

    CleanupStack::PopAndDestroy(message);
    message = NULL;

    message = aResFile.AllocReadLC(R_QTN_MEMLO_MEMORY_RUNNING_OUT);
    aResReader.SetBuffer(message);

    iMessageInfo[EWarning] = new (ELeave) CMessageInfo(aResReader.ReadHBufCL());

    CleanupStack::PopAndDestroy(message);

    // MMC strings.
    message = aResFile.AllocReadLC(R_QTN_MEMLO_MMC_MEMORY_RUNNING_OUT);
    aResReader.SetBuffer(message);

    iMessageInfo[EWarningMMC] = new (ELeave) CMessageInfo(aResReader.ReadHBufCL());

    CleanupStack::PopAndDestroy(message);

    message = aResFile.AllocReadLC(R_QTN_MEMLO_MMC_MEMORY_FULL);
    aResReader.SetBuffer(message);

    iMessageInfo[ECriticalMMC] = new (ELeave) CMessageInfo(aResReader.ReadHBufCL());

    CleanupStack::PopAndDestroy(message);
    TRACES("CLafShutdownManager::CGlobalQueryActive::ConstructL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CGlobalQueryActive::DisplayL(TMessageType aType, TBool aForcedNote)
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::DisplayL");
    if (aType == EClearNotesC || aType == EClearNotesMMC)
        {
        TInt index = (aType == EClearNotesC)?EWarning:ECriticalMMC;
        for (TInt ii = 0; ii < 2; ii++)
            {
            iMessageInfo[index-ii]->iNoteId = KErrNotFound;
            }
        return;
        }

      if (aType != ECallBack)
        {
        iMessageType = aType;
        }

    CleanupL();

          
        CHbDeviceMessageBoxSymbian* globalNote = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EWarning);
        CleanupStack::PushL(globalNote);
        globalNote->SetTextL((iMessageInfo[iMessageType]->iMessage)->Des());
        globalNote->SetTimeoutL(0);
        globalNote->ExecL();
        CleanupStack::PopAndDestroy(globalNote);
        
    iMessageType = ENone;
    TRACES("CLafShutdownManager::CGlobalQueryActive::DisplayL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CGlobalQueryActive::CleanupL(TBool aCancel)
    {
    TRACES("CLafShutdownManager::CGlobalQueryActive::CleanupL");
    if (iMessageType != ENone)
        {
        TInt cancelNoteId = KErrNotFound;
        switch (iMessageType)
            {
        case ECritical:
            {
            cancelNoteId = iMessageInfo[EWarning]->iNoteId;
            iMessageInfo[EWarning]->iNoteId = KErrNotFound;
            break;
            }
        case ECriticalMMC:
            {
            cancelNoteId = iMessageInfo[EWarningMMC]->iNoteId;
            iMessageInfo[EWarningMMC]->iNoteId = KErrNotFound;
            break;
            }
        default:
            break;
            }

         }
    TRACES("CLafShutdownManager::CGlobalQueryActive::CleanupL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CSubscriber::CSubscriber(TCallBack aCallBack, RProperty& aProperty)
    : CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    TRACES("CLafShutdownManager::CSubscriber::CSubscriber");
    CActiveScheduler::Add(this);
    TRACES("CLafShutdownManager::CSubscriber::CSubscriber: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
CLafShutdownManager::CSubscriber::~CSubscriber()
    {
    TRACES("CLafShutdownManager::CSubscriber::~CSubscriber");
    Cancel();
    TRACES("CLafShutdownManager::CSubscriber::~CSubscriber: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CSubscriber::SubscribeL()
    {
    TRACES("CLafShutdownManager::CSubscriber::SubscribeL");
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    TRACES("CLafShutdownManager::CSubscriber::SubscribeL: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CSubscriber::StopSubscribe()
    {
    TRACES("CLafShutdownManager::CSubscriber::StopSubscribe");
    Cancel();
    TRACES("CLafShutdownManager::CSubscriber::StopSubscribe: End");
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CSubscriber::RunL()
    {
    TRACES("CLafShutdownManager::CSubscriber::RunL");        
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    TRACES("CLafShutdownManager::CSubscriber::RunL: End");        
    }

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
void CLafShutdownManager::CSubscriber::DoCancel()
    {
    TRACES("CLafShutdownManager::CSubscriber::DoCancel");        
    iProperty.Cancel();
    TRACES("CLafShutdownManager::CSubscriber::DoCancel: End");    
    }

// End of file.
