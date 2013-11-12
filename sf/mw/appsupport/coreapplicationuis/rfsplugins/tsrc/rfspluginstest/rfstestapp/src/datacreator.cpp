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
* Description:  Implementation of CDataCreator class
*
*/


#include "datacreator.h"
#include "testdatacreatorapi.h"
#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CDataCreator::NewL
// ---------------------------------------------------------------------------
//
CDataCreator* CDataCreator::NewL(
    const TDesC& aSpecFile,
    const TDesC& aExcludeFile,
    const TDesC& aExcludeTargetFile,
    const TCallBack& aCallBack )
    {
    FUNC_LOG;

    CDataCreator* self = new ( ELeave ) CDataCreator( aCallBack );
    CleanupStack::PushL( self );
    self->ConstructL( aSpecFile, aExcludeFile, aExcludeTargetFile );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CDataCreator::~CDataCreator
// ---------------------------------------------------------------------------
//
CDataCreator::~CDataCreator()
    {
    FUNC_LOG;

    Cancel();
    delete iExcludeFileName;
    delete iExcludeTargetFileName;
    }


// ---------------------------------------------------------------------------
// CDataCreator::CDataCreator
// ---------------------------------------------------------------------------
//
CDataCreator::CDataCreator( const TCallBack& aCallBack )
  : CActive( EPriorityStandard ),
    iCallBack( aCallBack )
    {
    FUNC_LOG;

    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CDataCreator::ConstructL
// ---------------------------------------------------------------------------
//
void CDataCreator::ConstructL( const TDesC& aSpecFile, 
                               const TDesC& aExcludeFile,
                               const TDesC& aExcludeTargetFile )
    {
    FUNC_LOG;

    iExcludeFileName = aExcludeFile.AllocL();
    iExcludeTargetFileName = aExcludeTargetFile.AllocL();
    iState = ESetupData;

    User::LeaveIfError( iDataCreator.Create(
        _L( "testdatacreator.exe" ), KSetupFilesParam ) );
    User::LeaveIfError( iDataCreator.SetParameter( KFileNameIndex, aSpecFile ) );

    iDataCreator.Logon( iStatus );
    iDataCreator.Resume();
    SetActive();
    }


// ---------------------------------------------------------------------------
// CDataCreator::DoCancel
// ---------------------------------------------------------------------------
//
void CDataCreator::DoCancel()
    {
    FUNC_LOG;

    iDataCreator.LogonCancel( iStatus );
    iDataCreator.Kill( KErrAbort );
    }


// ---------------------------------------------------------------------------
// CDataCreator::RunL
// ---------------------------------------------------------------------------
//
void CDataCreator::RunL()
    {
    FUNC_LOG;
    ERROR( iStatus.Int(), "testdatacreator.exe failed" );

    if ( iStatus.Int() == KErrNone && iState == ESetupData )
        {
        iState = ESetupExcludeList;

        User::LeaveIfError( iDataCreator.Create(
            _L( "testdatacreator.exe" ), KSetupExcludeListParam ) );
        User::LeaveIfError(
            iDataCreator.SetParameter( KFileNameIndex, *iExcludeFileName ) );
        User::LeaveIfError(
            iDataCreator.SetParameter( KTargetFileNameIndex, *iExcludeTargetFileName ) );

        iDataCreator.Logon( iStatus );
        iDataCreator.Resume();
        SetActive();
        }
    else if ( iState == ESetupExcludeList )
        {
        // Finished, inform observer
        iCallBack.CallBack();
        }
    }
