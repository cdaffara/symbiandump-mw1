/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: caappscannerhandler.cpp
*
*/

#include "caappscannerhandler.h"

// ============================ MEMBER FUNCTIONS ===============================

CCaAppScannerHandler* CCaAppScannerHandler::NewL(TPluginParams* aPluginParams)
    {
    CCaAppScannerHandler* self = new (ELeave) CCaAppScannerHandler(
            aPluginParams);
    CleanupStack::PushL(self);
    self->ConstructL(*aPluginParams->storageProxy,
	    *aPluginParams->softwareRegistry, *aPluginParams->engUtils);
    CleanupStack::Pop();
    return self;
    }

CCaAppScannerHandler::~CCaAppScannerHandler()
    {
    delete iScanner;
    }

CCaAppScannerHandler::CCaAppScannerHandler(TPluginParams* /*aPluginParams*/)
    {
    }

void CCaAppScannerHandler::ConstructL(CCaStorageProxy& aCaStorageProxy,
        Usif::RSoftwareComponentRegistry& aSoftwareRegistry,
        CCaSrvEngUtils& aUtils)
    {
    iScanner = CCaSrvAppScanner::NewL(aCaStorageProxy, aSoftwareRegistry,
	    aUtils);
    }

//  End of File
