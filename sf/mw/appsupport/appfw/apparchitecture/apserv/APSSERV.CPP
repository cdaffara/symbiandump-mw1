// Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apsserv.cpp
//

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#if !defined(__APA_INTERNAL_H__)
#include "apainternal.h"
#endif
#include "apaidpartner.h"
#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
#include <e32svr.h>
#include <u32hal.h>
#include "apsserv.h"
#include "APFREC.H"
#include "APSSES.H"
#include "APSSTD.H"
#include "../aplist/aplapplistitem.h"
#include "APSSCAN.H"
#include "APSSTD.H"
#include "APASVST.H"
#include <datastor.h>
#include "APSRECCACHE.h"
#include "../apgrfx/apprivate.h"
#include "apgnotif.h"

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
#include<usif/scr/scr.h>
#include<usif/scr/appregentries.h>
#include<swi/sisregistrysession.h>
#else
#include "../aplist/aplappregfinder.h"
#include "apsnnapps.h"
#include "../apfile/apinstallationmonitor.h"
#endif


_LIT(KAppArcServerSemaphore,"AppArcServerSemaphore");
_LIT(KAppArcServerThread,"AppArcServerThread");

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
_LIT(KAppRegistrationFileImportLocation, "?:\\private\\10003a3f\\import\\apps\\");
_LIT(KAppResourceAppsLocation, "?:\\resource\\apps\\");
_LIT(KNonNativeApplicationTypeRegistry, ":\\private\\10003a3f\\NonNativeTypes.dat");

//To monitor all drives.
const TInt KApaMonitorAllDrives = 0x3FFFFFF;
#endif

/*
 * patchable const data values defined in ApsConstData.cpp
 */

IMPORT_C extern const TInt KApaLoadDataRecognizersOnDemand;
IMPORT_C extern const TInt KApaUnloadRecognizersTimeout;
IMPORT_C extern const TInt KApaDrivesToMonitor;
IMPORT_C extern const TInt KApaLoadMbmIconsOnDemand;

const TUint8 KPolicyElementWriteDeviceData = 0;

const TUint KRangeCount = 3; 

const TInt KAppListServRanges[KRangeCount] = 
	{	
	EFirstUnrestrictedOpcodeInAppListServ,
	EFirstOpcodeNeedingWriteDeviceDataInAppListServ, 
	EAppListFirstUnusedOpcode,
	};

const TUint8 KElementsIndex[KRangeCount] =
	{
	CPolicyServer::EAlwaysPass,			//Always passing no capability required [0-99]
	KPolicyElementWriteDeviceData,		//Requires WriteDeviceData				 [100-(EAppListFirstUnusedOpcode-1)]
	CPolicyServer::ENotSupported, 		//Not Supported		[EAppListFirstUnusedOpcode-End]
	};

const CPolicyServer::TPolicyElement KPolicyElements[] = 
	{ 
	{_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient} 
	};

const CPolicyServer::TPolicy KApaServPolicy =
	{
	CPolicyServer::EAlwaysPass, 
	KRangeCount,
	KAppListServRanges,
	KElementsIndex, 	
	KPolicyElements 	
	};
 	


//
// CApaAppArcServer
//

const TInt KAppListServerPriority=CActive::EPriorityStandard;
/**
NameApaServStartSemaphore

@internalTechnology
@released
*/
EXPORT_C TPtrC NameApaServStartSemaphore()
	{
	TPtrC nameApaServStartSemaphore(KAppArcServerSemaphore);
	return nameApaServStartSemaphore;
	}

/**
NameApaServServerThread

@internalTechnology
@released
*/
EXPORT_C TPtrC NameApaServServerThread()
	{
	TPtrC nameApaServServerThread(KAppArcServerThread);
	return nameApaServServerThread;
	}

EXPORT_C CApaAppArcServer* CApaAppArcServer::Self()
	{ // static
	return static_cast<CApaAppArcServer*>(Dll::Tls());
	}

