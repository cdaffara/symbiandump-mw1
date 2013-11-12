/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  cbsserver.cpp
*
*/



//  INCLUDE FILES

#define __INCLUDE_CAPABILITY_NAMES__
#define __INCLUDE_ALL_SUPPORTED_CAPABILITIES__

#include "cbsserver.h"
#include "cbssession.h"
#include "bsserverdefs.h"
#include "DebugTrace.h"
#include "cbsstoragemanager.h"
#include "cbsinstallhandler.h"
#include "cbsbackupobserver.h"

// CBSFileMapping

CBSServer::CBSFileMapping* CBSServer::CBSFileMapping::NewL( CBSSession* aSession,
				    									    const TDesC& aFile,
				    									    TBool aVersioned )
	{
	CBSFileMapping* self = new( ELeave ) CBSFileMapping( aVersioned );
	CleanupStack::PushL( self );
	self->ConstructL( aSession, aFile );
	CleanupStack::Pop( self );
	return self;
	}
void CBSServer::CBSFileMapping::ConstructL(  CBSSession* aSession,
    									     const TDesC& aFile )
	{
	iSession = aSession;
	iFile = aFile.AllocL();
	}
CBSServer::CBSFileMapping::CBSFileMapping( TBool aVersioned )
: iVersioned( aVersioned )
	{
	}
CBSServer::CBSFileMapping::~CBSFileMapping()
	{
	delete iFile;
	}

CBSSession* CBSServer::CBSFileMapping::Session()
	{
	return iSession;
	}
const TDesC& CBSServer::CBSFileMapping::File()
	{
	return *iFile;
	}
TBool CBSServer::CBSFileMapping::Versioned()
	{
	return iVersioned;
	}


// ==============================================================
// =============== PLATSEC POLICY CONFIGURATION =================
// ==============================================================

static const TInt KBSPlatSecRangeCount = 2;

//Ranges for the Request values
static const TInt KBSPlatSecRanges[ KBSPlatSecRangeCount ] =
    {
    0,
    EBSOperationLast
    };


// Element indexes for the defined ranges
static const TUint8 KBSPlatSecElementsIndex[ KBSPlatSecRangeCount ] =
    {
    0,
    CPolicyServer::ENotSupported
    };


// Policy elements
static const CPolicyServer::TPolicyElement KBSPlatSecElements[] =
    {
        {
        _INIT_SECURITY_POLICY_C2( ECapabilityReadUserData,
                                  ECapabilityWriteUserData ),
        -5 //CPolicyServer::EFailClient
        }
    };


// The platsec policy
static const CPolicyServer::TPolicy KBSPlatSecPolicy =
    {
    // Shortcut to the index into Elements,that is used to check a connection attempt
    0,

    // Number of ranges in the iRanges array
    KBSPlatSecRangeCount,

    // A pointer to an array of ordered ranges of request numbers
    KBSPlatSecRanges,

    // A pointer to an array of TUint8 values specifying
    // the appropriate action to take for each range in iRanges
    KBSPlatSecElementsIndex,

    // A pointer to an array of distinct policy elements
    KBSPlatSecElements
    };



// ==============================================================
// ======================= SERVER  ==============================
// ==============================================================
void CBSServer::ExecuteL()
    {
    TRACE( T_LIT( "CBrandingServer::ExecuteL() begin") );
    // start scheduler
    CActiveScheduler* pA = new( ELeave )CActiveScheduler;
    CleanupStack::PushL( pA );
    CActiveScheduler::Install( pA );


    // create server
    CBSServer* server = new( ELeave ) CBSServer();
    CleanupStack::PushL( server );
    server->InitializeL();
    server->StartL( KBSServerName );


    //Signal client that we are started
    RProcess().Rendezvous( KErrNone );

    //Execute the server
    // Codescanner warning: using CActiveScheduler::Start (id:3)
    // this has to be called for server starting.
    CActiveScheduler::Start(); // CSI: 3 # See above


    //Cleanup
    CleanupStack::PopAndDestroy( server );//server
    CleanupStack::PopAndDestroy( pA );
    CActiveScheduler::Install( NULL );
    TRACE( T_LIT( "CBrandingServer::ExecuteL() end") );
    }



