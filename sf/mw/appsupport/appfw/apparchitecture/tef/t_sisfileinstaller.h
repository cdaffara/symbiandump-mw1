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
//

/**
 @file
 @internalComponent - Internal Symbian test code
*/

#ifndef     __T_SISFILEINSTALLER_H
#define     __T_SISFILEINSTALLER_H

#include "appfwk_test_appui.h"
#include "apparctestserver.h"
#include "testableapalssession.h"

#include <usif/sif/sif.h>
#include <usif/scr/screntries.h>
#include <usif/sif/siftransportclient.h>
#include <usif/sif/sifcommon.h>
#include <scs/scsclient.h>
#include <usif/usifcommon.h>
#include <usif/scr/scr.h>
#include <e32std.h>
#include <scs/scscommon.h>
#include <scs/scsserver.h>
#include <e32const.h>

//! CTestAppSisFile
/*! 
  This class is used for install and uninstall of sis files
*/
class CSisFileInstaller: public CBase
    {
public:  
    CSisFileInstaller();
    ~CSisFileInstaller();

    TInt InstallSisL(const TDesC& aFileName);
    TInt InstallSisAndWaitForAppListUpdateL(const TDesC& aFileName);
    TInt UninstallSisL(const TDesC& aComponentName);
    TInt UninstallSisAndWaitForAppListUpdateL(const TDesC& aComponentName);    
    TInt FindComponentInScrL(const TDesC& aComponentName, const TDesC& aVendor);
    void CancelInstallation();
    TInt InstallSisAsynchronouslyL(const TDesC& aFileName, TRequestStatus& status);
    void Close();
private:
    friend class CSifOperationStep;
    Usif::RSoftwareInstall iSif;
    Usif::TComponentId iComponentId;
    TRequestStatus iStatus;
    };

#endif
