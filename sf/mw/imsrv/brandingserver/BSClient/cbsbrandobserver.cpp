/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  cbsbrandobserver.cpp
*
*/

//  INCLUDE FILES

#include "cbsbrandobserver.h"
#include <mbsbrandchangeobserver.h>
#include <mbsbackuprestorestateobserver.h>
#include "mbsbrandobserverregistrar.h"

CBSBrandObserver* CBSBrandObserver::NewL( MBSBrandChangeObserver* aObserver,
										  MBSBackupRestoreStateObserver* aBackupObserver,
									   	  MBSBrandObserverRegistrar* aRegistrar )
    {
	CBSBrandObserver* self = new ( ELeave ) CBSBrandObserver( aObserver, aBackupObserver, aRegistrar ) ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  //self
    return self;
    }


void CBSBrandObserver::ConstructL()
    {
    iContinueBackupStateObserving = ETrue ;
    iContinueDataUpdateObserving = ETrue ;
    
	CActiveScheduler::Add( this );
	iRegistrar->RegisterObserverToServerL( iStatus );
	SetActive();
    }


CBSBrandObserver::~CBSBrandObserver()
    {
    Cancel();
    }


CBSBrandObserver::CBSBrandObserver( MBSBrandChangeObserver* aObserver,
									MBSBackupRestoreStateObserver* aBackupObserver,
									MBSBrandObserverRegistrar* aRegistrar )
: CActive( CActive::EPriorityIdle ), iObserver( aObserver ), iBackupObserver( aBackupObserver ),
									 iRegistrar( aRegistrar )
	{
	}

void CBSBrandObserver::RunL()
	{
	
	// Pankaj - chk for what has changed..
	TInt isBackupRestore = iRegistrar->GetBackupRestoreL() ;
	if(isBackupRestore)
		{
		// if client donot want to listen backup state event any more donot call its observer..
		if(iBackupObserver && iContinueBackupStateObserving)
			{
			TInt activeState = iRegistrar->GetBackupStateL() ;
			iContinueBackupStateObserving = iBackupObserver->BackupRestoreActivated(activeState) ;
			}
		}
	else
		{
		// if client donot want to listen update data event any more donot call its observer..
		if(iObserver && iContinueDataUpdateObserving)
			{
			TInt newVersion = iRegistrar->GetNewVersionL();
			iContinueDataUpdateObserving = iObserver->BrandUpdateAvailable( newVersion ) ;
			}
		}
		
	if(iContinueBackupStateObserving || iContinueDataUpdateObserving)
		{
		iRegistrar->RegisterObserverToServerL( iStatus );
		SetActive();
		}
	}
	
void CBSBrandObserver::DoCancel()
	{
	
	}


//  END OF FILE
