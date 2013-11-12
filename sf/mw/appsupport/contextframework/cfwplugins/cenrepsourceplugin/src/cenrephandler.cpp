/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCenRepHandler class implementation.
*
*/


#include <centralrepository.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>

#include "cenrephandler.h"
#include "cenrepattributesmasked.h"
#include "cenrepattributesnonmasked.h"
#include "cenreptrace.h"
// #include "cenrepsourceplugintester.h"

// ======== LOCAL FUNCTIONS =========

// ======== MEMBER FUNCTIONS ========

CCenRepHandler* CCenRepHandler::NewL(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings,
    TInt aIndex)
    {
    FUNC_LOG;

    CCenRepHandler* self = new (ELeave) CCenRepHandler();
    CleanupStack::PushL( self );
    self->ConstructL( aCF, aSettings, aIndex);
    CleanupStack::Pop( self );
    return self;
    }

void CCenRepHandler::ConstructL( MCFContextInterface& aCF,
                                 CCFContextSourceSettingArray& aSettings,
                                 TInt aIndex )
    {
    FUNC_LOG;

    TUid repositoryUid = TUid::Null();
    TUint32 repositoryInt( 0 );

    const RKeyValueArray& attrArray = aSettings.Item( aIndex ).Attributes();
    TInt attrCount = attrArray.Count();

    // Find attribute values
    for( TInt i = 0; i < attrCount; i++ )
        {
        if ( attrArray[i]->Key().CompareF( KUID ) == KErrNone )
            {
            TInt err = CCenRepAttributes::ConvertToHex( attrArray[i]->Value(),
                                                        repositoryInt );
            User::LeaveIfError( err );
            repositoryUid = TUid::Uid( repositoryInt );
            }
        else if ( attrArray[i]->Key().CompareF( KMode ) == KErrNone )
            {
            if ( attrArray[i]->Value().CompareF( KMasked ) == KErrNone )
                {
                iCenRepAttributes = CCenRepAttributesMasked::NewL( aCF, aSettings );
                }
            else if ( attrArray[i]->Value().CompareF( KNonMasked ) == KErrNone )
                {
                iCenRepAttributes = CCenRepAttributesNonMasked::NewL( aCF, aSettings );
                }
            else
                {
                User::Leave( KErrNotSupported );
                }
            iKey = iCenRepAttributes->InitCenRepKeyL( aIndex );
            }
        }

    iCrep = CRepository::NewL( repositoryUid );

    QueryAndUpdateContext();

    // Test
    
    /*
    iTester = CCenRepSourcePluginTester::NewL( this );
    iTester->StartTimer();
    */

    TInt err = iCrep->NotifyRequest( iKey, iStatus );
    if( err == KErrNone )
        {
        SetActive();
        }
    }

CCenRepHandler::CCenRepHandler():
    CActive( CActive::EPriorityStandard )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

CCenRepHandler::~CCenRepHandler()
    {
    FUNC_LOG;

    Cancel();
    delete iCenRepAttributes;
    delete iCrep;
    // delete iTester;
    }

// ---------------------------------------------------------------------------
// CCenRepHandler::RunL
// ---------------------------------------------------------------------------
//
void CCenRepHandler::RunL()
    {
    FUNC_LOG;
    
    TInt err( KErrGeneral );

    // Test
    /*
    vvalue = 0x1;
    iCenRepAttributes->UpdateContext( vvalue );

    vvalue = 0xb;
    iCenRepAttributes->UpdateContext( vvalue );
    */
    // End of test
    
    TInt tmpStat = iStatus.Int();
    INFO_1( "CCenRepHandler::RunL: iStatus == %d", tmpStat );
    
    if ( tmpStat > 0 )
        {
        QueryAndUpdateContext();
        err = iCrep->NotifyRequest( iKey, iStatus );
        }
    else 
        {
        err = iCrep->NotifyRequest( iKey, iStatus );
        }
    
    INFO_1( "Error code of Central Repository NotifyRequest access: %d", err );
    
    if ( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }

    SetActive();
    }

// ---------------------------------------------------------------------------
// CCenRepHandler::RunError
// ---------------------------------------------------------------------------
//
TInt CCenRepHandler::RunError( TInt aError )
    {
    FUNC_LOG;
    
    INFO_1( "CCenRepHandler::RunError: aError == %d", aError );

    // Avoid Panic in CActiveScheduler
    aError = KErrNone;

    return aError;
    }

// ---------------------------------------------------------------------------
// CCenRepHandler::DoCancel
// ---------------------------------------------------------------------------
//
void CCenRepHandler::DoCancel()
    {
    FUNC_LOG;

    iCrep->NotifyCancel( iKey );
    }

// ---------------------------------------------------------------------------
// CCenRepHandler::QueryAndUpdateContext
// ---------------------------------------------------------------------------
//
void CCenRepHandler::QueryAndUpdateContext()
    {
    FUNC_LOG;

    TInt value( 0 );

    TInt err = iCrep->Get( iKey, value );
    
    INFO_1( "Error code of Central Repository get access: %d", err );

    if( err == KErrNone )
        {
        TRAPD( updateErr, iCenRepAttributes->UpdateContextL( value ) );

        INFO_1( "Leave code of CenRepAttributes->UpdateContextL: %d", updateErr );

        updateErr = updateErr; // Make compiler happy
        }
    }
