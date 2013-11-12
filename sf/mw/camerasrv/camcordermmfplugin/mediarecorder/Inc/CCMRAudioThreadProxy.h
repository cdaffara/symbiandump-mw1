/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CCMRAUDIOTHREADPROXY_H
#define CCMRAUDIOTHREADPROXY_H

// INCLUDES

#include "CCMRSubThread.h"

#include <mmf/common/mmfbase.h>


// DATA TYPES


/**
* Audio thread messages ids
*/ 
enum TCMRAudioSubThreadMessageIds
    {
    ECMRAudioThreadSetOutput = ECMRThreadLastCommonMsgId+1,  //combine IDs with TCMRSubThreadMessageIds
    ECMRAudioThreadAddDataSource,
    ECMRAudioThreadSetAudioCodec,
    ECMRAudioThreadSetPriority,
    ECMRAudioThreadSetGain,
    ECMRAudioThreadGetGain,
    ECMRAudioThreadMaxGain,
    ECMRAudioThreadPrime,
    ECMRAudioThreadPlay,
    ECMRAudioThreadPause,
    ECMRAudioThreadStop,
    ECMRAudioThreadWaitUntilStopped,
    ECMRAudioThreadSetConfigManager
    };


// FORWARD DECLARATIONS
class CCMRActiveOutput;
class CCMRAudioCodecData;
class MDataSource;
class CCMRConfigManager;

//CLASS DECLARATIONS

/**
*  Audio thread proxy class
*
*  @lib CAMCMEDIARECORDER.LIB
*  @since 2.1
*/
class RCMRAudioThreadProxy : public RCMRSubThreadBase
    {
    public: // Constructor
        /**
        * C++ default constructor.
        */
        RCMRAudioThreadProxy() : RCMRSubThreadBase(KCMRThreadProxyShutdownTimeout) {};
    public: // new functions
        /**                     
        * Create thread
        * @param  TBool& aThreadCreated; thread may be created but session not necessarily so return value doesn't tell all
        * @return TInt error code
        */
        TInt CreateSubThread(TBool& aThreadCreated);
        /**                     
        * Set output (sink) for audio
        * @param CCMRActiveOutput* aOutputAO
        * @return TInt error code
        */
        TInt SetOutput(CCMRActiveOutput* aOutputAO) const;
        /**                     
        * Add datasource
        * @param MDataSource* aSource
        * @return TInt error code
        */
        TInt AddDataSource(MDataSource* aSource) const;
        /**                     
        * Set Set config manager.
        * @since 3.2
        * @param  TInt aConfig
        * @return TInt error code
        */
        TInt SetConfigManager(CCMRConfigManager* aConfig) const;          
        /**                     
        * SetAudioCodec
        * @param MDataSink* aSink
        * @return TInt error code
        */
        TInt SetAudioCodec(CCMRAudioCodecData* aAudioCodec) const;
        /**                     
        * Get audio thread priority
        * @return TThreadPriority aThreadPriority
        */
        TThreadPriority GetThreadPriority() const;
        /**                     
        * Set audio policy priority
        * @return TInt error code
        */
        TInt SetPriority(const TMMFPrioritySettings& aPrioritySettings) const;
        /**                     
        * Set audio gain
        * @return TInt error code
        */
        TInt SetGain(TInt aGain) const;
        /**                     
        * Get audio gain
        * @return TInt error code
        */
        TInt GetGain(TInt& aGain) const;
        /**                     
        * Get max audio gain
        * @return TInt error code
        */
        TInt MaxGain(TInt& aGain) const;
        /**                     
        * Prime datapath
        * @return TInt error code
        */
        TInt Prime() const;
        /**                     
        * Start playing (recording)
        * @return TInt error code
        */
        TInt Play() const;
        /**                     
        * Pause datapath
        * @return TInt error code
        */
        TInt Pause() const;
        /**                     
        * Stop datapath
        * @return TInt error code
        */
        TInt Stop() const;

        /**                     
        * Wait until audio datapath has stopped
        * @return TInt error code
        */
        TInt WaitUntilStopped();

        /**                     
        * Close thread
        * @return TInt error code
        */
        void Close();
    };

#endif      
            
// End of File



