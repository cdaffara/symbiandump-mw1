/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


// USER INCLUDES
#include "CDbRecovery.h"

// SYSTEM INCLUDES
#include <coreapplicationuisdomainpskeys.h>
#include <s32file.h>

#include <cntdb.h>
#include <shareddataclient.h>  // Free disk space request


#ifdef DBRECOVERY_WRITE_LOGS
#include <f32file.h>
#include <utf.h>     // converter
#endif // DBRECOVERY_WRITE_LOGS



// CONSTANTS
_LIT( KDbRecoveryThreadName , "DbRecovery" );
_LIT( KSystemIniFile, "C:\\System\\System.ini" );
_LIT( KSystemIniPath, "C:\\System\\System.ini\\" );

_LIT_SECURITY_POLICY_PASS(KAlwaysPassPolicy);
_LIT_SECURITY_POLICY_C1(KWriteDeviceDataPolicy, ECapabilityWriteDeviceData);

const TInt KSpaceRequiredByContactsDb = 128*1024; // 128kB


//----------------------------------------------------------------------------
// CDbRecovery::NewLC()
//----------------------------------------------------------------------------
CDbRecovery* CDbRecovery::NewLC()
    {
    CDbRecovery* self = new( ELeave ) CDbRecovery;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

//----------------------------------------------------------------------------
// CDbRecovery::~CDbRecovery()
//----------------------------------------------------------------------------
CDbRecovery::~CDbRecovery()
    {
    // Close connections
    iProperty.Close();
    // iDbsSession.Close();
    }

//----------------------------------------------------------------------------
// CDbRecovery::CDbRecovery()
//----------------------------------------------------------------------------
CDbRecovery::CDbRecovery()
    {
    }


//----------------------------------------------------------------------------
// CDbRecovery::ConstructL()
//----------------------------------------------------------------------------
void CDbRecovery::ConstructL()
    {
    // Connect to database server
    // User::LeaveIfError( iDbsSession.Connect() );
    
    User::LeaveIfError( RProperty::Define( KPSUidCoreApplicationUIs,
                                           KCoreAppUIsDbRecoveryContacts,
                                           RProperty::EInt,
                                           KAlwaysPassPolicy,
                                           KWriteDeviceDataPolicy ) );
    
    User::LeaveIfError( RProperty::Set( KPSUidCoreApplicationUIs,
                                        KCoreAppUIsDbRecoveryContacts,
                                        KErrNone ) );
    }


//----------------------------------------------------------------------------
// CDbRecovery::DoContactsDbCheckL()
//----------------------------------------------------------------------------
void CDbRecovery::DoContactsDbCheckL()
    {  
    CContactDatabase* db = NULL;
    TRAPD( err, db = CContactDatabase::OpenL() );
    
    if ( err == KErrDiskFull )
        {
        TRACES("CDbRecovery::CheckContactsDb() - KErrDiskFull - reserving memory");
        // Opening the db failed due to disk space running out.
        // Reserve memory and try again.
        
        RSharedDataClient sd;
        User::LeaveIfError( sd.Connect() );
        sd.RequestFreeDiskSpaceLC(KSpaceRequiredByContactsDb);
        TRAP( err, db = CContactDatabase::OpenL() );
        CleanupStack::PopAndDestroy( &sd ); // frees up the reserved space
        }
    
    if (!err)
        {
        TRACES("CDbRecovery::CheckContactsDb() - Contacts.cdb opened");
        // Opening the db succeeded. If db is damaged try to recover it.
        
        CleanupStack::PushL( db );
        const TBool isDamaged = db->IsDamaged();
        if(isDamaged)
            {
            iProperty.Attach( KPSUidCoreApplicationUIs, KCoreAppUIsDbRecoveryContacts );
            iProperty.Set(KErrCorrupt);
            
            TRACES("CDbRecovery::CheckContactsDb() - Contacts.cdb damaged, trying to recover");
            db->RecoverL();
            TRACES("CDbRecovery::CheckContactsDb() - Recovering completed");
            
            iProperty.Set(KErrNone);        
            }
        CleanupStack::PopAndDestroy( db ); // db.Close()
        TRACES("CDbRecovery::CheckContactsDb() - Contacts.cdb closed");
        }
           
    }



// Common db check currently not used as contacts db used to be the only db:

//----------------------------------------------------------------------------
// CDbRecovery::DoDatabaseCheckL()
//----------------------------------------------------------------------------
/*
void CDbRecovery::DoDatabaseCheckL()
    {
    // Define databases and their PS keys:
    
    // Contacs db
    _LIT(KCntDbPath, "C:Contacts.cdb");
    _LIT(KCntDbSecureFormat,"secure[100065FF]");
    
    const TSingleDatabase KContactsDb( KCntDbPath,
                                       KCntDbSecureFormat,
                                       KCoreAppUIsDbRecoveryContacts );
  
  
    // Add all defined databases into the array
    RArray<TSingleDatabase> dbNames;
    dbNames.Append( KContactsDb );
   
    // For closing the array
    CleanupClosePushL( dbNames );

#ifdef DBRECOVERY_WRITE_LOGS
    HBufC* buffer = HBufC::NewLC( 50 * dbNames.Count()*50 );
    TPtr ptr = buffer->Des();
    TTime time1;
    TTime time2;

    ptr.Append( _L("Check:\r\n") );
#endif

    // 1. Check the databases state:
    TInt i;
    for ( i = 0 ; i < dbNames.Count() ; i++ )
        {
#ifdef DBRECOVERY_WRITE_LOGS
        time1.UniversalTime();
#endif

        // Open the database:
        RDbNamedDatabase database;
        TInt error = database.Open( iDbsSession, dbNames[i].iName, dbNames[i].iSFId );
        if ( error != KErrNone )
            {
#ifdef DBRECOVERY_WRITE_LOGS
            ptr.Append( dbNames[i].iKey );
            ptr.Append( _L(": failed open, r:"));
            ptr.AppendNum( error );
            ptr.Append( _L("\r\n") );
#endif
            continue; // Failed to open db, nothing can be done, try next db
            }

        CleanupClosePushL( database );


        // Check if damaged and write status to the PS key:
        TInt value;
        if ( database.IsDamaged() )
            {
            value = KErrCorrupt;
            }
        else
            {
            value = KErrNone;    
            }

        iProperty.Attach( KPSUidCoreApplicationUIs, dbNames[i].iKey );
        iProperty.Set( value );

        CleanupStack::PopAndDestroy(); // database.Close()

#ifdef DBRECOVERY_WRITE_LOGS
        time2.UniversalTime();
        TTimeIntervalMicroSeconds micro = time2.MicroSecondsFrom( time1 );

        ptr.Append( dbNames[i].iKey );
        ptr.Append( _L(": "));
        ptr.AppendNum( micro.Int64()/1000 );
        ptr.Append( _L("ms r:") );
        ptr.AppendNum( value );
        ptr.Append( _L("\r\n") );
#endif
        }

#ifdef DBRECOVERY_WRITE_LOGS
    ptr.Append( _L("Fixes:\r\n") );
#endif

    // 2. Try to fix damaged databases:
    for ( i = 0 ; i < dbNames.Count() ; i++ )
        {
#ifdef DBRECOVERY_WRITE_LOGS
        time1.UniversalTime();
#endif
        TInt value;
        // Check if the db is not OK
        iProperty.Attach( KPSUidCoreApplicationUIs, dbNames[i].iKey );
        if ( iProperty.Get( value ) == KErrNone )
            {
            if ( value < KErrNone )
                {
                iProperty.Set( KErrInUse );

                // Open and try to recover:
                RDbNamedDatabase database;
                User::LeaveIfError( database.Open( iDbsSession , dbNames[i].iName, dbNames[i].iSFId ) );
                CleanupClosePushL( database );

                TInt error = database.Recover();

                // Write status to the PS key.
                if ( error != KErrNone )
                    {
                    error = KErrNotReady;
                    }

                iProperty.Set( error );
                CleanupStack::PopAndDestroy(); // database.Close()

#ifdef DBRECOVERY_WRITE_LOGS
                time2.UniversalTime();
                TTimeIntervalMicroSeconds micro = time2.MicroSecondsFrom( time1 );
                
                ptr.Append( dbNames[i].iKey );
                ptr.Append( _L(": "));
                ptr.AppendNum( micro.Int64()/1000 );
                ptr.Append( _L("ms r:") );
                ptr.AppendNum( error );
                ptr.Append( _L("\r\n") );
#endif
                }

            }
        }

#ifdef DBRECOVERY_WRITE_LOGS
    RFs fs;
    if ( fs.Connect() == KErrNone )
        {
        CleanupClosePushL( fs );

        RFile file;
        TInt error = file.Replace( fs , _L("c:\\DbRecoveryLog.txt") , EFileStreamText|EFileWrite|EFileShareAny );
        if ( error != KErrNone )
            {
            error = file.Create(  fs , _L("c:\\DbRecoveryLog.txt"), EFileStreamText|EFileWrite|EFileShareAny );
            }
        if ( error == KErrNone )
            {
            CleanupClosePushL( file );

            TBuf8<400> toFile;
            CnvUtfConverter::ConvertFromUnicodeToUtf8( toFile, ptr.Left(400) );
            if ( file.Write( toFile ) == KErrNone )
                {
                file.Flush();
                }

            CleanupStack::PopAndDestroy(); // file.Close()
            }

        CleanupStack::PopAndDestroy(); // fs.Close()
        }

    CleanupStack::PopAndDestroy( buffer );
#endif

    CleanupStack::PopAndDestroy(); // dbNames.Close()
    }
*/

//----------------------------------------------------------------------------
// CDbRecovery::CheckSystemIniFileL()
//----------------------------------------------------------------------------
void CDbRecovery::CheckSystemIniFileL()
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    fs.RmDir( KSystemIniPath );
    TRAPD( err,
        {
        CDictionaryFileStore::SystemLC( fs );
        CleanupStack::PopAndDestroy();
        } );
    
    if ( err != KErrNone )
        {
        fs.Delete( KSystemIniFile );
        }
    
    CleanupStack::PopAndDestroy(); // fs
    }

//----------------------------------------------------------------------------
// PrepareRecoveryL()
//
// Create scheduler:
//----------------------------------------------------------------------------
static void PrepareRecoveryL()
    {
    // Rename thread - ignore error
    User::RenameThread( KDbRecoveryThreadName );

    CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    CDbRecovery* rec = CDbRecovery::NewLC();
    rec->DoContactsDbCheckL();
    rec->CheckSystemIniFileL();
   
    CleanupStack::PopAndDestroy( 2 ); // rec, scheduler
    }


//----------------------------------------------------------------------------
// DLL/EXE entry point
//----------------------------------------------------------------------------
#if defined(__WINS__)
EXPORT_C TInt WinsMain(TAny *)
	{
	return KErrNone;
	}
#endif

//----------------------------------------------------------------------------
// E32Main()
//----------------------------------------------------------------------------
GLDEF_C TInt E32Main()
    {
    __UHEAP_MARK;
    TInt err = KErrNoMemory;
    CTrapCleanup* trap = CTrapCleanup::New();
    if (trap)
        {
        TRAP( err, PrepareRecoveryL() );
        }
    delete trap;

    __UHEAP_MARKEND;
    return err;
    }

//  End of File  
