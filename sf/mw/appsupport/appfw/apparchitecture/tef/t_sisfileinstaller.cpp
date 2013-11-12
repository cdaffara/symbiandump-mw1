// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// The following test case is used to test if apparctestserver 
// can return app data for a specific app by caching that data when requested.
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#include <apgcli.h>
#include "T_SisFileInstaller.h"
#include <usif/scr/screntries.h>
#include <usif/usiferror.h>

using namespace Usif;

CSisFileInstaller::CSisFileInstaller()
    {   
    }

CSisFileInstaller::~CSisFileInstaller()
    {
    iSif.Close();
    }

TInt CSisFileInstaller::InstallSisAndWaitForAppListUpdateL(const TDesC& aFileName)
{
 RApaLsSession ls;
 User::LeaveIfError(ls.Connect());
 CleanupClosePushL(ls);
 TRequestStatus status;
 ls.SetNotify(EFalse, status);
 InstallSisL(aFileName);
 User::WaitForRequest(status);
 CleanupStack::PopAndDestroy();
 return KErrNone;
}

TInt CSisFileInstaller::UninstallSisAndWaitForAppListUpdateL(const TDesC& aComponentName)
{
 RApaLsSession ls;
 User::LeaveIfError(ls.Connect());
 CleanupClosePushL(ls);
 TRequestStatus status;
 ls.SetNotify(EFalse, status);
 UninstallSisL(aComponentName);
 User::WaitForRequest(status);
 CleanupStack::PopAndDestroy();
 return KErrNone;
}

TInt CSisFileInstaller::InstallSisL(const TDesC& aFileName)
    {
    TBuf<256> buf;
    buf.Copy(aFileName);
    HBufC* as = buf.AllocL();        
    TPtr16 sisFileName = as->Des();
    CleanupStack::PushL(as);
    
    RFs fs;
    RFile file;
    User::LeaveIfError(fs.Connect());
    fs.ShareProtected();
    CleanupClosePushL(fs);
    User::LeaveIfError(file.Open(fs, sisFileName, EFileRead | EFileShareReadersOnly));
    CleanupClosePushL(file);
    User::LeaveIfError(iSif.Connect()); 
    CleanupClosePushL(iSif);  
    TInt err=KErrNone;
     do
         {
         iSif.Install(aFileName, iStatus, ETrue);
         User::WaitForRequest(iStatus);
         err=iStatus.Int();
         }
     while( err == KErrScrReadOperationInProgress);
     
    User::LeaveIfError(err);
     
    CleanupStack::PopAndDestroy(4, as);
    return KErrNone;
    }


TInt CSisFileInstaller::UninstallSisL(const TDesC& aComponentName)
    {    
    _LIT(KSisComponentVendor, "Nokia India Pvt Ltd");   
    
    RFs fs;
    RFile file;
    User::LeaveIfError(fs.Connect());
    fs.ShareProtected();
    CleanupClosePushL(fs);     

    User::LeaveIfError(iSif.Connect());
    CleanupClosePushL(iSif);
    
    TBuf<256> buf;
    buf.Copy(aComponentName);
    HBufC* as1 = buf.AllocL();        
    TPtr16 componentName = as1->Des();
    HBufC* as2 = KSisComponentVendor().AllocL();
    TPtr16 componentVendor = as2->Des();
    iComponentId = FindComponentInScrL(componentName, componentVendor); 

    TInt err=KErrNone;
     do
         {
         iSif.Uninstall(iComponentId, iStatus, ETrue);
         User::WaitForRequest(iStatus);
         err=iStatus.Int();
         }
     while( err == KErrScrReadOperationInProgress);
     
    //Leave if sis file uninstllation failed.
    User::LeaveIfError(err);
    
    delete as1;
    delete as2;
    as1 = NULL;
    as2 = NULL;  
    CleanupStack::PopAndDestroy(2, &fs); 
    return KErrNone;
    }


void CSisFileInstaller::CancelInstallation()
    {
    iSif.CancelOperation();
    }

void CSisFileInstaller::Close()
    {
    iSif.Close();
    }

TInt CSisFileInstaller::InstallSisAsynchronouslyL(const TDesC& aFileName, TRequestStatus& status)
{
    TBuf<256> buf;
    buf.Copy(aFileName);
    HBufC* as = buf.AllocL();        
    TPtr16 sisFileName = as->Des();
    CleanupStack::PushL(as);
    
    RFs fs;
    RFile file;
    User::LeaveIfError(fs.Connect());
    fs.ShareProtected();
    CleanupClosePushL(fs);
    User::LeaveIfError(file.Open(fs, sisFileName, EFileRead | EFileShareReadersOnly));
    CleanupClosePushL(file);
    CleanupStack::PopAndDestroy(3, as);
    
    User::LeaveIfError(iSif.Connect()); 
    
    iSif.Install(aFileName, status, ETrue);
    return KErrNone;
}

TInt CSisFileInstaller::FindComponentInScrL(const TDesC& aComponentName, const TDesC& aVendor)
    {
    RSoftwareComponentRegistry scr;
    User::LeaveIfError(scr.Connect());
    CleanupClosePushL(scr);

    RSoftwareComponentRegistryView scrView;
    CComponentFilter* filter = CComponentFilter::NewLC();
    filter->SetNameL(aComponentName);
    filter->SetVendorL(aVendor);

    scrView.OpenViewL(scr, filter);
    CleanupClosePushL(scrView);

    CComponentEntry* component = scrView.NextComponentL();
    TInt componentId = 0;
    if (component != NULL)
        {
        componentId = component->ComponentId();
        delete component;
        }

    CleanupStack::PopAndDestroy(3, &scr);    
    return componentId; 
    }

