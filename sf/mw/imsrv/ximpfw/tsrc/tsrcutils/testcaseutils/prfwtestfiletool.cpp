/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Test support file tool
 *
*/



#include <e32std.h>
#include <f32file.h>
#include <s32file.h>

#include "prfwtestfiletool.h"
#include "prfwtestfilesrvmsg.h"
#include "ximpeventcodec.h"
#include "prfwtestfileobserver.h"

/* Hierarchy under C:/temp/prfw/:
 * /protocoluid/instancenumber/Plugin\
 * /protocoluid/instancenumber/SrvMsg\
 */

_LIT( KFileToolMainDirFull, "c:\\temp\\prfw" );

_LIT( KFileToolBackslash, "\\" );
_LIT( KFileToolDirUid, "%d" );
_LIT( KInstanceAsNumber, "%d" );

_LIT( KFileToolPluginDirBase, "Plugin" );
_LIT( KFileToolSrvDirBase, "SrvMsg" );

_LIT( KFileToolWildcard, "file*.prfw" );
_LIT( KFileToolFileBase, "file%d.prfw" );

// ======== LOCAL FUNCTIONS ========

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileTool::CXIMPTestFileTool( TUint32 aUid ) :
    iUid( aUid )
    {
    iFs.Connect();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileTool::ConstructL( const TDesC16& aInstanceId )
    {
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iInstance = aInstanceId.AllocL();
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
void CXIMPTestFileTool::ConstructL( TInt aInstanceIdAsNum )
    {
    TInt instanceLength = NumLenInChars( aInstanceIdAsNum );
    HBufC* instance = HBufC::NewLC( instanceLength );
    instance->Des().AppendFormat( KInstanceAsNumber, aInstanceIdAsNum );
    ConstructL( *instance );
    CleanupStack::PopAndDestroy( instance );
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileTool* CXIMPTestFileTool::NewL( TUint32 aUid, const TDesC16& aInstanceId )
    {
    CXIMPTestFileTool* self = new( ELeave ) CXIMPTestFileTool( aUid );
    CleanupStack::PushL( self );
    self->ConstructL( aInstanceId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileTool* CXIMPTestFileTool::NewL( TUint32 aUid, TInt aInstanceIdAsNum )
    {
    CXIMPTestFileTool* self = new( ELeave ) CXIMPTestFileTool( aUid );
    CleanupStack::PushL( self );
    self->ConstructL( aInstanceIdAsNum );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CXIMPTestFileTool::~CXIMPTestFileTool()
    {
    iFs.Close();
    if ( iTimer )
        {
        iTimer->Deque();
        }
    delete iTimer;
    iObservers.Close();
    delete iInstance;
    }

// ---------------------------------------------------------------------------
// Remove the entire prfw directory 
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileTool::CleanAllL()
    {
    RFs fs;
    fs.Connect();
    CleanupClosePushL( fs );

    CFileMan* fileMan = CFileMan::NewL( fs );
    CleanupStack::PushL( fileMan );

    // remove the files and the directory
    fileMan->RmDir( KFileToolMainDirFull );

    CleanupStack::PopAndDestroy( 2 );  // fileman, fs
    }

// ---------------------------------------------------------------------------
// Prepare everything, after this objects can be externalized and
// internalized
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileTool::PrepareL()
    {
    // create the directory structure
    HBufC* dirName = GetDirNameAndBackslashLC( KFileToolPluginDirBase );
    iFs.MkDirAll( *dirName );
    CleanupStack::PopAndDestroy( dirName );
    
    dirName = GetDirNameAndBackslashLC( KFileToolSrvDirBase );
    iFs.MkDirAll( *dirName );
    CleanupStack::PopAndDestroy( dirName );
    }

// ---------------------------------------------------------------------------
// Write the file to correct directory.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileTool::PluginStoreL( const TDesC8& aExternalizedObject )
    {
    HBufC* fileName = GetFileNameLC( 
            KFileToolPluginDirBase,
            iObjIndex );

    RFileWriteStream out;
    CleanupClosePushL( out );
    out.Create( iFs, *fileName, EFileWrite|EFileStream|EFileShareAny );

    // write the file
    TUint32 len = aExternalizedObject.Length();
    out.WriteUint32L( len );
    out.WriteL( aExternalizedObject );
    CleanupStack::PopAndDestroy(); // out

    CleanupStack::PopAndDestroy( fileName ); // fileName

    // next file will have a new index
    iObjIndex++;
    }

// ---------------------------------------------------------------------------
// Restore the file from given directory and given index
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPApiEventBase* CXIMPTestFileTool::PluginRestoreLC(
        TInt aObjIndex )
    {
    HBufC* fileName = GetFileNameLC( 
            KFileToolPluginDirBase, 
            aObjIndex );

    // try to instantiate it
    RFileReadStream in;
    CleanupClosePushL( in );

    in.Open( iFs, *fileName, EFileStream );
    TInt len = in.ReadUint32L();
    HBufC8* buffer = HBufC8::NewLC( len );
    TPtr8 ptr = buffer->Des();
    in.ReadL( ptr );

    // instantiate the object
    TInt32 dummy = 0;
    CXIMPApiEventBase* object = XIMPEventCodec::UnPackL( *buffer, dummy );

    CleanupStack::PopAndDestroy( 2 ); // buffer, in

    // delete the just read object
    iFs.Delete( *fileName );

    CleanupStack::PopAndDestroy( fileName );

    CleanupStack::PushL( object );
    return object;
    }

// ---------------------------------------------------------------------------
// Write the server message object to correct directory.
// ---------------------------------------------------------------------------
//
EXPORT_C void CXIMPTestFileTool::SrvMsgStoreL( 
        CXIMPTestFileSrvMsg* aSrvMsg )
    {
    TInt nextObjIndex = NumSrvMsgL();
    HBufC* fileName = GetFileNameLC( 
            KFileToolSrvDirBase,
            nextObjIndex);

    RFileWriteStream out;
    CleanupClosePushL( out );
    TInt retVal = out.Create( iFs, *fileName, EFileWrite|EFileStream|EFileShareAny );
    User::LeaveIfError( retVal );

    aSrvMsg->ExternalizeL( out );
    
    CleanupStack::PopAndDestroy( 2, fileName ); // out, fileName

    // next file will have a new index
    //iSrvObjIndex++;
    }

// ---------------------------------------------------------------------------
// Restore the file from given directory and given index
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPTestFileSrvMsg* CXIMPTestFileTool::SrvMsgRestoreLC(
        TInt aObjIndex )
    {
    CXIMPTestFileSrvMsg* msg = CXIMPTestFileSrvMsg::NewLC();

    HBufC* fileName = GetFileNameLC( 
            KFileToolSrvDirBase, 
            aObjIndex );

    // try to instantiate it
    RFileReadStream in;
    CleanupClosePushL( in );

    in.Open( iFs, *fileName, EFileStream );

    msg->InternalizeL( in ); 

    CleanupStack::PopAndDestroy(); // in

    // delete the just read object
    iFs.Delete( *fileName );

    CleanupStack::PopAndDestroy( fileName ); // fileName
    return msg;
    }

// ---------------------------------------------------------------------------
// Common file counting helper
// ---------------------------------------------------------------------------
//
TInt CXIMPTestFileTool::DoNumObjectsL( const TDesC& aTemplate )
    {
    CDir* dir = GetDirListL( aTemplate );
    TInt count = dir->Count();
    delete dir;
    return count;
    }

// ---------------------------------------------------------------------------
// Number of objects (files) in a given directory
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPTestFileTool::NumObjectsL()
    {
    return DoNumObjectsL( KFileToolPluginDirBase );
    }

// ---------------------------------------------------------------------------
// Number of server-originated objects (files) in a given directory
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXIMPTestFileTool::NumSrvMsgL()
    {
    // returns the number of objects in the directory
    // get file listing and derive the count from that
    return DoNumObjectsL( KFileToolSrvDirBase );
    }

// ---------------------------------------------------------------------------
// Instace id
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC16& CXIMPTestFileTool::InstanceId() const
    {
    return *iInstance;
    }
    
// ---------------------------------------------------------------------------
// Length of number in characters (e.g. 501 --> 3)
// ---------------------------------------------------------------------------
//
TInt CXIMPTestFileTool::NumLenInChars( TInt aNum )
    {
    TInt len = 0;
    TInt tmp = aNum;
    do
        {
        tmp /= 10;  // number is base-10
        len++;
        }
    while ( tmp > 0 );
    return len;
    }

// ---------------------------------------------------------------------------
// Gets properly expanded directory name with backslash appended
// ---------------------------------------------------------------------------
//
HBufC* CXIMPTestFileTool::GetDirNameAndBackslashLC( 
        const TDesC& aTemplate )
    {
    TInt uidLength = NumLenInChars( iUid );
    TInt instanceLength = iInstance->Length();

    HBufC* dirName = HBufC::NewLC( 
            KFileToolMainDirFull().Length()
            + KFileToolBackslash().Length()
            + KFileToolDirUid().Length()
            -2  // "%d" gets modified to be the number
            + KFileToolBackslash().Length()
            + uidLength + instanceLength
            + KFileToolBackslash().Length()
            + aTemplate.Length()
            + KFileToolBackslash().Length() // added for later use by GetDirNameAndBackslashLC.
            );

    dirName->Des().Append( KFileToolMainDirFull );
    dirName->Des().Append( KFileToolBackslash );
    dirName->Des().AppendFormat( KFileToolDirUid, iUid );
    dirName->Des().Append( KFileToolBackslash );
    dirName->Des().Append( *iInstance );
    dirName->Des().Append( KFileToolBackslash );
    dirName->Des().Append( aTemplate );
    
    dirName->Des().Append( KFileToolBackslash );
    iFs.MkDirAll( *dirName );
    return dirName;
    }

// ---------------------------------------------------------------------------
// Get full path wildcard for files residing in a directory
// with given index
// ---------------------------------------------------------------------------
//
HBufC* CXIMPTestFileTool::GetWildcardLC( 
        const TDesC& aDirTemplate )
    {
    HBufC* dirName = GetDirNameAndBackslashLC( aDirTemplate );
    HBufC* wildcard = HBufC::NewLC(
            dirName->Length() + KFileToolWildcard().Length() );
    wildcard->Des().Append( *dirName );
    wildcard->Des().Append( KFileToolWildcard );

    CleanupStack::Pop( 2 ); // wildcard, dirName
    delete dirName;

    CleanupStack::PushL( wildcard );
    return wildcard;
    }

// ---------------------------------------------------------------------------
// Get a list of files in a given directory
// ---------------------------------------------------------------------------
//
CDir* CXIMPTestFileTool::GetDirListL( const TDesC& aTemplate )
    {
    HBufC* wildcard = GetWildcardLC( aTemplate );

    CDir* dirList = NULL;
    iFs.GetDir( *wildcard, KEntryAttNormal, ESortByName, dirList );
    CleanupStack::PopAndDestroy( wildcard );

    return dirList;
    }

// ---------------------------------------------------------------------------
// Get fully qualified file name
// ---------------------------------------------------------------------------
//
HBufC* CXIMPTestFileTool::GetFileNameLC( 
        const TDesC& aDirTemplate,
        TInt aObjIndex )
    {
    // create file name
    HBufC* dirName = GetDirNameAndBackslashLC( aDirTemplate );

    TInt numLen = NumLenInChars( aObjIndex );
    HBufC* fileName = HBufC::NewLC(
            dirName->Length()
            + KFileToolFileBase().Length()
            + numLen
            - 2  // "%d" gets modified to be the number
            );
    fileName->Des().Append( *dirName );
    fileName->Des().AppendFormat( KFileToolFileBase, aObjIndex );

    CleanupStack::Pop( fileName );
    CleanupStack::PopAndDestroy( dirName );
    CleanupStack::PushL( fileName );

    // now we have the filename
    return fileName;
    }

const TInt KSecond = 1000000;

// ---------------------------------------------------------------------------
// CXIMPTestFileTool::RegisterObserverL
// ---------------------------------------------------------------------------
//
void CXIMPTestFileTool::RegisterObserverL( MXIMPTestFileObserver* aObserver )
    {    
    iObservers.AppendL( aObserver );
    if( !iTimer->IsActive() )
        {
        TCallBack callback( Process, this );
        iTimer->Start( KSecond, KSecond, callback );
        }        
    }
    
// ---------------------------------------------------------------------------
// CXIMPTestFileTool::UnregisterObserver
// ---------------------------------------------------------------------------
//
void CXIMPTestFileTool::UnregisterObserver( MXIMPTestFileObserver* aObserver )
    {    
    TInt index = iObservers.Find( aObserver );
    if( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    if( iObservers.Count() == 0 )
        {
        iTimer->Cancel();
        }
    }


// ---------------------------------------------------------------------------
// CXIMPTestFileTool::RunL
// ---------------------------------------------------------------------------
//
TInt CXIMPTestFileTool::Process( TAny* aMyself )
    {
    TRAPD( ignore, ( static_cast< CXIMPTestFileTool* >( aMyself ) )->DoProcessL() );
    return 0; // ignored by CPeriodic
    }

// ---------------------------------------------------------------------------
// CXIMPTestFileTool::RunL
// ---------------------------------------------------------------------------
//
void CXIMPTestFileTool::DoProcessL()
    {
    TInt countOfSrvMsg = NumSrvMsgL();
    TInt countOfObservers = iObservers.Count();
    
    for( TInt a = 0; a < countOfSrvMsg; ++a )
        {
        CXIMPTestFileSrvMsg* newMsg = SrvMsgRestoreLC( a );
        for( TInt b = 0;b < countOfObservers; ++b )
            {
            iObservers[ b ]->HandleL( newMsg->SrvMsgId(), newMsg->PayloadL() );
            }
        CleanupStack::PopAndDestroy( newMsg );
        }
    }


// End of file