CBSServer::~CBSServer()
    {
    TRACE( T_LIT( "CBrandingServer::~CBSServer() begin") );
    delete iBackupObserver;
    delete iInstallHandler;
    iSessions.Close();
    iFileMapping.Close();
    delete iStorageManager;

#if _BullseyeCoverage
    cov_write();
#endif
    TRACE( T_LIT( "CBrandingServer::~CBSServer() end") );
    }



CBSServer::CBSServer()
    : CPolicyServer( CActive::EPriorityStandard,
                     KBSPlatSecPolicy )
    {
    }



CSession2* CBSServer::NewSessionL( const TVersion &aVersion,
                                          const RMessage2& /*aMessage*/ ) const
    {
    TRACE( T_LIT( "CBrandingServer::NewSessionL() begin") );
    TVersion srvVersion( KBSVersionMajor,
                         KBSVersionMinor,
                         KBSVersionBuild );


    if( !User::QueryVersionSupported( aVersion, srvVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    CBSSession* session = CBSSession::NewL();
    //enable backup observer
    iBackupObserver->RegisterObserver( session );
    
    TRACE( T_LIT( "CBrandingServer::NewSessionL() end session[%d] created"), session );
    return session;
    }


CPolicyServer::TCustomResult
    CBSServer::CustomFailureActionL( const RMessage2& aMsg,
                                            TInt /*aAction*/,
                                            const TSecurityInfo& aMissing )
    {
    TRACE( T_LIT( "CBrandingServer::CustomFailureActionL() Request %d to session [%d] failed."),
                   aMsg.Function(), aMsg.Session() );

    TBuf<512> diagnosticMsg;
    _LIT( KDetails, "Failure details: ");
    diagnosticMsg.Append( KDetails );
    _LIT( KFormat, "SecureId[%d] VendorId[%d] Missing caps[" );
    diagnosticMsg.AppendFormat( KFormat,
                                aMissing.iSecureId.iId, aMissing.iVendorId.iId );

    const SCapabilitySet& missingCaps = (const SCapabilitySet&) aMissing.iCaps;
    TBuf<1> separator;
    _LIT( KSeparator, "]" );
	for( TInt ix = 0; ix < ECapability_Limit; ix++ )
		{
		if( missingCaps[ix>>5] &(1<<(ix&31)))
			{
            diagnosticMsg.Append( separator );

            const char* capName = CapabilityNames[ ix ];
		    while( *capName )
		        {
                TUint16 c = *capName;
                diagnosticMsg.Append( &c, 1 );
                ++capName;
                }
			
            separator = KSeparator;
    		}
		}

    diagnosticMsg.Append( KSeparator );
    TRACE( T_LIT( "%S"), &diagnosticMsg );

    return CPolicyServer::EFail;
    }

void CBSServer::HandleBackupStateL( TBackupState aState )
    {
    switch( aState )
        {
        case EBackupNotActive: // backup has completed
            {
            // restart install handler and check for brand changes
            iInstallHandler->InstallNewFilesL();
            iInstallHandler->StartObservingL();
            
            //notify clients that we're back in business
            break;
            }
        case EBackupActive: // backup activated
            {
            // stop install handler 
            iInstallHandler->StopObserving();
            
            // Notify clients that branding data is not currently 
            //       available
            break;
            }
        default:
            {
            // unknown state
            }
        }
    }

void CBSServer::SessionCreatedL( CBSSession* aSession )
    {
    TRACE( T_LIT( "CBSServer::SessionCreatedL begin aSession[%d]"),aSession );
    iSessionCount++;
    iSessions.AppendL( aSession );
    }


void CBSServer::SessionDied( CBSSession* aSession )
    {
    TInt count = iSessions.Count();
	for( TInt i = 0; i < count; i++ )
		{
		if( iSessions[i] == aSession )
			{
			iSessions.Remove( i );
			break;
			}
		}
	TInt ignore = 0;
	TRAP( ignore, UnRegisterSessionL( aSession ) );
	
    //enable backup observer
	iBackupObserver->UnregisterObserver( aSession );
    
    iSessionCount--;
    if( iSessionCount == 0 )
        {
        // Codescanner warning: using CActiveScheduler::Stop ( Id: 4)
        // it is required to stop the server
        CActiveScheduler::Stop(); // CSI: 4 # See above
        }
    }



TBool CBSServer::MatchSessionL( const TDesC& aApplicationId,
							    const TDesC& aBrandId,
							    TLanguage aLanguageId,
							 	CBSSession* aSession,
							 	TInt aReserved  )
	{
	TInt sessionCount = iSessions.Count();
	TBool returnValue = EFalse;
	for( TInt i = 0; i < sessionCount; i++ )
		{
		if( iSessions[i] == aSession )
			{
			// we don't want to report the querying session itself
			continue;
			}
		if( iSessions[i]->MatchSessionL( aApplicationId,
										 aBrandId,
										 aLanguageId,
										 aReserved ) )
			{
			// even one match is enough for us
			returnValue = ETrue;
			break;
			}
		}
	return returnValue;
	}

TBool CBSServer::MatchSessionUninstallL( const TDesC& aApplicationId,
								 const TDesC& aBrandId, CBSSession* aSession)
{
	TInt sessionCount = iSessions.Count();
	TBool returnValue = EFalse;
	for( TInt i = 0; i < sessionCount; i++ )
		{
		if( iSessions[i] == aSession )
			{
			// we don't want to report the querying session itself
			continue;
			}
		if( iSessions[i]->MatchSessionUninstallL( aApplicationId,
										 aBrandId ))
			{
			// even one match is enough for us
			returnValue = ETrue;
			break;
			}
		}
	return returnValue;
	
}

void CBSServer::RegisterFileForSessionL( CBSSession* aSession, 
										 const TDesC& aFile,
										 TBool aVersioned )
	{
	TRACE( T_LIT( "CBSServer::RegisterFileForSessionL begin aSession[%d],aFile[%S]"),aSession,&aFile );
	TBool found  = EFalse;
	TInt count = iFileMapping.Count();		
	for( TInt i = 0; i < count; i ++ )
		{		
		if( iFileMapping[i]->Session() == aSession && iFileMapping[i]->File() == aFile &&
				iFileMapping[i]->Versioned() == aVersioned)
			{
				found = ETrue; 			
			}			
		}
	
	if(!found)
		{			
		
		CBSFileMapping* fileMapping = CBSFileMapping::NewL( aSession, aFile, aVersioned );
		CleanupStack::PushL( fileMapping );
		iFileMapping.AppendL( fileMapping );
		CleanupStack::Pop( fileMapping );
		}
	TRACE( T_LIT( "CBSServer::RegisterFileForSessionL end") );
	}

void CBSServer::UnRegisterSessionL( CBSSession* aSession )
	{
	TRACE( T_LIT( "CBSServer::RegisterFileForSessionL begin aSession[%d]"),aSession );
	TInt count = iFileMapping.Count();
	for( TInt i = 0; i < count; i ++ )
		{
		if( iFileMapping[i]->Session() == aSession )
			{

			if( !iStorageManager )
				{
				iStorageManager = CBSStorageManager::NewL( aSession, KNullDesC );	
				}
			if( !FileStillInUse( aSession, iFileMapping[i]->File() ) )
				{
				TPtrC baseFile = KNullDesC();
				if( iFileMapping[i]->Versioned() )
					{
					baseFile.Set( iStorageManager->FilenameWithoutVersion( iFileMapping[i]->File() ) );
					}
				else
					{
					baseFile.Set( iFileMapping[i]->File() );
					}
				// we found the session, now check if any other
				// session is using the same file
				if( !FileStillInUse( aSession, baseFile ) )
					{
					// no other sessions using the file
					// cleanup versioned file
					CleanupFileL( iFileMapping[i]->File() );
					}					

				}
			CBSFileMapping* fileMapping = iFileMapping[i];
			iFileMapping.Remove( i );
			delete fileMapping;
			break;
			}
		}
	TRACE( T_LIT( "CBSServer::RegisterFileForSessionL end") );
	}

TBool CBSServer::FileStillInUse( CBSSession* aSession,
								 const TDesC& aFile )
	{
	TBool returnValue = EFalse;
	TInt count = iFileMapping.Count();
	for( TInt i = 0; i < count; i ++ )
		{
		if( 0 == iFileMapping[i]->File().Compare( aFile ) )
			{
			// file found, check that it's not the same sesion
			if( iFileMapping[i]->Session() != aSession )
				{

				// it's not the same session
				// so the file is still in use, we can
				// return ETrue
				returnValue = ETrue;
				break;
				}
			}
		}
	return returnValue;
	}
		
void CBSServer::CleanupFileL( const TDesC& aFile )
	{
	if( !iStorageManager )
		{
		iStorageManager = CBSStorageManager::NewL( NULL, KNullDesC);	
		}
	iStorageManager->CleanupFileL( aFile ); 
	}

TArray<CBSServer::CBSFileMapping*> CBSServer::RegisteredFiles()
	{
	return iFileMapping.Array();
	}

void CBSServer::InitializeL()
    {
    TRACE( T_LIT( "CBSServer::InitializeL begin") );
    // Initialize brand install handler
    iInstallHandler = CBSInstallHandler::NewL();
    iInstallHandler->InstallNewFilesL();
    iInstallHandler->StartObservingL();
    
    //to enable backup observer
    // Initialize backup and restore observer
    CBSBackupObserver* tmp = CBSBackupObserver::NewL();
    delete iBackupObserver;
    iBackupObserver = tmp;
    User::LeaveIfError( iBackupObserver->RegisterObserver( this ) );
    TRACE( T_LIT( "CBSServer::InitializeL end") );
    }

void CBSServer::BrandUpdatedL( const TDesC& aApplicationId,
							   const TDesC& aBrandId,
							   TLanguage aLanguageId,
							   CBSSession* aSession,
							   TInt aReserved )
	{
	TRACE( T_LIT( "CBSServer::BrandUpdatedL begin aAppid[%S],aBrandId[%S]"),&aApplicationId, &aBrandId );
	TInt sessionCount = iSessions.Count();
	for( TInt i = 0; i < sessionCount; i++ )
		{
		if( iSessions[i] == aSession )
			{
			// we don't want to report the querying session itself
			continue;
			}
		iSessions[i]->BrandUpdatedL( aApplicationId,
									 aBrandId,
									 aLanguageId,
									 aReserved );
		}
	TRACE( T_LIT( "CBSServer::BrandUpdatedL end") );
	}



// ---------------------------------------------------------------------------
// CBSServer::DisplaySessionInfoL display the info for each open session,
// except for the one that called this method
// ---------------------------------------------------------------------------
//
void CBSServer::DisplaySessionInfoL( CBSSession* aSession, TInt aErrorCode )
	{
    TRACE( T_LIT( "CBSServer::DisplaySessionInfoL() begin aSession[%d], aErrorCode[%d]"),aSession,aErrorCode );

    User::LeaveIfNull( aSession );
    // some constants
    const TInt KStringLength = 512;
    const TInt KNumberLength = 16;
    _LIT( KMessage0,"Operation failed.Errorcode:");    
    _LIT( KMessage1," Info on open sessions [ProcessFileName|ThreadId|ProcessId|Caption]:");
    _LIT( KBracketOpen, " [");
    _LIT( KBracketClose, "]");
    _LIT( KSeparator, "|");
    

    HBufC* outputString = HBufC::NewLC( KStringLength );
    TPtr outputStringPtr( outputString->Des() );

    TBuf<KNumberLength> number;
    number.Num( aErrorCode );
    
    //make sure the string is long enough
    TInt newLength = outputString->Length() +
                     KMessage0().Length() +
                     number.Length() + 
                     KMessage1().Length();
    
    //reallocate if necessary
    if ( outputStringPtr.MaxLength() < newLength )
        {
        CleanupStack::Pop( outputString );
        outputString = outputString->ReAllocL( newLength );
        outputStringPtr.Set( outputString->Des() );
        CleanupStack::PushL( outputString );
        }    
        
    outputStringPtr.Append( KMessage0 );
    outputStringPtr.Append( number );
    outputStringPtr.Append( KMessage1 );
    
        
    TInt count = iSessions.Count();
	TRACE( T_LIT( "CBSServer::DisplaySessionInfoL() numberSessions=%d"),count );
	CBSSession* currentSession;
	for( TInt i = 0; i < count; i++ )
		{		
		currentSession = iSessions[i];
        TRACE( T_LIT( "CBSServer::DisplaySessionInfoL() iteration=%d session=%d"),i,currentSession );
    	if ( currentSession 
    	     && currentSession->InfoAvailable()
    	     && currentSession != aSession )
    	    {
    		TBuf<KNumberLength> threadIdStr;
    		TThreadId threadId;
    		if ( KErrNone == currentSession->ThreadId( threadId ) )
    		    {
    		    threadIdStr.NumUC( threadId.Id(), EDecimal );
    		    }    		
    		TBuf<KNumberLength> processIdStr;
    		TProcessId processId;
    		if ( KErrNone == currentSession->ProcessId( processId ) )
    		    {
    		    processIdStr.NumUC( processId.Id(), EDecimal );
    		    }
            
            //make sure the string is long enough
            newLength = outputString->Length() +
                        KBracketOpen().Length() +
                        currentSession->FileName().Length() + 
                        threadIdStr.Length() + 
                        processIdStr.Length() +
                        currentSession->Caption().Length() +                        
                        ( 3 * KSeparator().Length() ) +                        
                        KBracketClose().Length();

            //reallocate if necessary
            if ( outputStringPtr.MaxLength() < newLength )
                {
                CleanupStack::Pop( outputString );
                outputString = outputString->ReAllocL( newLength );
                outputStringPtr.Set( outputString->Des() );
                CleanupStack::PushL( outputString );
                }

  		    outputStringPtr.Append( KBracketOpen );
    		
    		//processfilename
    		outputStringPtr.Append( currentSession->FileName() );
    		outputStringPtr.Append( KSeparator );
    		    		
    		//threadid    		
    		outputStringPtr.Append( threadIdStr );
    		outputStringPtr.Append( KSeparator );
    		
    		//processid    		
    		outputStringPtr.Append( processIdStr );
    		outputStringPtr.Append( KSeparator );	
    		
    		//caption
    		outputStringPtr.Append( currentSession->Caption() );    		

    		outputStringPtr.Append( KBracketClose );
    	    }
		}     
	
	TRACE( T_LIT( "CBSServer::DisplaySessionInfoL() string creation OK") );
	TRACE( T_LIT( "%S"), outputString );
	CleanupStack::PopAndDestroy( outputString );
	TRACE( T_LIT( "CBSServer::DisplaySessionInfoL() end") );
	}
	
// ==============================================================
// ====================== ENTRY POINT ===========================
// ==============================================================
GLDEF_C TInt E32Main()
    {
    TRACE( T_LIT("E32Main - enter") );

    User::RenameThread( KBSServerName );

    CTrapCleanup* tc = CTrapCleanup::New();
    if( !tc )
        {
        return KErrNoMemory;
        }

    TRAPD( err, CBSServer::ExecuteL() );
    delete tc;

    TRACE( T_LIT("E32Main - exit: %d"), err );
    return err;
    }



//  END OF FILE


