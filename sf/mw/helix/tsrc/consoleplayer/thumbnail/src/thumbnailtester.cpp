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
 * Source file for the main class of the Thumbnail Tester test app.
 * 
 */

#include "thumbnailtester.h"
#include "yuv2rgb24.h"

#include <e32base.h>
#include <e32debug.h>
#include <f32file.h>

const TInt KFontSize = 15;

// Thumbnail tester implementation

void CThumbnailTester::ExecuteL()
    {
    CThumbnailTester* self = new(ELeave) CThumbnailTester;
    CleanupStack::PushL( self );
    self->InitL();
    self->Main();
    CleanupStack::PopAndDestroy( self );    
    }
    
CThumbnailTester::CThumbnailTester() : 
    CTestAppBase( KFontSize )
    {    
    }
    
CThumbnailTester::~CThumbnailTester()
    {
    Cancel();
    
    if( iGc && iFullScreenWindow )
	      {
        iGc->Activate(*iFullScreenWindow);
        
        iFullScreenWindow->Invalidate();
        iFullScreenWindow->BeginRedraw();
        
        iGc->Reset();
        
        iGc->UseFont(iFont);                
        iGc->SetBrushColor(KRgbDarkBlue);
        
        iGc->Clear();

        if( iFinalResult == KErrNone )
            {
            // KRgbWhite seems to be having problems (0xffffff) in some emulators,
            // but 0xfefefe is working, so use that instead of white.        
            iGc->SetPenColor(0xfefefe);                 
            }
        else
            {
            iGc->SetPenColor(KRgbRed);
            }
    
        iBuffer.Format( _L("THUMBNAILTESTER EXIT, result = %i"), iFinalResult );
        iGc->DrawText( iBuffer, TPoint(5, 20) );
       
        iFullScreenWindow->EndRedraw();
        
        iGc->Deactivate();
        
        iFullScreenWindow->SetVisible( ETrue );
        
        WaitForAnyKey();
        }    
    
    }
    
void CThumbnailTester::InitL()
    {
    BaseConstructL( NULL, 0 );  // this app doesn't use the option pages concept
    
    iFullScreenWindow = new(ELeave) RWindow( iWs );
    User::LeaveIfError( iFullScreenWindow->Construct( *iWindowGroup, KNullWsHandle ) );
    iFullScreenWindow->SetExtent( TPoint(0,0), iDisplaySize );
    iFullScreenWindow->SetNonTransparent();
    iFullScreenWindow->SetBackgroundColor(KRgbBlack);
    iFullScreenWindow->SetVisible(false);
    iFullScreenWindow->Activate();
        
    iGc->Activate(*iFullScreenWindow);

    iFullScreenWindow->BeginRedraw();
    
    iGc->Reset();    
    iGc->UseFont(iFont);                
    iGc->SetBrushStyle(CWindowGc::ESolidBrush);
    iGc->SetBrushColor(KRgbBlack);    
    iGc->Clear();
    
    iFullScreenWindow->EndRedraw();
    
    iGc->Deactivate();    
    }

void CThumbnailTester::Main()
    {    
    TRAPD( err, MainL() ); 
    
    RDebug::Printf( "THUMBNAILTESTER Completed, err=%i", err );    
    }

void CThumbnailTester::MainL()        
    {
    TFileName filename;
    
    SelectFileWithHistoryL( TPoint(0,0), iDisplaySize, filename, KHistoryFilename, KMaxHistoryEntries );
    
    GenerateThumbnailL( filename );
    }
  
