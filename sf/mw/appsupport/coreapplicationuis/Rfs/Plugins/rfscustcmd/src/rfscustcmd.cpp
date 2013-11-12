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
* Description:
* Implementation of CRfsCustCmd class.
*
*/

#include "rfscustcmd.h"
#include "trace.h"

// Rfs script for first boot
_LIT( KInitScript, "Z:\\private\\100059C9\\ScriptInit.txt" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CRfsCustCmd::NewL
// ---------------------------------------------------------------------------
//
CRfsCustCmd* CRfsCustCmd::NewL( TRfsCustCmdType aRfsType )
	{
    FUNC_LOG;
    
	return new ( ELeave ) CRfsCustCmd( aRfsType );
	}


// ---------------------------------------------------------------------------
// CRfsCustCmd::~CRfsCustCmd
// ---------------------------------------------------------------------------
//
CRfsCustCmd::~CRfsCustCmd()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::CRfsCustCmd
// ---------------------------------------------------------------------------
//
CRfsCustCmd::CRfsCustCmd( TRfsCustCmdType aRfsType )
    : iRfsType( aRfsType )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::Initialize
// ---------------------------------------------------------------------------
//
TInt CRfsCustCmd::Initialize( CSsmCustomCommandEnv* /*aCmdEnv*/ )
    {
    FUNC_LOG;
    TInt err( KErrNone );

    err = iRfs.Connect();
    ERROR( err, "Failed to connect to RRfsClient" );
    
    if( err == KErrNone &&
        iRfsType == ERfsCustCmdDeep || iRfsType == ERfsCustCmdPreUi )
        {
        err = iAdaptation.Connect();
        ERROR( err, "Failed to connect to RSsmStateAdaptation" );
        }
    
    return err;
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::Execute
// ---------------------------------------------------------------------------
//
void CRfsCustCmd::Execute(
    const TDesC8& /*aParams*/,
    TRequestStatus& aRequest )
    {
    FUNC_LOG;

    aRequest = KRequestPending;
    TRequestStatus* status = &aRequest;
    TInt err( KErrNone );
    
    if( iRfsType == ERfsCustCmdFirstBoot )
        {
        err = iRfs.RunScriptInFile( KInitScript );
        User::RequestComplete( status, err );
        }
    else if( iRfsType == ERfsCustCmdDeep )
        {
        err = iRfs.PerformRfs( ERfsDeep );
        if( err == KErrNone )
            {
            iAdaptation.RequestCoopSysPerformRfsActions( ESsmDeepRfs, *status );
            }
        else
            {
            User::RequestComplete( status, err );
            }
        }
    else if( iRfsType == ERfsCustCmdPreUi )
        {
        err = iRfs.PerformRfs( ERfsNormal );
        if( err == KErrNone )
            {
            iAdaptation.RequestCoopSysPerformRfsActions( ESsmShallowRfs, *status );
            }
        else
            {
            User::RequestComplete( status, err );
            }
        }
    else if( iRfsType == ERfsCustCmdPostUi )
        {
        err = iRfs.PerformPostEikonRfs( ERfsNormal );
        User::RequestComplete( status, err );
        }
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::ExecuteCancel
// ---------------------------------------------------------------------------
//
void CRfsCustCmd::ExecuteCancel()
    {
    FUNC_LOG;
    
    if( iRfsType == ERfsCustCmdDeep || iRfsType == ERfsCustCmdPreUi )
        {
        iAdaptation.RequestCancel();
        }
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::Close
// ---------------------------------------------------------------------------
//
void CRfsCustCmd::Close()
    {
    FUNC_LOG;
    
    iRfs.Close();
    if( iRfsType == ERfsCustCmdDeep || iRfsType == ERfsCustCmdPreUi )
        {
        iAdaptation.Close();
        }
    }


// ---------------------------------------------------------------------------
// CRfsCustCmd::Release
// ---------------------------------------------------------------------------
//
void CRfsCustCmd::Release()
    {
    FUNC_LOG;

	delete this;
    }
