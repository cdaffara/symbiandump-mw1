/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * The header for the main class of the ConsolePlayer test app.
 * 
 */

#ifndef __CONSOLE_PLAYER_H__
#define __CONSOLE_PLAYER_H__

#include <e32msgqueue.h>

#include "hxtnutil.h"
#include "testappbase.h"

const TInt KMaxHistoryEntries = 10;
_LIT( KHistoryFilename, "c:\\thumbnail_tester_history.txt" );

class CThumbnailTester : public CTestAppBase,
                         private MHXThumbnailUtilityImplObserver
                       
{
public: // Methods

    static void ExecuteL();
    
private: // Methods

    CThumbnailTester(); 

    ~CThumbnailTester();

    void InitL();

    void Main();
        
    void MainL();
    
    void GenerateThumbnailL( TDes& aFileName );
    
    // inherited from CTestAppBase
    virtual bool ConsumeKeyEvent( TInt aKeyCode );
    virtual void ExecuteOperation( TInt aOperation, const TDesC& aOperationText );

private:
    
    // inherited from MHXThumbnailUtilityImplObserver
    virtual void MetaDataReady(TInt aError);
    virtual void PacketReady(TInt aError, void *pData, TUint32 aDataSize);    
    virtual void EndOfPackets();
    
    TBuf<120>             iBuffer;
    RWindow*              iFullScreenWindow;    
    CHXThumbnailUtility*  iThumbnailUtility;
    TInt                  iFinalResult;
    TInt                  iStartPositionInMs;
    TInt                  iFramesRendered;
    TInt                  iTotalFramesRendered;
    TInt                  iWidth;
    TInt                  iHeight;
    TUint                 iClipBitRate;
    TUint                 iVideoBitRate;
    TUint                 iAudioBitRate;
    TUint                 iFrameRateInSec;
    TUint                 iDuration;
    TInt                  iTotalFrameCount;
    TInt                  iDelayBetweenFrames;
    bool                  iStopThumbnailGeneration;
    
    };

#endif
