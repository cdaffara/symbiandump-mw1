/*
 * Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  COrientationAction class implementation.
 *
 */

#include <AknLayoutConfig.h>
#include <AknSgcc.h>
#include <aknconsts.h>
#include <UikonInternalPSKeys.h>
#include <UikonInternalPSKeys.h>
#include <apgtask.h> 
#include <AknCapServerDefs.h>

#include "orientationaction.h"
#include "trace.h"
#include "orientationactiondef.h"

// MEMBER FUNCTIONS

COrientationAction* COrientationAction::NewL()
    {
    FUNC_LOG;

    COrientationAction* self = COrientationAction::NewLC();
    CleanupStack::Pop( self );

    return self;
    }

COrientationAction* COrientationAction::NewLC()
    {
    FUNC_LOG;

    COrientationAction* self = new ( ELeave ) COrientationAction;
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// Destructor
COrientationAction::~COrientationAction()
    {
    FUNC_LOG;
    iWsSession.Close();
    }

COrientationAction::COrientationAction()
    {
    FUNC_LOG;

    // Nothing to do
    }

void COrientationAction::ConstructL()
    {
    FUNC_LOG;

    User::LeaveIfError( iWsSession.Connect() );
    }

// METHODS --------------------------------------------------------------------

//-----------------------------------------------------------------------------
// COrientationAction::Execute
//-----------------------------------------------------------------------------
//
TInt COrientationAction::Execute( const TDesC& aMode )
    {
    FUNC_LOG;
    TRAPD(err, RotateL( aMode ) );

    return err;
    }

//-----------------------------------------------------------------------------
// COrientationAction::RotateL
//-----------------------------------------------------------------------------
//
void COrientationAction::RotateL( const TDesC& aMode )
    {
    FUNC_LOG;
        INFO_1("aMode = %S", &aMode);
    TInt err = KErrNone;
    TBool isLandScape = EFalse;
    if( aMode.CompareF( KOrientationParamModeValNormal ) == KErrNone
        || aMode.CompareF( KOrientationParamModeValPortrait ) == KErrNone ) // portrait
        {
        INFO("Landscape = False");
        isLandScape = EFalse;
        }
    else if( aMode.CompareF( KOrientationParamModeValAlternate ) == KErrNone
            || aMode.CompareF( KOrientationParamModeValLandscape ) == KErrNone )
        {
        INFO("Landscape = True");
        isLandScape = ETrue;
        }
    else
        {
        INFO_1("unknown aMode = %S", &aMode);
        err = KErrUnknown;
        }
    User::LeaveIfError( err );

    CWsScreenDevice* screenDevice = NULL;
    screenDevice = new ( ELeave ) CWsScreenDevice( iWsSession );
    CleanupStack::PushL( screenDevice );
    User::LeaveIfError( screenDevice->Construct() );
    TSize currentScreenSize = screenDevice->SizeInPixels();

    TBool needsRotating = ETrue;
    if( currentScreenSize.iWidth > currentScreenSize.iHeight && isLandScape )
        {
        // we are already in landscape 
        needsRotating = EFalse;
        }
    if( currentScreenSize.iWidth < currentScreenSize.iHeight && ( !isLandScape ) )
        {
        // we are already in portrait 
        needsRotating = EFalse;
        }

    CAknLayoutConfig* layoutConfigPtr = CAknLayoutConfig::NewL();
    CleanupStack::PushL( layoutConfigPtr );

    CAknLayoutConfig& layoutConfig = *layoutConfigPtr;

    const CAknLayoutConfig::THardwareStateArray& hwStates =
        layoutConfig.HardwareStates();
    const CAknLayoutConfig::TScreenModeArray& screenModes =
        layoutConfig.ScreenModes();

    TInt newHwStateIndex = KErrNotFound;

    // lets select alternate state from current
    TSize newScreenSize;
    if( needsRotating )
        {
        newScreenSize = TSize( currentScreenSize.iHeight,
            currentScreenSize.iWidth );
        }
    else // basicly select current state again to ensure correct mode by sensors is informed to akncapserver
        {
        newScreenSize = TSize( currentScreenSize.iWidth,
            currentScreenSize.iHeight );
        }
    
    INFO_2(" new size should be = width %d height = %d", newScreenSize.iWidth, newScreenSize.iHeight );
    for( TInt i = 0; i < hwStates.Count(); i++ )
        {
        const CAknLayoutConfig::THardwareState hwState = hwStates.At( i );

        const CAknLayoutConfig::TScreenMode normal =
            screenModes.Find( hwState.ScreenMode() );

        if( normal.SizeInPixels() == newScreenSize )
            {
            INFO_1(" new HW state found = %d", i);
            newHwStateIndex = i;
            break;
            }
        }

    // print debug info about available screenmodes
#ifdef __UDEB__
    for(TInt i = 0; i<hwStates.Count(); i++)
        {
        const CAknLayoutConfig::THardwareState hwState = hwStates.At(i);

        const CAknLayoutConfig::TScreenMode normal = screenModes.Find(hwState.ScreenMode());

        const CAknLayoutConfig::TScreenMode alternate = screenModes.Find(hwState.AlternateScreenMode());

        TInt normalHeight = normal.SizeInPixels().iHeight;
        TInt normalWidth = normal.SizeInPixels().iWidth;

        TInt alternateHeight = alternate.SizeInPixels().iHeight;
        TInt alternateWidth = alternate.SizeInPixels().iWidth;

        TInt hwStateNumber = hwState.StateNumber();

        INFO_1("HW state = %d", hwStateNumber);
        INFO_2("normal: Height = %d width = %d", normalHeight, normalWidth );
        INFO_2("alternate: Height = %d width = %d", alternateHeight, alternateWidth );
        }
#endif

    if( newHwStateIndex >= 0 )
        {
        INFO("new HW state");
        const CAknLayoutConfig::THardwareState newHwState =
            hwStates.At( newHwStateIndex );

        TApaTaskList taskList( iWsSession );
        TApaTask aknCapsrvTask = taskList.FindApp( KAknCapServerUid );
        INFO_1("aknCapsrvTask.WgId(); = %d", aknCapsrvTask.WgId());

        aknCapsrvTask.SendKey( newHwState.KeyCode(), 0 );
        INFO("sent keycode to aknCapsrvTask");
        }
    CleanupStack::PopAndDestroy( layoutConfigPtr );
    CleanupStack::PopAndDestroy( screenDevice );
    }

// End of file