EXPORT_C CApaAppArcServer* CApaAppArcServer::NewL()
// Create a new CApaAppArcServer which owns it's own resources
	{
	CApaAppArcServer* self=new(ELeave) CApaAppArcServer(KAppListServerPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CApaAppArcServer::CApaAppArcServer(TInt aPriority)
	: CPolicyServer(aPriority,KApaServPolicy),
	iAppList(0),
	iTypeStoreModified(0),
	iLoadRecognizersOnDemand(KApaLoadDataRecognizersOnDemand),
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	iForceRegistrationStatus(EForceRegistrationNone),
#endif
	iLoadMbmIconsOnDemand(KApaLoadMbmIconsOnDemand)
	{
	
#ifdef __WINS__
	// KApaLoadDataRecognizersOnDemand and KApaloadIconsOnDemand are Rom patchable constants,
	// so need an emulator equivalent
	// if WINS then read value from epoc.ini
	// requires licencees to set property in epoc.ini

	TInt halValue = 0;
	if (UserSvr::HalFunction(EHalGroupEmulator, EEmulatorHalIntProperty, (TAny*)"patchdata_apserv_dll_KApaLoadDataRecognizersOnDemand", &halValue) == KErrNone)
		iLoadRecognizersOnDemand = halValue;

	if (UserSvr::HalFunction(EHalGroupEmulator, EEmulatorHalIntProperty, (TAny*)"patchdata_apserv_dll_KApaLoadMbmIconsOnDemand", &halValue) == KErrNone)
		iLoadMbmIconsOnDemand = halValue;
#endif
	}

void CApaAppArcServer::ConstructL()
	{
	StartL(KAppListServerName);
	User::LeaveIfError(Dll::SetTls(this));
	User::LeaveIfError(iFs.Connect());

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
    //Connect to sisregistrysession to initially populate the applications information and 
    //store it to SCR. 
    Swi::RSisRegistrySession sisReg;
    sisReg.Connect();
    sisReg.Close();
#endif

    
	// Get the idle timeout delay from the commandline if specified. The default is 50000ms
	const TInt cmdLineLen = User::CommandLineLength();
	TInt idlePeriodicDelay=50000; //default value
	if(cmdLineLen)
		{
		_LIT(KIdleTimeout,"IDLE_TIMEOUT_PERIODIC_DELAY_");
		
		// Extract the command line into a buffer
		HBufC* cmdLine = HBufC::NewLC(cmdLineLen);
		TPtr cmdLinePtr = cmdLine->Des();
		User::CommandLine(cmdLinePtr);
		cmdLinePtr.UpperCase();
				
		// Check if there is an idle timeout value given
		TInt idleTimeOutArgPos = cmdLinePtr.Find(KIdleTimeout);
		if(KErrNotFound != idleTimeOutArgPos)
			{
			// Extract the value out of the command line argument
			const TInt idleTimeOutValuePos = idleTimeOutArgPos + KIdleTimeout().Length();
			TInt i = idleTimeOutValuePos;
			while (i < cmdLineLen)
				{
				TChar c(cmdLinePtr[i]);
				if (!c.IsDigit())
					{
					break;
					}
				i++;
				}
			TLex idleTimeOutParser(cmdLinePtr.Mid(idleTimeOutValuePos, (i-idleTimeOutValuePos)));
		  	User::LeaveIfError(idleTimeOutParser.Val(idlePeriodicDelay));	  					  	
			}
							
		// Check if the recognition cache should be used or not. By default the cache is used
		iRecognitionCache = 0;
		_LIT(KWithoutRecognitionCache,"WITHOUT_RECOGNITION_CACHE");
		if(KErrNotFound == cmdLinePtr.Find(KWithoutRecognitionCache))
			{
			iRecognitionCache = new (ELeave) CApsRecognitionCache(iFs);
			}
			
		_LIT(KTextShellMode,"TEXT_SHELL_MODE");
		if (KErrNotFound == cmdLinePtr.Find(KTextShellMode))
			{
			// We are NOT running in text shell mode so connect to wserv session
			User::LeaveIfError(iWsSession.Connect());
			iRuleBasedPlugIns = CApaScanningRuleBasedPlugIns::NewL();
			}

		CleanupStack::PopAndDestroy(cmdLine);		
		}
	else
		{
		// no arguments so cache is used
		iRecognitionCache = new (ELeave) CApsRecognitionCache(iFs);
		// and rule based plugins are enabled
		User::LeaveIfError(iWsSession.Connect());
		iRuleBasedPlugIns = CApaScanningRuleBasedPlugIns::NewL();
		}
	
	iAppList=CApaAppList::NewL(iFs, iLoadMbmIconsOnDemand, idlePeriodicDelay); // takes ownership of scanner

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	// If the phone rebooted halfway through processing updates, there will be a log file left
	// look for one and recover if neccessary
	CApsNonNativeApplicationsManager::RecoverFromUpdateLogL(iFs);
#endif	

	iMimeTypeRecognizer=CApaScanningDataRecognizer::NewL(iFs, !iLoadRecognizersOnDemand);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	ConstructPathsToMonitorL();	

	if ( iAppFsMonitor )
		{
		iAppFsMonitor->Start(ENotifyFile);
		iAppFsMonitor->SetBlocked(ETrue);			
		}
#endif
	
	TRAP_IGNORE(iAppList->InitListL(this));
	
	//
	iRecEcomMonitor=CApaEComMonitor::NewL(TCallBack(&PlugInNotifyCallBack,this));
	iRecEcomMonitor->Start();

	// Create the EMIME type store manager (part of the app-framework_emime component)
	iMimeTypeToAppMappingsManager=CTypeStoreManager::NewL(iFs);
	TRAPD(err,iMimeTypeToAppMappingsManager->RestoreL());
	if(!err)
		User::LeaveIfError(iFs.Modified(iMimeTypeToAppMappingsManager->IniFileName(),iTypeStoreModified));

	// Create the EMIME type store monitor
	iTypeStoreMonitor = CApaFsMonitor::NewL(iFs,iMimeTypeToAppMappingsManager->IniFileName(),TCallBack(&TypeStoreNotifyCallback,this));
	iTypeStoreMonitor->Start(ENotifyWrite); // this presumably needs to be ENotifyWrite rather than ENotifyFile (the latter being used or the other CApaFsMonitor objects) because CTypeStoreManager internally uses CDictionaryFileStore::OpenL, which presumably itself uses RFile::Open, which isn't covered by ENotifyFile according to its documentation
	TypeStoreNotifyCallback(this);

	//
	iBaBackupSessionWrapper=CBaBackupSessionWrapper::NewL();
	iBaBackupSessionWrapper->RegisterBackupOperationObserverL(*((MBackupOperationObserver*)this));

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	TRAP_IGNORE(InitNonNativeApplicationTypeArrayL());
#else
    //
    TChar sysDrive = RFs::GetSystemDriveChar();
    TInt maxSizeofFileName = KNonNativeApplicationTypeRegistry().Length() + 1;
    iNonNativeApplicationTypeRegistry.CreateL(maxSizeofFileName);
    iNonNativeApplicationTypeRegistry.Append(sysDrive);
    iNonNativeApplicationTypeRegistry.Append(KNonNativeApplicationTypeRegistry());
	
    TRAP_IGNORE(InternalizeNonNativeApplicationTypeArrayL());   // We don't want a corrupt file to prevent from starting	
#endif
    
	if(iLoadRecognizersOnDemand)
		iRecognizerUnloadTimer=CPeriodic::NewL(EPriorityNormal);

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	iAppInstallationMonitor = CApaAppInstallationMonitor::NewL(this);
	iAppInstallationMonitor->Start();
#endif	
	}
	
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
void CApaAppArcServer::ConstructPathsToMonitorL()
	{
	TInt drivesToMonitor = KApaDrivesToMonitor;
	#ifdef __WINS__
	// KApaDrivesToMonitor is a Rom patchable constant, so need an emulator equivalent
	// if WINS then read value from epoc.ini requires licencees to set property in epoc.ini
	// Usage: In epoc.ini patchdata_apserv_dll_KApaDrivesToMonitor 4
	
	TInt valueOfKApaDrivesToMonitor = 0;
	if (UserSvr::HalFunction(EHalGroupEmulator,EEmulatorHalIntProperty,(TAny*)"patchdata_apserv_dll_KApaDrivesToMonitor",&valueOfKApaDrivesToMonitor) == KErrNone)
		{
		drivesToMonitor = valueOfKApaDrivesToMonitor;
		}
	#endif
	
	if ( drivesToMonitor != KApaMonitorAllDrives )
		{
		// 1. Create and add CApaFsNotifier for all locations belonging to a drive if the drive is set to monitor.
		// 2. Do this for all the drives which are required to monitor.
		TInt maskBit = 1;
		TChar driveLetter;
		//Only bits from 0(EDriveA)- 25(EDriveZ) are valid other bits are ignored from 32-bit KApaDrivesToMonitor.
		for ( TInt i = EDriveA; i <= EDriveZ; i++ )
			{
			if ( drivesToMonitor & maskBit )
				{
				RFs::DriveToChar(i,driveLetter);
				RBuf pathToBeMonitored;
				TBuf<1> drive;
				drive.Append(driveLetter);
				
				//Creating or Adding <driveLetter>:\private\10003a3f\import\apps\ path to monitor.
				pathToBeMonitored.CreateL(KAppRegistrationFileImportLocation());				
				CleanupClosePushL(pathToBeMonitored);
				pathToBeMonitored.Replace(0,1,drive);				
				if ( iAppFsMonitor == NULL )
					{
					iAppFsMonitor=CApaFsMonitor::NewL(iFs, pathToBeMonitored, TCallBack(&AppFsNotifyCallBack, this));
					}
				else
					{
					iAppFsMonitor->AddLocationL(pathToBeMonitored);		
					}
				CleanupStack::PopAndDestroy(&pathToBeMonitored);
			
				//Adding <driveLetter>:\resource\apps\ path to monitor.
				pathToBeMonitored.CreateL(KAppResourceAppsLocation());
				CleanupClosePushL(pathToBeMonitored);
				pathToBeMonitored.Replace(0,1,drive);			
				iAppFsMonitor->AddLocationL(pathToBeMonitored);
				CleanupStack::PopAndDestroy(&pathToBeMonitored);
				}
			maskBit = maskBit << 1;
			}	
		}
	else
		{
		iAppFsMonitor=CApaFsMonitor::NewL(iFs, KAppRegistrationFileImportLocation, TCallBack(&AppFsNotifyCallBack, this));
		iAppFsMonitor->AddLocationL(KAppResourceAppsLocation);
		}	
	}
#endif

EXPORT_C CApaAppArcServer::~CApaAppArcServer()
	{
	if(iBaBackupSessionWrapper)
		iBaBackupSessionWrapper->DeRegisterBackupOperationObserver(*this);
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	
	delete iAppInstallationMonitor;
    delete iAppFsMonitor;
    iNonNativeApplicationTypeRegistry.Close();    
#endif
	delete iAppList; // deletes scanners
	delete iMimeTypeRecognizer;
	delete iMimeTypeToAppMappingsManager;
	delete iTypeStoreMonitor;
	delete iBaBackupSessionWrapper;
	delete iRecognitionCache;
	delete iRecEcomMonitor;

	iFs.Close();
	iWsSession.Close();	
	delete 	iRuleBasedPlugIns;
	for (TInt i = iNonNativeApplicationTypeArray.Count()-1; i >= 0; --i)
		delete iNonNativeApplicationTypeArray[i].iNativeExecutable;

	iNonNativeApplicationTypeArray.Close();

	delete iRecognizerUnloadTimer;
	}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK  
EXPORT_C void CApaAppArcServer::HandleInstallationStartEvent()
	{
	if ( iAppFsMonitor )
		{
		iAppFsMonitor->SetBlocked(ETrue);	
		}
	AppList().StopScan();
	}

EXPORT_C void CApaAppArcServer::HandleInstallationEndEventL()
	{
	if ( iAppFsMonitor )
		{
		iAppFsMonitor->SetBlocked(EFalse);	
		}
	AppList().RestartScanL();
	}
#endif

CSession2* CApaAppArcServer::NewSessionL(const TVersion& aVersion,const RMessage2&/* aMessage*/) const
// Create a new server session.
	{
	// check we're the right version
	TVersion v(KAppListServMajorVersionNumber, KAppListServMinorVersionNumber, KAppListServBuildVersionNumber);
	if (!User::QueryVersionSupported(v,aVersion))
		User::Leave(KErrNotSupported);
	// make new session
	return CApaAppArcServSession::NewL(const_cast<CApaAppArcServer&>(*this), const_cast<RFs&>(iFs));
	}

//
// scanning code here
//
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
EXPORT_C TCallBack CApaAppArcServer::RescanCallBack()
	{
	return TCallBack(&AppFsNotifyWithForcedRegistrationsResetCallBack,this);
	}

TInt CApaAppArcServer::AppFsNotifyWithForcedRegistrationsResetCallBack(TAny* aPtr)
	{
	ASSERT(aPtr);
	reinterpret_cast<CApaAppArcServer*>(aPtr)->AppList().ResetForcedRegistrations();
	return AppFsNotifyCallBack(aPtr);
	}

TInt CApaAppArcServer::AppFsNotifyCallBack(TAny* aObject)
	{
	ASSERT(aObject);
	reinterpret_cast<CApaAppArcServer*>(aObject)->UpdateApps();
	return KErrNone;
	}
#endif

TInt CApaAppArcServer::PlugInNotifyCallBack(TAny* aObject)
	{
	//update the pre-Platform-security style  recognizers and rule-based plug-ins
	ASSERT(aObject);
	reinterpret_cast<CApaAppArcServer*>(aObject)->UpdatePlugIns();
	return KErrNone;
	}

TInt CApaAppArcServer::TypeStoreNotifyCallback(TAny* aObject)
	{
	ASSERT(aObject);
	reinterpret_cast<CApaAppArcServer*>(aObject)->UpdateTypeStore();
	return KErrNone;
	}

#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
void CApaAppArcServer::UpdateApps()
// update the list
	{
	// File system change, rescan
	TRAP_IGNORE(iAppList->StartIdleUpdateL (this));
	}

/*
Updates application list with force registered applications.
*/
 
void CApaAppArcServer::UpdateAppsByForceRegistration()
    {
    iForceRegistrationStatus|=EForceRegistrationRequested;
    UpdateApps();
    }	
#endif

void CApaAppArcServer::NotifyUpdate(TInt aReason)
// tell all sessions to update their clients
	{
	// Updates the applist with the icon caption details from the Central Repository.
	TRAP_IGNORE(iAppList->UpdateAppListByIconCaptionOverridesL());
	// The short caption value sets through the API has got the highest precedence over the
	// values found in either central repository or resource file.
	TRAP_IGNORE(iAppList->UpdateAppListByShortCaptionL());

    //Delete any data mappings in service registry related to uninstalled applications.
    CArrayFixFlat<TUid>* uninstalledApps=iAppList->UninstalledAppArray();
    TBool modificationStatus=EFalse;
    TBool modified;
    
    if(uninstalledApps)
        {
        TInt count=uninstalledApps->Count();
        for(int index=0;index<count;index++)
            {
            modified=iMimeTypeToAppMappingsManager->DeleteApplicationDataMappings((*uninstalledApps)[index]);
        
            //Set modificationStatus if any data mapping is removed from the service registry
            if(modified)
                modificationStatus=ETrue;
            }
        
        delete uninstalledApps;
        
        //If service registry is modified, store it to a file
        if(modificationStatus)
            TRAP_IGNORE(iMimeTypeToAppMappingsManager->StoreL());
        }
   
	// iterate through sessions
	iSessionIter.SetToFirst();
	CApaAppArcServSession* ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
	
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    //Get the updated application information from iAppList
    CArrayFixFlat<TApaAppUpdateInfo>* updatedAppsInfo=iAppList->UpdatedAppsInfo();

    while (ses!=NULL)
        {
        //Call session object NotifyClients and pass the updated application information.
        ses->NotifyClients(aReason, updatedAppsInfo);    
        ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
        }
#else
	
	while (ses!=NULL)
		{
		if(iForceRegistrationStatus & EForceRegistrationRequested)
            {
             //Notify clients about completion of force registration. 
            ses->NotifyScanComplete();
            }
		else
            {
            ses->NotifyClients(aReason);
            }
		ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
		}
	
     if(iForceRegistrationStatus & EForceRegistrationRequested)
         {
         //If this function is called because of force registration, clear force registration request status and
        //set force registration applist change status
        iForceRegistrationStatus &= (~EForceRegistrationRequested);
         iForceRegistrationStatus |= EForceRegistrationAppListChanged;
         }
     else
         {
         //If this function is called not because of force registration, clear force registration applist change status. 
        iForceRegistrationStatus &= (~EForceRegistrationAppListChanged);        
         }
#endif     
	}

void CApaAppArcServer::UpdatePlugIns()
// update the pre-Platform-security style, ecom style recognizers and rule-based plug-ins
	{
	//we want the recognizers to be loaded either if:
	// 1) they are not loaded on demand
	// 2) they are loaded on demand and actively used
	// 3) they are loaded on demand but waiting for the timer to be unloaded
	if(!iLoadRecognizersOnDemand || iRecognizerUsageCount > 0 || (iRecognizerUnloadTimer && iRecognizerUnloadTimer->IsActive()))
		{
		TRAP_IGNORE(iMimeTypeRecognizer->LoadRecognizersL());
		}
	if (iRecognitionCache)
		{	// RecognitionCache is flushed if there are any changes in plugins, i.e.,if:
			// 1. New ECom Plugin is installed.
			// 2. Existing ECom Plugin is uninstalled.
		iRecognitionCache->Flush();
		}

	TRAP_IGNORE(iRuleBasedPlugIns->ScanForRuleBasedPlugInsL());
	}

void CApaAppArcServer::UpdateTypeStore()
// Update the internal type store if things have changed
	{
	TRAP_IGNORE(DoUpdateTypeStoreL());
	}

void CApaAppArcServer::DoUpdateTypeStoreL()
	{
	TTime modified;
	TInt err=iFs.Modified(iMimeTypeToAppMappingsManager->IniFileName(),modified);
	// check the time stamp to see if we are interested in an update
	if (modified>iTypeStoreModified && err==KErrNone)
		{
		CTypeStoreManager* manager=CTypeStoreManager::NewL(iFs);
		CleanupStack::PushL(manager);
		manager->RestoreL();
		iTypeStoreModified=modified;
		delete iMimeTypeToAppMappingsManager;
		iMimeTypeToAppMappingsManager=manager;
		CleanupStack::Pop(manager); 
		}
		
	for (iSessionIter.SetToFirst(); ; iSessionIter++) //lint !e1757 prefix operator not defined for TDblQueIter
		{
     	CSession2* const session=iSessionIter;
     	if (!session)
  			break;

     	static_cast<CApaAppArcServSession*>(session)->NotifyClientOfDataMappingChange();
		}
	}

void CApaAppArcServer::HandleBackupOperationEventL(const TBackupOperationAttributes& aBackupOperationAttributes)
//
// Handle a signal from the backup server to indicate that a backup has started or finished.
//
	{
	switch(aBackupOperationAttributes.iOperation)
		{
	case MBackupOperationObserver::ENone:
	case MBackupOperationObserver::EAbort:
		break;
	case MBackupOperationObserver::EStart:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK   
		if ( iAppFsMonitor )
			{
			iAppFsMonitor->SetBlocked(ETrue);	
			}
#endif		
		break;
	case MBackupOperationObserver::EEnd:
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK	    
		if ( iAppFsMonitor )
			{
			iAppFsMonitor->SetBlocked(EFalse);	
			}
#endif		
		break;
	default:
		Panic(EEventFromBackupObserverError);
		break;
		}
	}

void CApaAppArcServer::InitialListPopulationComplete()
	{
#ifndef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK    
	if ( iAppFsMonitor )
		{
		iAppFsMonitor->SetBlocked(EFalse);	
		}
#endif	
	
	// notify clients (whoever is interested) that initial population of list is completed
	iSessionIter.SetToFirst();
	CApaAppArcServSession* ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
	// iterate through sessions
	while (ses!=NULL)
		{
		ses->NotifyClientForCompletionOfListPopulation();	
		ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
		}
	}


#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK

/* 
 * Cleanup RPointerArray
 */
void CleanupAndDestroyLauncherArray(TAny* aRPrray)
    {
    RPointerArray<Usif::CLauncherExecutable>* rpArray = (static_cast<RPointerArray<Usif::CLauncherExecutable>*>(aRPrray));
    rpArray->ResetAndDestroy();
    rpArray->Close();
    }


/* 
 * Gets non-native type to its run time mapping information from SCR and initialzes 
 * a mapping array
 */
void CApaAppArcServer::InitNonNativeApplicationTypeArrayL()
    {
     Usif::RSoftwareComponentRegistry scrSession;
     User::LeaveIfError(scrSession.Connect());
     CleanupClosePushL(scrSession);
     
     RPointerArray<Usif::CLauncherExecutable> launchers;
     //Get non-native type to its run-time mappings 
     scrSession.GetApplicationLaunchersL(launchers);
     TCleanupItem cleanup(CleanupAndDestroyLauncherArray, &launchers); 
     CleanupStack::PushL(cleanup);     

     //Get each mapping and add it to mapping array
     for(TInt index=0;index<launchers.Count();index++)
         {
         Usif::CLauncherExecutable* launcherInfo=launchers[index];
         SNonNativeApplicationType nonNativeApplicationType;
         nonNativeApplicationType.iTypeUid.iUid=launcherInfo->TypeId();
         nonNativeApplicationType.iNativeExecutable=launcherInfo->Launcher().AllocLC();
         iNonNativeApplicationTypeArray.AppendL(nonNativeApplicationType);
         CleanupStack::Pop(nonNativeApplicationType.iNativeExecutable);     
         }
     CleanupStack::PopAndDestroy(2, &scrSession);
    }

void CApaAppArcServer::UpdateAppListL(RArray<TApaAppUpdateInfo>* aAppUpdateInfo, TUid aSecureID)
{
   iAppList->UpdateApplistL(this, aAppUpdateInfo, aSecureID);
}
#else

void CApaAppArcServer::RegisterNonNativeApplicationTypeL(TUid aApplicationType, const TDesC& aNativeExecutable)
    {
    for (TInt i=iNonNativeApplicationTypeArray.Count()-1; i>=0; --i)
        {
        if (iNonNativeApplicationTypeArray[i].iTypeUid.iUid==aApplicationType.iUid)
            User::Leave(KErrAlreadyExists);
        }
        
    SNonNativeApplicationType nonNativeApplicationType;
    nonNativeApplicationType.iTypeUid.iUid=aApplicationType.iUid;
    nonNativeApplicationType.iNativeExecutable=aNativeExecutable.AllocLC();
    iNonNativeApplicationTypeArray.AppendL(nonNativeApplicationType);
    CleanupStack::Pop(nonNativeApplicationType.iNativeExecutable);
    CleanupStack::PushL(TCleanupItem(DeleteLastNonNativeApplicationType, this));
    ExternalizeNonNativeApplicationTypeArrayL();
    CleanupStack::Pop(this); // the TCleanupItem
    }

void CApaAppArcServer::DeregisterNonNativeApplicationTypeL(TUid aApplicationType)
    {
    TInt i;
    for (i=iNonNativeApplicationTypeArray.Count()-1; i>=0; --i)
        {
        if (iNonNativeApplicationTypeArray[i].iTypeUid.iUid==aApplicationType.iUid)
            break;
        }
        
    if (i>=0)
        {
        ExternalizeNonNativeApplicationTypeArrayL(i);
        delete iNonNativeApplicationTypeArray[i].iNativeExecutable;
        iNonNativeApplicationTypeArray[i].iNativeExecutable = NULL;
        iNonNativeApplicationTypeArray.Remove(i);
        }
    }

void CApaAppArcServer::InternalizeNonNativeApplicationTypeArrayL()
	{
	RFile file;
	CleanupClosePushL(file);
	const TInt error=file.Open(iFs, iNonNativeApplicationTypeRegistry, EFileShareReadersOnly|EFileStream|EFileRead);
	if (error==KErrNone) // don't leave if the file can't be opened (because it doesn't exist, or because the directory we're looking for it in doesn't exist)
		{
		RFileReadStream sourceStream;
		sourceStream.Attach(file); // file gets closed by this call, but that's okay, we don't need it any more (sourceStream has its own copy of this RFile object that it owns)
		CleanupClosePushL(sourceStream);
		TCardinality arrayCount;
		arrayCount.InternalizeL(sourceStream);
		for (TInt i=0; i<TInt(arrayCount); ++i)
			{
			SNonNativeApplicationType nonNativeApplicationType;
			nonNativeApplicationType.iTypeUid.iUid=sourceStream.ReadUint32L();
			nonNativeApplicationType.iNativeExecutable=HBufC::NewLC(sourceStream, KMaxFileName);
			iNonNativeApplicationTypeArray.AppendL(nonNativeApplicationType);
			CleanupStack::Pop(nonNativeApplicationType.iNativeExecutable);
			}
		CleanupStack::PopAndDestroy(&sourceStream);
		}
		
	CleanupStack::PopAndDestroy(&file);
	}


void CApaAppArcServer::ExternalizeNonNativeApplicationTypeArrayL(TInt aIndexToIgnore/*=-1*/) const
    {
    RFs& fs=const_cast<RFs&>(iFs);
    fs.MkDirAll(iNonNativeApplicationTypeRegistry); // ignore any error
    RFile file;
    CleanupClosePushL(file);
    User::LeaveIfError(file.Replace(fs, iNonNativeApplicationTypeRegistry, EFileShareExclusive|EFileStream|EFileWrite));
    RFileWriteStream targetStream;
    targetStream.Attach(file); // file gets closed by this call, but that's okay, we don't need it any more (targetStream has its own copy of this RFile object that it owns)
    CleanupClosePushL(targetStream);
    const TInt arrayCount(iNonNativeApplicationTypeArray.Count());
    TInt arrayCountToExternalize=arrayCount;
    if (aIndexToIgnore>=0)
        --arrayCountToExternalize;

    TCardinality(arrayCountToExternalize).ExternalizeL(targetStream);
    for (TInt i=0; i<arrayCount; ++i)
        {
        if (i!=aIndexToIgnore)
            {
            const SNonNativeApplicationType& nonNativeApplicationType=iNonNativeApplicationTypeArray[i];
            targetStream.WriteUint32L(nonNativeApplicationType.iTypeUid.iUid);
            targetStream << *nonNativeApplicationType.iNativeExecutable;
            }
        }
        
    targetStream.CommitL();
    CleanupStack::PopAndDestroy(2, &file);
    }
#endif

TPtrC CApaAppArcServer::NativeExecutableL(TUid aNonNativeApplicationType)
	{
	for (TInt i=iNonNativeApplicationTypeArray.Count()-1; i>=0; --i)
		{
		const SNonNativeApplicationType& nonNativeApplicationType=iNonNativeApplicationTypeArray[i];
		if (nonNativeApplicationType.iTypeUid.iUid==aNonNativeApplicationType.iUid)
			return *nonNativeApplicationType.iNativeExecutable;
		}

#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
	//If the mapping is not available in the list, search in SCR and add it to the list.
	TPtrC nativeExecutableName=FindAndAddNonNativeRuntimeMappingL(aNonNativeApplicationType);
	if(nativeExecutableName==KNullDesC())
	    User::Leave(KErrNotSupported); // not KErrNotFound

	return nativeExecutableName;
#else
	User::Leave(KErrNotSupported); // not KErrNotFound
	return KNullDesC();
#endif	
	}


#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
TPtrC CApaAppArcServer::FindAndAddNonNativeRuntimeMappingL(TUid aNonNativeApplicationType)
{
    //If non-native type to its runtime is not available search in SCR and update in list 
    Usif::RSoftwareComponentRegistry scrSession;
    User::LeaveIfError(scrSession.Connect());
    CleanupClosePushL(scrSession);
    
    RPointerArray<Usif::CLauncherExecutable> launchers;
    //Get non-native type to its run-time mappings 
    scrSession.GetApplicationLaunchersL(launchers);
    TCleanupItem cleanup(CleanupAndDestroyLauncherArray, &launchers); 
    CleanupStack::PushL(cleanup);     

    //Search for mapping and add it mapping list.
    for(TInt index=0;index<launchers.Count();index++)
        {
        Usif::CLauncherExecutable* launcherInfo=launchers[index];
        if(aNonNativeApplicationType.iUid==launcherInfo->TypeId())
            {
            SNonNativeApplicationType nonNativeApplicationType;
            nonNativeApplicationType.iTypeUid.iUid=launcherInfo->TypeId();
            nonNativeApplicationType.iNativeExecutable=launcherInfo->Launcher().AllocLC();
            iNonNativeApplicationTypeArray.AppendL(nonNativeApplicationType);
            CleanupStack::Pop(nonNativeApplicationType.iNativeExecutable);
            CleanupStack::PopAndDestroy(2, &scrSession);
            return *nonNativeApplicationType.iNativeExecutable;
            }
        }
    
    CleanupStack::PopAndDestroy(2, &scrSession);    
    return KNullDesC();
}
#endif

void CApaAppArcServer::DeleteLastNonNativeApplicationType(TAny* aThis)
	{ // static
	CApaAppArcServer& self=*static_cast<CApaAppArcServer*>(aThis);
	const TInt arrayIndex=self.iNonNativeApplicationTypeArray.Count()-1;
	delete self.iNonNativeApplicationTypeArray[arrayIndex].iNativeExecutable;
	self.iNonNativeApplicationTypeArray[arrayIndex].iNativeExecutable = NULL;
	self.iNonNativeApplicationTypeArray.Remove(arrayIndex);
	}

void CApaAppArcServer::NotifyScanComplete()
	{
	// Updates the applist with the icon caption details from the Central Repository.
	TRAP_IGNORE(iAppList->UpdateAppListByIconCaptionOverridesL());
	// The short caption value sets through the API has got the highest precedence over the
	// values found in either central repository or resource file.		
	TRAP_IGNORE(iAppList->UpdateAppListByShortCaptionL());

	// iterate through sessions
	iSessionIter.SetToFirst();
	CApaAppArcServSession* ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
#ifdef SYMBIAN_UNIVERSAL_INSTALL_FRAMEWORK
    while (ses)
        {
        ses->NotifyScanComplete();  
        ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
        }	
#else
	while (ses)
		{
		if((iForceRegistrationStatus & EForceRegistrationRequested) ||
	            !(iForceRegistrationStatus & EForceRegistrationAppListChanged))
		    {
		    //Notify clients about completion of force registration or scan completion		
		    ses->NotifyScanComplete();
		    }
		else
		    {
		    //If force registration changes applicaiton list,
		    // then notify applist change to clients.
		    ses->NotifyClients(MApaAppListServObserver::EAppListChanged);
		    }		
		ses=static_cast<CApaAppArcServSession*>(&(*iSessionIter++));
		}
	if(!(iForceRegistrationStatus & EForceRegistrationRequested))
	    {
	    //If this function is called not because of force registration, 
	    //clear force registration applist change status. 
            iForceRegistrationStatus &= (~EForceRegistrationAppListChanged);	        
	    }
	//Clear force registration request status
        iForceRegistrationStatus &= (~EForceRegistrationRequested);
#endif        
	}

/*
 * Data Recognizer calls
 */
 
TBool CApaAppArcServer::CachedRecognitionResult(const TParseBase& aParser, TDataRecognitionResult& aResult) const
	{
	if(iRecognitionCache && aParser.PathPresent() && aParser.NamePresent())
		return iRecognitionCache->Get(aParser.DriveAndPath(), aParser.NameAndExt(), aResult);

	return EFalse;
	}

/**
N.B. The @c CRecognitionResult object is reference counted so it must be closed!
*/
CRecognitionResult* CApaAppArcServer::CachedRecognitionResult(const RFile& aFile, const TParseBase& aParser) const
	{
	if(iRecognitionCache && aParser.PathPresent() && aParser.NamePresent())
		return iRecognitionCache->Get(aFile, aParser.DriveAndPath(), aParser.NameAndExt());

	return NULL;
	}

void CApaAppArcServer::CacheRecognitionResultL(const TParseBase& aParser, const TDataRecognitionResult& aResult)
	{
	if(iRecognitionCache && aParser.PathPresent() && aParser.NamePresent())
		iRecognitionCache->AddL(aParser.DriveAndPath(), aParser.NameAndExt(), aResult);
	}

void CApaAppArcServer::CacheRecognitionResultL(const RFile& aFile, const TParseBase& aParser, const TDataRecognitionResult& aResult)
	{
	if(iRecognitionCache && aParser.PathPresent() && aParser.NamePresent())
		{
		iRecognitionCache->AddL(aFile, aParser.DriveAndPath(), aParser.NameAndExt(), aResult);
		}
	}

TDataRecognitionResult CApaAppArcServer::RecognizeDataL(const TDesC& aName, const TDesC8& aBuffer)
	{
	TParsePtrC parser(iFs.IsValidName(aName) ? aName : KNullDesC);
	TDataRecognitionResult result;
	
	// check cache	
	if(!CachedRecognitionResult(parser, result))
		{
		// recognize
		if(iLoadRecognizersOnDemand)
			LoadRecognizersLC();

		result = iMimeTypeRecognizer->RecognizeL(aName, aBuffer);
		if(iLoadRecognizersOnDemand)
			CleanupStack::PopAndDestroy();

		// add to cache
		CacheRecognitionResultL(parser, result);
		}
	
	return result;	
	}

TDataRecognitionResult CApaAppArcServer::RecognizeDataL(RFile& aFile, TInt aPreferredBufSize)
	{
	CRecognitionResult* result = RecognizeDataAsCRecognitionResultL(aFile, aPreferredBufSize);
	TDataRecognitionResult ret;
	result->Get(ret);
	result->Close();
	return ret;
	}

/**
Same as @c RecognizeDataL(RFile&, TInt) but returns a @c CRecognitionResult 
instead of a @c TDataRecognitionResult.

N.B. The @c CRecognitionResult object is reference counted so it must be closed!
*/
CRecognitionResult* CApaAppArcServer::RecognizeDataAsCRecognitionResultL(RFile& aFile, TInt aPreferredBufSize)
	{
	CRecognitionResult* result = NULL;
	
	TFileName fileName;
	User::LeaveIfError(aFile.FullName(fileName));
	TParsePtrC parser(fileName); //fileName is valid since it comes from RFile

	//check cache
	result = CachedRecognitionResult(aFile,parser);
	if(!result)
		{
		// recognize
		if(iLoadRecognizersOnDemand)
			LoadRecognizersLC();

		const TDataRecognitionResult recResult = iMimeTypeRecognizer->RecognizeL(aFile, aPreferredBufSize);
		if(iLoadRecognizersOnDemand)
			CleanupStack::PopAndDestroy();
	
		//add to cache
		CacheRecognitionResultL(aFile, parser, recResult);

		result = CRecognitionResult::NewL(parser.NameAndExt(), recResult);
		}
	
	return result;
	}

TBool CApaAppArcServer::RecognizeDataL(const TDesC& aName, const TDesC8& aBuffer, const TDataType& aDataType)
	{
	if(iLoadRecognizersOnDemand)
		LoadRecognizersLC();

	const TBool ret = iMimeTypeRecognizer->RecognizeL(aName,aBuffer,aDataType);
	if(iLoadRecognizersOnDemand)
		CleanupStack::PopAndDestroy();

	return ret;
	}

TBool CApaAppArcServer::RecognizeDataL(RFile& aFile, TInt aPreferredBufSize, const TDataType& aDataType)
	{
	if(iLoadRecognizersOnDemand)
		LoadRecognizersLC();

	const TBool ret = iMimeTypeRecognizer->RecognizeL(aFile,aPreferredBufSize,aDataType);
	if(iLoadRecognizersOnDemand)
		CleanupStack::PopAndDestroy();

	return ret;
	}

TInt CApaAppArcServer::DataRecognizerPreferredBufSizeL()
	{
	if(iLoadRecognizersOnDemand)
		LoadRecognizersLC();

	const TInt ret = iMimeTypeRecognizer->PreferredBufSize();
	if(iLoadRecognizersOnDemand)
		CleanupStack::PopAndDestroy();

	return ret;
	}

void CApaAppArcServer::DataTypeL(CDataTypeArray& aArray)
	{
	if(iLoadRecognizersOnDemand)
		LoadRecognizersLC();

	iMimeTypeRecognizer->DataTypeL(aArray);
	if(iLoadRecognizersOnDemand)
		CleanupStack::PopAndDestroy();
	}

/*
 * Recognizer loading/unloading code
 */

void CApaAppArcServer::LoadRecognizersLC()
	{
	ASSERT(iLoadRecognizersOnDemand);

	LoadRecognizersL();
	TCleanupItem cleanup(CApaAppArcServer::RecognizerCleanup, this);
	CleanupStack::PushL(cleanup);
	}

void CApaAppArcServer::RecognizerCleanup(TAny* aSelf)
	{
	if (aSelf)
		{
		static_cast<CApaAppArcServer*>(aSelf)->UnloadRecognizers();
		}
	}

void CApaAppArcServer::LoadRecognizersL()
	{
	ASSERT(iLoadRecognizersOnDemand);

	if(iRecognizerUnloadTimer->IsActive())
		{
		__ASSERT_DEBUG(iRecognizerUsageCount==0,Panic(EReferenceCountingError1));
		iRecognizerUnloadTimer->Cancel();
		}
	else if(iRecognizerUsageCount==0)
		{
		iMimeTypeRecognizer->LoadRecognizersL();
		}

	++iRecognizerUsageCount;
	}

TInt CApaAppArcServer::UnloadRecognizers()
	{
	ASSERT(iLoadRecognizersOnDemand);

	--iRecognizerUsageCount;
	__ASSERT_DEBUG(iRecognizerUsageCount>=0,Panic(EReferenceCountingError2));
	if (iRecognizerUsageCount==0)
		{
		iRecognizerUnloadTimer->Start(KApaUnloadRecognizersTimeout,0,TCallBack(CApaAppArcServer::DoUnloadRecognizersCallback,this));
		}
	return KErrNone;
	}
	
TInt CApaAppArcServer::DoUnloadRecognizersCallback(TAny* aSelf)
	{
	TInt ret=KErrNone;
	if (aSelf)
		{
		ret = static_cast<CApaAppArcServer*>(aSelf)->DoUnloadRecognizers();
		}
	return ret;
	}

TInt CApaAppArcServer::DoUnloadRecognizers()
	{
	ASSERT(iLoadRecognizersOnDemand);

	// need to cancel the periodic timer since we only want a oneshot timer
	iRecognizerUnloadTimer->Cancel();
	iMimeTypeRecognizer->UnloadRecognizers();
	return KErrNone;
	}
	
void CApaAppArcServer::GetAppForMimeType(const TDataType& aDataType, TUid& aUid) const
	{
	iMimeTypeToAppMappingsManager->GetAppByDataType(aDataType, aUid);
	}
	
void CApaAppArcServer::GetAppForMimeType(const TDataType& aDataType, TUid aServiceUid, TUid& aUid) const
	{
	iMimeTypeToAppMappingsManager->GetAppByDataType(aDataType, aServiceUid, aUid);
	}
	
TBool CApaAppArcServer::InsertAndStoreIfHigherL(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid)
	{
	return iMimeTypeToAppMappingsManager->InsertAndStoreIfHigherL(aDataType, aPriority, aUid);
	}
	
void CApaAppArcServer::InsertAndStoreDataMappingL(const TDataType& aDataType, TDataTypePriority aPriority, TUid aUid, TUid aServiceUid)
	{
	iMimeTypeToAppMappingsManager->InsertAndStoreDataMappingL(aDataType, aPriority, aUid, aServiceUid);
	}
	
void CApaAppArcServer::DeleteAndStoreDataMappingL(const TDataType& aDataType, TUid aServiceUid)
	{
	iMimeTypeToAppMappingsManager->DeleteAndStoreDataMappingL(aDataType, aServiceUid);
	}

TBool CApaAppArcServer::LoadMbmIconsOnDemand() const
	{
	return iLoadMbmIconsOnDemand;
	}

#ifdef _DEBUG

/**
Flushes the recognition cache.

Useful for debugging.
*/
void CApaAppArcServer::FlushRecognitionCache()
	{
	if(iRecognitionCache)
		iRecognitionCache->Flush();
	}

/**
Sets whether or not recognizers should be loaded when they are needed.

Useful for debugging.
*/	
void CApaAppArcServer::SetLoadRecognizersOnDemandL(TBool aLoadRecognizersOnDemand)
	{
	if(iLoadRecognizersOnDemand == aLoadRecognizersOnDemand)
		return;
	
	CPeriodic* newUnloadTimer;
	if(aLoadRecognizersOnDemand)
		{
		ASSERT(!iRecognizerUnloadTimer);
		newUnloadTimer = CPeriodic::NewL(EPriorityNormal);
		}
	else
		{
		ASSERT(iRecognizerUnloadTimer);
		newUnloadTimer = NULL;
		}
	CleanupStack::PushL(newUnloadTimer);
	
	CApaScanningDataRecognizer* newMimeTypeRecognizer = CApaScanningDataRecognizer::NewL(iFs,!aLoadRecognizersOnDemand);
	delete iMimeTypeRecognizer;
	iMimeTypeRecognizer = newMimeTypeRecognizer;
	
	delete iRecognizerUnloadTimer;
	iRecognizerUnloadTimer = newUnloadTimer;
	CleanupStack::Pop(newUnloadTimer);
	
	iRecognizerUsageCount = 0;
	iLoadRecognizersOnDemand = aLoadRecognizersOnDemand;	
	}

/**
If recognizers are set to be loaded on demand this method can be used to perform
the unloading synchronously, instead of waiting for the unloading timer to go off.

Useful for debugging.
*/
void CApaAppArcServer::PerformOutstandingRecognizerUnloading()
	{
	if(iLoadRecognizersOnDemand && iRecognizerUnloadTimer->IsActive())
		{
		__ASSERT_DEBUG(iRecognizerUsageCount==0,Panic(EReferenceCountingError3));
		DoUnloadRecognizers();
		}
	}

#endif //_DEBUG