void CThumbnailTester::GenerateThumbnailL( TDes& aFileName )
    {
    SelectIntegerL( TPoint(0,0), iDisplaySize, _L("Select starting position(seconds):"), 0, 9999, iStartPositionInMs );
    
    // Convert from seconds to ms.
    iStartPositionInMs *= 1000;
        
    _LIT( KFull,   "Full speed" );
    _LIT( KMedium, "100 millisecond delay" );
    _LIT( KSlow,   "1 second delay" );

    RPointerArray<TDesC> speed;
    speed.Append( &KFull );
    speed.Append( &KMedium );
    speed.Append( &KSlow );
    
    TInt index = SelectFromListL( TPoint(0,0), iDisplaySize, _L("Select speed:"), speed );
    
    speed.Reset();
    
    switch( index )
        {
        case 0:
            iDelayBetweenFrames = 0;
            break;
        case 1:
            iDelayBetweenFrames = 100000;  // 1/10 second
            break;
        case 2:
            iDelayBetweenFrames = 1000000;  // 1 second
            break;
        }

    RFile file;    
    User::LeaveIfError( file.Open( iFs, aFileName, EFileShareReadersOnly | EFileStream | EFileRead ) );
    
    iThumbnailUtility = CHXThumbnailUtility::NewL(*this);

    bool done = false;

    iStopThumbnailGeneration = false;
    
    StartMonitoringWindowEvents();
    
    TInt originalStartPositionInMs = iStartPositionInMs;
    
    while( !done )
        {
        iThumbnailUtility->OpenFileL( file, iStartPositionInMs );

        // OpenFileL will block until all bitmaps have been passed to PacketReady.
        
        // This isn't necessarily exact, but advance the start position by the time represented
        // by the number of renderered frames.
        iStartPositionInMs = originalStartPositionInMs + (iTotalFramesRendered * 1000) / iFrameRateInSec;
                
        done = ((iFramesRendered < 5) || iStopThumbnailGeneration);
        
        iFramesRendered = 0;
        }
    
    Cancel();
 
    WaitForAnyKey();
    
    delete iThumbnailUtility;
    iThumbnailUtility = NULL;
    
    file.Close();    
    }
           
bool CThumbnailTester::ConsumeKeyEvent( TInt /*aKeyCode*/ )
    {
    // Stop thumbnail generation when a key is pressed.
    iStopThumbnailGeneration = true;
    iThumbnailUtility->CancelThumb();
    return true;
    }

void CThumbnailTester::ExecuteOperation( TInt aOperation, const TDesC& aOperationText )
    {    
    }

