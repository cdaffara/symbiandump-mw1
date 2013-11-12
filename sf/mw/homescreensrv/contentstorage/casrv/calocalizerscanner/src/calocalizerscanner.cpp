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
 * Description: calocalizerscanner.cpp
 *
 */

#include "calocalizerscanner.h"
#include "calocalizerscannerproxy.h"
#include "castorageproxy.h"
#include "casrvplugin.h"

// ---------------------------------------------------------------------------
// CCaLocalizerScanner::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaLocalizerScanner* CCaLocalizerScanner::NewL(
        TPluginParams* aPluginParams )
    {
    CCaLocalizerScanner* self = new ( ELeave ) CCaLocalizerScanner();
    CleanupStack::PushL( self );
    self->ConstructL( aPluginParams );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScanner::CCaLocalizerScanner
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CCaLocalizerScanner::CCaLocalizerScanner()
    {
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScanner::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CCaLocalizerScanner::ConstructL( TPluginParams* aPluginParams )
    {
    iLocalizerScanner = CCaLocalizerScannerProxy::NewL(
        aPluginParams->storageProxy );
    }

// ---------------------------------------------------------------------------
// CCaLocalizerScanner::~CCaLocalizerScanner
// Destructor.
// ---------------------------------------------------------------------------
//
CCaLocalizerScanner::~CCaLocalizerScanner()
    {
    delete iLocalizerScanner;
    }

