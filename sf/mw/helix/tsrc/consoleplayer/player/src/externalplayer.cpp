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
 * Source file for the main class of the external player executable.
 * 
 */

#include "externalplayer.h"
#include "mmfplayerwindow.h"
#include "testappbase.h"

// This is the implementation for the external player executable.

class CExternalPlayerExe : public CActive
    {
public:
    
    static void ExecuteL()
        {
        CExternalPlayerExe* self = new(ELeave) CExternalPlayerExe;
        CleanupStack::PushL( self );
        self->MainL();
        CleanupStack::PopAndDestroy( self );    
        }
    
private:
    
    CExternalPlayerExe() : CActive( EPriorityStandard )
        {
        CActiveScheduler::Add( this );
        }
    
    ~CExternalPlayerExe()
        {
        Cancel();
        iMsgQueue.Close();
        iChunk.Close();
        delete iVideoPlayer;
        delete iWindowGroup;
        delete iScreenDevice;
        iWs.Close();
        iFs.Close();
        }
    
    void MainL()
        {
        TInt err = KErrNone;
                
        User::LeaveIfError( iFs.Connect() );        
        
        User::LeaveIfError( iWs.Connect() );    
        
        iScreenDevice = new(ELeave) CWsScreenDevice( iWs );
        User::LeaveIfError( iScreenDevice->Construct() );

        TBuf<200> commandLine;
        User::CommandLine( commandLine );

        TInt startIndex = commandLine.Find(KMsgQueuePrefix);
        
        if( startIndex < 0 )
            {
            err = startIndex;
            }
        else
            {
            TPtrC msgQueueName = commandLine.Right( commandLine.Length() - startIndex - KMsgQueuePrefix().Length());
            err = iMsgQueue.CreateGlobal( msgQueueName, 3 );

            if( err == KErrNone )
                {
                TBuf<80> chunkName;
                chunkName.Copy( msgQueueName );
                chunkName.Append( _L("_chunk") );
                err = iChunk.CreateGlobal( chunkName, 2048, 2048 );  // 2K chunk        
                }            
            }
        
        RProcess::Rendezvous( err );
        
        if( err == KErrNone )
            {
            SetActive();
            iMsgQueue.NotifyDataAvailable( iStatus );
        
            CActiveScheduler::Start();
            }
        }
    
    // inherited from CActive
    
    void RunL()
        {        
        if( iStatus == KErrNone )
            {
            TInt err = KErrNone;
        
            TExternalPlayerCommands msg;
            iMsgQueue.Receive( msg );
            
            RDebug::Printf( "EXTERNALPLAYER RunL() msg=%i", msg );
    
            switch( msg )
                {
                case EExternalPlayer_StartPlayback:
                    {
                    TStartPlayback* chunk = (TStartPlayback*)iChunk.Base();
                    
                    TPtrC8 filename( chunk->name, chunk->length );
                    iLocation.Copy( filename ); 
                    iLocationIsFilename = chunk->locationIsFilename;
                    
                    iWindowTopRight.iX  = chunk->x;
                    iWindowTopRight.iY  = chunk->y;
                    iWindowSize.iWidth  = chunk->width;
                    iWindowSize.iHeight = chunk->height;
                    
                    iWindowGroup = new RWindowGroup( iWs );
                    err = iWindowGroup->ConstructChildApp( chunk->idOfParentWindowGroup,  
                                                           KNullWsHandle, 
                                                           false );
                    ASSERT( err == KErrNone );   
                    
                    TStartPlaybackReturn* chunkReturn = (TStartPlaybackReturn*)iChunk.Base();
                    
                    chunkReturn->windowGroupId = iWindowGroup->Identifier();
                    
                    iVideoPlayer = CMmfPlayerWindow::NewL( iFs, 
                                                           iWs, 
                                                           *iScreenDevice, 
                                                           *iWindowGroup,
                                                           iWindowTopRight,
                                                           iWindowSize,
                                                           true,  // transparent
                                                           KRgbTransparent,
                                                           true );
                    
                    if( iLocationIsFilename )
                        {
                        iVideoPlayer->StartWithFilenameL( iLocation, CPlayerWindow::EBestFit);
                        }
                    else
                        {
                        iVideoPlayer->StartWithUrlL( iLocation, CPlayerWindow::EBestFit);                
                        }
                    
                    RDebug::Printf( "EXTERNALPLAYER player started" );
                    break;
                    }
                case EExternalPlayer_ExecuteOperation:
                    {
                    TExecuteOperation* chunk = (TExecuteOperation*)iChunk.Base();
                    err = iVideoPlayer->ExecuteOperation(chunk->operation );
                    break;
                    }
                case EExternalPlayer_MoveWindow:
                    {
                    TMoveWindow* chunk = (TMoveWindow*)iChunk.Base();
                    iVideoPlayer->MoveWindow( TPoint(chunk->centerX, chunk->centerY) );
                    break;
                    }
                case EExternalPlayer_Shutdown:
                    {
                    RDebug::Printf( "EXTERNALPLAYER player stopped" );
                    
                    CActiveScheduler::Stop();
                    break;
                    }
                }
            
            RProcess::Rendezvous( err );
            
            SetActive();
            iMsgQueue.NotifyDataAvailable( iStatus );
            }
        }
    
    void DoCancel()
        {
        iMsgQueue.CancelDataAvailable();
        }    
    
    RFs                                iFs;
    RWsSession                         iWs;
    CWsScreenDevice*                   iScreenDevice; 
    RWindowGroup*                      iWindowGroup;
    CPlayerWindow*                     iVideoPlayer;
    RMsgQueue<TExternalPlayerCommands> iMsgQueue;
    RChunk                             iChunk;
    TFileName                          iLocation;
    bool                               iLocationIsFilename;
    TPoint                             iWindowTopRight;
    TSize                              iWindowSize;
    
    };

GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    
    CActiveScheduler* scheduler = new CActiveScheduler;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if( scheduler != NULL && cleanup != NULL )
    {
        CActiveScheduler::Install( scheduler );
        TRAP_IGNORE( CExternalPlayerExe::ExecuteL() );
    }
    delete cleanup;
    delete scheduler;
    __UHEAP_MARKEND;
    return 0;
    }