void CThumbnailTester::MetaDataReady(TInt aError)
    {
    RDebug::Printf( "THUMBNAILTESTER MetadataReady(%i)", aError );
    
    // Get basic meta data.
    
    TUint lCount = 0;
    TUint i=0;
    
    iThumbnailUtility->GetMetaDataCount(lCount);

    RDebug::Printf( "THUMBNAILTESTER metadata count=%i", lCount );
    
    for (i=0; i<lCount; i++)
        {
        HBufC *pDes = NULL;           
        HXMetaDataKeys::EHXMetaDataId id;        
        iThumbnailUtility->GetMetaDataAt(i, id, pDes);

        const char* key = "";
        
        for( TInt j = 0; j < sizeof(HXMetaDataKeyTab)/sizeof(HXMetaDataKeys); j++ )
            {
            if( id == HXMetaDataKeyTab[j].m_id )
                {
                key = HXMetaDataKeyTab[j].m_pHXKey;
                }
            }
        
        for( TInt j = 0; j < sizeof(HXStreamMetaDataKeyTab)/sizeof(HXMetaDataKeys); j++ )
            {
            if( id == HXStreamMetaDataKeyTab[j].m_id )
                {
                key = HXStreamMetaDataKeyTab[j].m_pHXKey;
                }
            }
        
        RDebug::Printf( "THUMBNAILTESTER metadata[%i] id=%i key=%s", i, id, key );        
        
        TInt current = 0;
        TBuf8<400> outputLine;
        while( current < pDes->Length() )
            {
            char c = pDes->Des()[current];                
            char c2 = c;
            if( c2 < ' ' || c2 > '~' )
                {
                c2 = ' ';
                }
            
            outputLine.AppendFormat(_L8("%02x/%c "), c, c2 ); 
            
            current++;
            
            if( (current % 20 == 0) || current == pDes->Length()  )
                {
                outputLine.Append( '\0' );
                RDebug::Printf( (char*)outputLine.Ptr() );
                outputLine.SetLength(0);
                }            
            }        
        
        if (id == HXMetaDataKeys::EHXFrameSize && pDes)
            {
            TPtr pFrameSizePtr = pDes->Des();
            _LIT(KChar_x, "x");
            TInt xLoc = pFrameSizePtr.Find(KChar_x);
            if (xLoc != KErrNotFound)
                {                           
                TLex lexWidth(pFrameSizePtr.Mid(0, xLoc));
                TLex lexHeight(pFrameSizePtr.Mid(xLoc+1));
                lexWidth.Val(iWidth);  // Storing into local iWidth variable
                lexHeight.Val(iHeight);  // Storing into local iHeight variable  
                }
            }
        else if (id == HXMetaDataKeys::EHXDuration && pDes)
            {
            TLex lexDuration(pDes->Des());
            lexDuration.Val(iDuration);
            }        
        else if (id == HXMetaDataKeys::EHXFramesPerSecond && pDes)
            {
            TLex lexFramesPerSecond(pDes->Des());           
            lexFramesPerSecond.Val(iFrameRateInSec);            
            }
        else if (id == HXMetaDataKeys::EHXClipBitRate && pDes)
            {
            TLex lexBitRate(pDes->Des());           
            lexBitRate.Val(iClipBitRate);            
            }
        else if (id == HXMetaDataKeys::EHXVideoBitRate && pDes)
            {
            TLex lexBitRate(pDes->Des());           
            lexBitRate.Val(iVideoBitRate);            
            }
        else if (id == HXMetaDataKeys::EHXAudioBitRate && pDes)
            {
            TLex lexBitRate(pDes->Des());           
            lexBitRate.Val(iAudioBitRate);            
            }
        
        } // end for
    
    iTotalFrameCount = ( (iDuration+500)/1000 ) * iFrameRateInSec;
    
    RDebug::Printf( "THUMBNAILTESTER width=%i", iWidth );
    RDebug::Printf( "THUMBNAILTESTER height=%i", iHeight );
    RDebug::Printf( "THUMBNAILTESTER frame rate=%i", iFrameRateInSec );
    RDebug::Printf( "THUMBNAILTESTER duration=%i", iDuration );
    RDebug::Printf( "THUMBNAILTESTER frame count=%i", iTotalFrameCount );
    RDebug::Printf( "THUMBNAILTESTER clip bit rate=%i", iClipBitRate );
    RDebug::Printf( "THUMBNAILTESTER video bit rate=%i", iVideoBitRate );
    RDebug::Printf( "THUMBNAILTESTER audio bit rate=%i", iAudioBitRate );
    }

