/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Includes some common defines used in the OOD monitor app
*
*/


#ifndef LAFSHUT_H
#define LAFSHUT_H

// SYSTEM INCLUDES
#include "hbdevicemessageboxsymbian.h"
// USER INCLUDES

// CONSTANTS
_LIT(KDriveZ, "z:");
_LIT(KOOMWatcherResourceFileName, "lafmemorywatcher.rsc");
const TInt KAlphaStartPoint = 65;
const TInt KBreatheTime = 5000000;
#ifdef _DEBUG
_LIT(KCriticalMode, "%c: CRITICAL");
_LIT(KWarningMode, "%c: WARNING");
_LIT(KNormalMode, "%c: Normal");
const TInt KMaxMsgSize = 256;
#endif // _DEBUG

// ---------------------------------------------------------
//
// ---------------------------------------------------------
//
NONSHARABLE_CLASS(CLafShutdownManager) : public CLafShutdownManagerBase
    {  // There are friends defined at end of this class definition.
public:
    static CLafShutdownManager* NewL(MShutdownEventObserver& aObserver);
    ~CLafShutdownManager();

private:

    // ======================================================================
    // Out of disk watcher
    // ======================================================================
    NONSHARABLE_CLASS(COutOfDiskWatcher) : public CActive // friend of CLafShutdownManager
        {
    public:
        static COutOfDiskWatcher* NewL(CLafShutdownManager& aLafShutdown,
                                       TInt64 aThreshold,
                                       TDriveNumber aDrive,
                                       RFs& aFs);
        ~COutOfDiskWatcher();
        void Start();
    private:
        COutOfDiskWatcher(CLafShutdownManager& aLafShutdown,
                          TInt64 aThreshold,
                          TDriveNumber aDrive,
                          RFs& aFs);
        void ConstructL();
    private: // from CActive
        void DoCancel();
        void RunL();
    private: // data
        RFs& iFs;
        CLafShutdownManager& iLafShutdown;
        TInt64 iThreshold;
        TDriveNumber iDrive;
        };

    NONSHARABLE_CLASS(CGlobalQueryActive) : public CBase
        {
        public:
            enum TMessageType{
                ENone          = -1,
                ECritical      =  0,
                EWarning       =  1,
                EWarningMMC    =  2,
                ECriticalMMC   =  3,
                ECallBack      =  4,

                EClearNotesC   = 10,
                EClearNotesMMC = 11
                };

        public:
            static CGlobalQueryActive* NewL(RResourceFile& aResFile, TResourceReader& aResReader);
            ~CGlobalQueryActive();

            void DisplayL(TMessageType aType, TBool aForcedNote);

        private:
            CGlobalQueryActive();
            void ConstructL(RResourceFile& aResFile, TResourceReader& aResReader);
            void CleanupL(TBool aCancel = ETrue);

        private:
            class CMessageInfo : public CBase
                {
                public:
                    CMessageInfo(HBufC* aMessage):
                        iMessage(aMessage), iNoteId(KErrNotFound){};
                    ~CMessageInfo() { delete iMessage; };
                public:
                    HBufC* iMessage;
                    TInt   iNoteId;
                };

        private:
            TMessageType    iMessageType;
            CMessageInfo*   iMessageInfo[4];
        
        
        };

    // ======================================================================

    NONSHARABLE_CLASS(CSubscriber) : public CActive
        {
        public:
            CSubscriber(TCallBack aCallBack, RProperty& aProperty);
            ~CSubscriber();

        public: // New functions
            void SubscribeL();
            void StopSubscribe();

        private: // from CActive
            void RunL();
            void DoCancel();

        private:
            TCallBack   iCallBack;
            RProperty&  iProperty;
        };

private:
    CLafShutdownManager(MShutdownEventObserver& aObserver);
    void ConstructL();
    void ReportEventL(MSaveObserver::TSaveType aAction,TBool aPowerOff);
    void GetShutdownState(TBool& aPowerOff, TBool& aAllSessionsHavePendingRequest);
    void FreeDiskThresholdCrossedL(TDriveNumber aDrive, TBool aIsCallBack = EFalse, TBool aForcedNote = EFalse);
    void ShowGlobalQueryL(const CGlobalQueryActive::TMessageType& aMessageType, TBool aForcedNote);
    TInt GetDiskStatusL(TVolumeInfo& aVolInfo,TDriveNumber aDrive);
    static TInt DisplayNotesCallBackL(TAny* aPtr);
    void HandleDisplayNotesCallBackL();
    static TInt AutolockStatusCallBackL(TAny* aPtr);
    void HandleAutolockStatusCallBackL();
    void HandleMMCCallBackL();
    static TInt MMCStatusChanged(TAny* aPtr);

private:

    RWsSession              iWs;
    RFs                     iFs;
    COutOfDiskWatcher*      iOODWarningWatcher;
    COutOfDiskWatcher*      iOODCriticalWatcher;
    TUint                   iOODWarningThreshold;
    TUint                   iOODCriticalThreshold;
    RProperty               iDiskStatusProperty;
    RProperty               iMMCStatusProperty;
    RProperty               iCanDisplayNotesProperty;
    RProperty               iAutolockStatusProperty;
    RProperty               iEnableMMCWatchProperty;
    RProperty               iWatchdogStatusProperty;
    CSubscriber*            iDisplayNotesSubscriber;
    CSubscriber*            iAutolockStatusSubscriber;
    CSubscriber*            iMMCWatchSubscriber;
    CSubscriber*            iWatchdogStatusSubscriber;
    CGlobalQueryActive*     iGlobalQueryActive;
    COutOfDiskWatcher*      iOOMMCWarningWatcher;
    COutOfDiskWatcher*      iOOMMCCriticalWatcher;
    TInt                    iAutolockStatus;

private:
    friend class CLafShutdownManager::COutOfDiskWatcher;
    };

#endif LAFSHUT_H