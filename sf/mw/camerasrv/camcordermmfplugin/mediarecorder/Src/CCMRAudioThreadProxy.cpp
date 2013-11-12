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
* Description:  Audio thread proxy classes
*
*/

//INCLUDES
#include "CCMRAudioThreadProxy.h"
#include "CCMRAudioThreadProxyServer.h"
#include "CCMRThreadPriorities.h"
#include "CCMRAudioCodecs.h"

#include <e32math.h>

//MACROS
// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#define KCMRAudioThreadProxyVersion TVersion(1,0,0)



// ========================= RCMRAudioThreadProxy MEMBER FUNCTIONS =========================

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::CreateSubThread
// Create thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::CreateSubThread(TBool& aThreadCreated)
    {
    PRINT((_L("RCMRAudioThreadProxy::CreateSubThread() in")));
    //start the subthread with no name
    
    TInt error = DoCreateSubThread(&CCMRAudioThreadProxyServer::StartThread, KCMRAudioThreadProxyVersion, aThreadCreated);
    if (error)
        {
        return error;
        }

    iSessionCreated = ETrue;

    PRINT((_L("RCMRAudioThreadProxy::CreateSubThread() out")));
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::SetOutput
// Load datapath
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::SetOutput(CCMRActiveOutput* aOutputAO) const
    {
    PRINT((_L("RCMRAudioThreadProxy::SetOutput()")));
    return SendReceive(ECMRAudioThreadSetOutput, TIpcArgs(aOutputAO, RThread().Id()));
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::AddDataSource
// Add datasource
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::AddDataSource(MDataSource* aSource) const
    {
    PRINT((_L("RCMRAudioThreadProxy::AddDataSource()")));
    return SendReceive(ECMRAudioThreadAddDataSource, TIpcArgs(aSource));
    }
    
// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::SetConfigManager
// Set config manager.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::SetConfigManager(CCMRConfigManager* aConfig) const
    {
    PRINT((_L("RCMRAudioThreadProxy::SetConfigManager()")));
    return SendReceive( ECMRAudioThreadSetConfigManager, TIpcArgs(aConfig ));
    }    

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::SetAudioCodec
// Set used audio codec
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::SetAudioCodec(CCMRAudioCodecData* aAudioCodec) const
    {
    PRINT((_L("RCMRAudioThreadProxy::SetAudioCodec()")));

    TCMRCodecType codecType = ECodecTypeNA;
    TRAPD( err, codecType = aAudioCodec->GetCodecSWHWTypeL());
    if ( err != KErrNone )
        {
        PRINT((_L("RCMRAudioThreadProxy::SetAudioCodec() GetCodecSWHWTypeL returned error %d"), err));
        return err;
        }

    if ( codecType == ECodecTypeSW )
        {
        PRINT((_L("RCMRAudioThreadProxy::SetAudioCodec(), SW codec used, set thread priority to %d"),KCMRAudioThreadPrioritySWCodec));
        iSubThread.SetPriority(KCMRAudioThreadPrioritySWCodec);
        }
    else
        {
        PRINT((_L("RCMRAudioThreadProxy::SetAudioCodec(), HW codec used, set thread priority to %d"),KCMRAudioThreadPriorityHWAccelarated));
        iSubThread.SetPriority(KCMRAudioThreadPriorityHWAccelarated);
        }

    return SendReceive(ECMRAudioThreadSetAudioCodec, TIpcArgs(aAudioCodec));
    }


// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::GetThreadPriority
// Get audio thread priority
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TThreadPriority RCMRAudioThreadProxy::GetThreadPriority() const
    {
    PRINT((_L("RCMRAudioThreadProxy::GetThreadPriority()")));
    return iSubThread.Priority();
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::SetPriority
// Set audio priority
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::SetPriority(const TMMFPrioritySettings &aPrioritySettings) const
    {
    PRINT((_L("RCMRAudioThreadProxy::SetPriority()")));
    return SendReceive(ECMRAudioThreadSetPriority, TIpcArgs(&aPrioritySettings));
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::SetGain
// Set audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::SetGain(TInt aGain) const
    {
    PRINT((_L("RCMRAudioThreadProxy::SetGain()")));
    return SendReceive(ECMRAudioThreadSetGain, TIpcArgs(aGain));
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::GetGain
// Get audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::GetGain(TInt& aGain) const
    {
    PRINT((_L("RCMRAudioThreadProxy::GetGain()")));
    return SendReceive(ECMRAudioThreadGetGain, TIpcArgs(&aGain));
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::MaxGain
// Get max audio gain
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::MaxGain(TInt& aGain) const
    {
    PRINT((_L("RCMRAudioThreadProxy::MaxGain()")));
    return SendReceive(ECMRAudioThreadMaxGain, TIpcArgs(&aGain));
    }


// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::Prime
// Prime datapath
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::Prime() const
    {
    PRINT((_L("RCMRAudioThreadProxy::Prime()")));
    return SendReceive(ECMRAudioThreadPrime);
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::Play
// Start playing (recording)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::Play() const
    {
    PRINT((_L("RCMRAudioThreadProxy::Play()")));
    return SendReceive(ECMRAudioThreadPlay);
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::Pause
// Pause datapath
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::Pause() const
    {
    PRINT((_L("RCMRAudioThreadProxy::Pause()")));
    return SendReceive(ECMRAudioThreadPause);
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::Stop
// Start stopping datapath
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::Stop() const
    {
    PRINT((_L("RCMRAudioThreadProxy::Stop()")));
    return SendReceive(ECMRAudioThreadStop);
    }



// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::WaitUntilStopped
// Wait until audio datapath has stopped
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCMRAudioThreadProxy::WaitUntilStopped()
    {
    PRINT((_L("RCMRAudioThreadProxy::WaitUntilStopped()")));
    return SendReceive(ECMRAudioThreadWaitUntilStopped);
    }

// -----------------------------------------------------------------------------
// RCMRAudioThreadProxy::Close
// Close thread
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCMRAudioThreadProxy::Close()
    {
    PRINT((_L("RCMRAudioThreadProxy::Close() in")));
    Shutdown();
    PRINT((_L("RCMRAudioThreadProxy::Close() out")));
    }