void CThumbnailTester::PacketReady(TInt /*aError*/, void *pYuvBuffer, TUint32 /*aYuvBufferSize*/)
    {
    // TODO: HANDLE aError
    
    iFramesRendered++;
    iTotalFramesRendered++;
    
    // Convert from YUV420 to RGB24.
    
    const TInt KBytesPerPixel = 3; // 24-bit rgb takes 3 bytes
    
    TUint32 rgbBufferSize = iWidth * iHeight * KBytesPerPixel;
    TUint8 *pRgbBuffer =(TUint8*) User::AllocL(rgbBufferSize);
        
    CYuv2Rgb24* yuvConverter = NULL; 
    TRAPD(err, yuvConverter = new(ELeave) CYuv2Rgb24);
    
    // TODO: CHECK ERROR

    TRAP(err, yuvConverter->ConstructL(iWidth, iHeight, iWidth, iHeight))

    // TODO: CHECK ERROR

    TInt scanLineLength = iWidth * KBytesPerPixel;
    
    TUint8* yBuf = (TUint8*)pYuvBuffer;
    TUint8* uBuf = yBuf + iWidth*iHeight;
    TUint8* vBuf = uBuf + (iWidth*iHeight)/4;    
    
    yuvConverter->Convert(yBuf, uBuf, vBuf, iWidth, iHeight, pRgbBuffer, scanLineLength);
    
    delete yuvConverter;        

    // Use RGB24 buffer to create CFbsBitmap.
    
    CFbsBitmap* pBitmap = NULL;
    
    TRAP(err, pBitmap = new (ELeave) CFbsBitmap);
    if( err == KErrNone )
        {
        err = pBitmap->Create(TSize(iWidth, iHeight), EColor16M); 
    
        if( err == KErrNone )
            {
            // fill image from rgb buffer to input bitmap buffer 
            TPtr8 linePtr(0,0); 
            TInt lineLength = pBitmap->ScanLineLength(iWidth, EColor16M); 
            for(int j=0, i=0; j<iHeight; j++, i+=lineLength)
                {
                linePtr.Set(pRgbBuffer+i, lineLength, lineLength);
                pBitmap->SetScanLine((TDes8&)linePtr,j); 
                }
                
            iGc->Activate(*iSelectionWindow);
            
            iSelectionWindow->Invalidate();
            iSelectionWindow->BeginRedraw();
            
            iGc->Reset();
            
            iGc->UseFont(iFont);                
            iGc->SetBrushColor(KRgbDarkBlue);
            
            iGc->Clear();
            
            // KRgbWhite seems to be having problems (0xffffff) in some emulators,
            // but 0xfefefe is working, so use that instead of white.        
            iGc->SetPenColor(0xfefefe);

            TPoint topLeft( iDisplaySize.iWidth/2 - iWidth/2, 
                            iDisplaySize.iHeight/2 - iHeight/2 ); 
            iGc->BitBlt( topLeft, pBitmap );
            
            const TInt KColumn = 3;
            const TInt KRowIncrement = KFontSize + 1;
            TInt row = KRowIncrement;

            iBuffer.Format( _L("Frame # %i"), iFramesRendered );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;

            iBuffer.Format( _L("Total frames rendered: %i"), iTotalFramesRendered );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;

            iBuffer.Format( _L("Start time(ms): %i"), iStartPositionInMs );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;

            iBuffer.Format( _L("Display size: %ix%i"), iDisplaySize.iWidth, iDisplaySize.iHeight );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;
            
            iBuffer.Format( _L("Video size: %ix%i"), iWidth, iHeight );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Total frame count: %i"), iTotalFrameCount );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Duration: %i"), iDuration );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Frame rate: %i"), iFrameRateInSec );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Clip bit rate: %i"), iClipBitRate );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Video bit rate: %i"), iVideoBitRate );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            
            
            iBuffer.Format( _L("Audio bit rate: %i"), iAudioBitRate );
            iGc->DrawText( iBuffer, TPoint(KColumn, row) );
            row += KRowIncrement;            

            iGc->DrawText( _L("<Press key to stop then another to exit>"), TPoint(KColumn, iDisplaySize.iHeight-10) );
            
            iSelectionWindow->EndRedraw();
            
            iGc->Deactivate();
            
            iSelectionWindow->SetVisible( ETrue );            
            }
        
        delete pBitmap;
        }
    
    delete pRgbBuffer;    

    User::After( iDelayBetweenFrames );    
    }

void CThumbnailTester::EndOfPackets()
    {
    RDebug::Printf( "THUMBNAILTESTER end of packets" );
    }

GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    
    CActiveScheduler* scheduler = new CActiveScheduler;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if( scheduler != NULL && cleanup != NULL )
    {
        CActiveScheduler::Install( scheduler );
        TRAP_IGNORE( CThumbnailTester::ExecuteL() );
    }
    delete cleanup;
    delete scheduler;
    REComSession::FinalClose();
    __UHEAP_MARKEND;
    return 0;
    }

