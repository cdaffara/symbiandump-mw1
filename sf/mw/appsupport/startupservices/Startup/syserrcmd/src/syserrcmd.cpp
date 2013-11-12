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
* Implementation of CSysErrCmd class.
*
*/

#include "syserrcmd.h"
#include "trace.h"


#include <data_caging_path_literals.hrh>
#include <featmgr.h>
#include <SecondaryDisplay/SecondaryDisplayStartupAPI.h>
#include <StringLoader.h>
#include <startup.rsg>
#include <stringresourcereader.h>
#include <hb/hbwidgets/hbdevicemessageboxsymbian.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSysErrCmd::NewL
// ---------------------------------------------------------------------------
//
CSysErrCmd* CSysErrCmd::NewL()
	{
    FUNC_LOG;
	return new ( ELeave ) CSysErrCmd();
	}


// ---------------------------------------------------------------------------
// CSysErrCmd::~CSysErrCmd
// ---------------------------------------------------------------------------
//
CSysErrCmd::~CSysErrCmd()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Initialize
// ---------------------------------------------------------------------------
//
TInt CSysErrCmd::Initialize( CSsmCustomCommandEnv* aCmdEnv )
    {
    FUNC_LOG;
    ASSERT_TRACE( aCmdEnv );
    iEnv = aCmdEnv;
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Execute
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Execute( const TDesC8& /*aParams*/, TRequestStatus& aRequest )
    {
    FUNC_LOG;
    
    INFO_1( "[0x%08x] Execute", this );
    TInt err( KErrNone );
    TRAP( err, DoExecuteL( aRequest ) );
    ERROR( err, "DoExecuteL failed." );
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::ExecuteCancel
// ---------------------------------------------------------------------------
//
void CSysErrCmd::ExecuteCancel()
    {
    FUNC_LOG;

    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Close
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Close()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSysErrCmd::Release
// ---------------------------------------------------------------------------
//
void CSysErrCmd::Release()
    {
    FUNC_LOG;

	delete this;
    }

// ---------------------------------------------------------------------------
// CSysErrCmd::DoExecuteL
// ---------------------------------------------------------------------------
//
void CSysErrCmd::DoExecuteL( TRequestStatus& aRequest )
    {
    aRequest = NULL;
	//Hb device message box implementation for global permanent note goes here
	CHbDeviceMessageBoxSymbian *aMessageBox = NULL;
    aMessageBox = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EWarning);
    _LIT(KText, "Self-test failed. Contact retailer.");
    aMessageBox->SetTextL(KText);
    aMessageBox -> SetDismissPolicy(0);
	aMessageBox -> SetTimeout(0);
    aMessageBox->ExecL();
	delete aMessageBox;
    }
