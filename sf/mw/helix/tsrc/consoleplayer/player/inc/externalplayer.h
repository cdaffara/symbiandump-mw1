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
 * Header for the external player main class.
 * 
 */

#ifndef __EXTERNAL_PLAYER_H__
#define __EXTERNAL_PLAYER_H__

#include <e32base.h>
#include <e32msgqueue.h>
#include <w32std.h>

#include "PlayerWindow.h"

// Note: the executable name is also used as the message queue name.
_LIT( KExternalPlayerExe, "externalplayer.exe" );
_LIT( KExternalPlayerExe2, "externalplayer2.exe" );

enum TExternalPlayerCommands
    {
    EExternalPlayer_StartPlayback,
    EExternalPlayer_ExecuteOperation,
    EExternalPlayer_MoveWindow,
    EExternalPlayer_Shutdown
    };

struct TStartPlayback
    {
    TInt x;
    TInt y;
    TInt width;
    TInt height;
    TInt idOfParentWindowGroup;
    bool locationIsFilename;
    TInt length;
    unsigned char name[200];
    };

struct TStartPlaybackReturn
    {
    TInt windowGroupId;
    };

struct TExecuteOperation
    {
    TInt operation;
    };

struct TMoveWindow
    {
    TInt centerX;
    TInt centerY;
    };

_LIT( KMsgQueuePrefix, "-msg=" );

// This is the client-side class for launching and controlling an external player executable.

class CExternalPlayer : public CBase, public MPlayerCommands
    {
public:
    
    ~CExternalPlayer()
        {
        Terminate();
        }
    
    CExternalPlayer( RWsSession& aWs ) : iWs( aWs )
        {        
        }
    
    TInt SendMsg( TExternalPlayerCommands aMsg )
        {
        TRequestStatus status;
        status = KRequestPending;
        iProcess.Rendezvous( status );
        
        iMsgQueue.SendBlocking( aMsg );
                
        User::WaitForRequest( status );   
        
        return status.Int();
        }
    
    TInt Launch( const TDesC&  aExeName, 
                 RWindowGroup& aParentWindowGroup, 
                 const TDesC&  aLocation,
                 bool          aLocationIsFilename, // false means location is URL
                 TPoint        aTopRight, 
                 TSize         aSize )
        {
        // The base name for message queue and chunk is the name of the executable followed by the address of this instance.
        TBuf<80> nameBase;
        nameBase.Copy( aExeName );
        nameBase.AppendFormat(_L("%08x"), this );
        
        TBuf<100> commandLineArguments;
        commandLineArguments.Copy( KMsgQueuePrefix );
        commandLineArguments.Append( nameBase );
        
        TInt err = iProcess.Create( aExeName, commandLineArguments );

        if( err == KErrNone )
            {
            aParentWindowGroup.AllowProcessToCreateChildWindowGroups( iProcess.SecureId() );
        
            iProcess.Resume();
            
            TRequestStatus status;
            status = KRequestPending;
            iProcess.Rendezvous( status );
            User::WaitForRequest( status );
            
            err = status.Int();
                    
            if( err == KErrNone )
                {  
                err = iMsgQueue.OpenGlobal( nameBase );
                }
            
            if( err == KErrNone )
                {
                nameBase.Append( _L("_chunk") );
                err = iChunk.OpenGlobal( nameBase, EFalse );
                }
                
            if( err == KErrNone )
                {
                iActive = true;
                
                // Send the start messages to the external player.
                
                TStartPlayback* chunk = (TStartPlayback*)iChunk.Base();
                
                TPtr8 filename( chunk->name, 0, 200 );
                filename.Copy( aLocation );
                chunk->length = filename.Length();            
                chunk->locationIsFilename = aLocationIsFilename;
                
                chunk->x = aTopRight.iX;
                chunk->y = aTopRight.iY;
                chunk->width = aSize.iWidth;
                chunk->height = aSize.iHeight;
                chunk->idOfParentWindowGroup = aParentWindowGroup.Identifier();
                err = SendMsg( EExternalPlayer_StartPlayback );
                
                if( err == KErrNone )
                    {
                    TStartPlaybackReturn* chunk = (TStartPlaybackReturn*)iChunk.Base();
                    iWindowGroupId = chunk->windowGroupId;                            
                    }
                }
            else
                {
                iProcess.Kill( KErrCancel );
                iProcess.Close();     
                }
            
            }
        
        return err;        
        }

    // inherited from MPlayerCommands
    TInt ExecuteOperation( TInt aOperation )
        {
        TInt err = KErrNone;
        
        if( iActive )
            {
            TExecuteOperation* chunk = (TExecuteOperation*)iChunk.Base();
            chunk->operation = aOperation;            
            err = SendMsg( EExternalPlayer_ExecuteOperation );
            }
        
        return err;
        }
    
    // inherited from MPlayerCommands
    void MoveWindow( TPoint aNewCenter )
        {
        if( iActive )
            {
            TMoveWindow* chunk = (TMoveWindow*)iChunk.Base();
            chunk->centerX = aNewCenter.iX;            
            chunk->centerY = aNewCenter.iY;            
            SendMsg( EExternalPlayer_MoveWindow );
            }
        }
    
    void SetOrdinalPosition( TInt /*aPosition*/ )
        {
        if( iActive )
            {        
// TODO: DEBUG THIS.  HOW DOES WINDOW GROUP ORDINAL POSITION RELATE TO WINDOW ORDINAL POSITION?        
//            iWs.SetWindowGroupOrdinalPosition( iWindowGroupId, aPosition );
            }
        }
    
    void Terminate()
        {
        if( iActive )
            {
            SendMsg( EExternalPlayer_Shutdown );
        
            iProcess.Close();
            iMsgQueue.Close();
            
            iActive = false;
            }
        }
    
private:
    
    RWsSession&                        iWs;
    RProcess                           iProcess;
    bool                               iActive;
    RMsgQueue<TExternalPlayerCommands> iMsgQueue;
    RChunk                             iChunk;
    TInt                               iWindowGroupId;

    };
    
#endif

