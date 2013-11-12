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
 * Source file containing common test app functionality.
 * 
 */

#include <f32file.h>
#include <remconcoreapitarget.h>
#include <remconinterfaceselector.h>

#include "testappbase.h"

const TInt KLeftSoftKeyCode  = EKeyDevice0;
const TInt KRightSoftKeyCode = EKeyDevice1;

const TInt KHelpWindowBorderPixels = 20;
const TInt KHelpWindowSpaceBetweenColumns = 25;
const TInt KHelpWindowSpaceBetweenRows = 2;

struct TKeyListEntry
    {
    TInt         keyCode;
    const TText* keyName;
    };

const TKeyListEntry KSupportedKeys[KSupportedKeysCount] =
        {
            {EKeyEnter,      STR("Enter")},      
            {EKeyUpArrow,    STR("Up")},
            {EKeyDownArrow,  STR("Down")}, 
            {EKeyLeftArrow,  STR("Left")},
            {EKeyRightArrow, STR("Right")},
            {'0',            STR("0")},
            {'1',            STR("1")},  
            {'2',            STR("2")}, 
            {'3',            STR("3")},
            {'4',            STR("4")},
            {'5',            STR("5")},
            {'6',            STR("6")},
            {'7',            STR("7")},
            {'8',            STR("8")},
            {'9',            STR("9")},
            {'*',            STR("*")},
            {'#',            STR("#")}
        };
        
static TPtrC KeyName( TInt aIndex )
    {
    return TPtrC(KSupportedKeys[aIndex].keyName);
    }

// Portable app implementation

CTestAppBase::CTestAppBase( TInt aFontSize ) : 
    CActive( EPriorityStandard ),
    iFontSize( aFontSize ),
    iHelpSemitransparentBackgroundActive( true )
    {    
    CActiveScheduler::Add(this);
    }

CTestAppBase::~CTestAppBase()
    {
    delete iInterfaceSelector;
    delete iHelpWindow;
    delete iSelectionWindow;
    delete iGc;
    delete iWindowGroup;
    delete iScreenDevice;
    delete iTypefaceStore;
    iWs.Close();
    iFs.Close();
    iFileHistory.ResetAndDestroy();
    }

void CTestAppBase::BaseConstructL( const TOperationsPage* aKeyMap, TInt aPageCount )
    {
    iKeyMap = aKeyMap;
    iPageCount = aPageCount;
    
    User::LeaveIfError( iFs.Connect() );        
    
    User::LeaveIfError( iWs.Connect() );    

    iScreenDevice = new(ELeave) CWsScreenDevice( iWs );
    User::LeaveIfError( iScreenDevice->Construct() );
    iDisplaySize = iScreenDevice->SizeInPixels();
    
    User::LeaveIfError( iScreenDevice->CreateContext(iGc) );

    iWindowGroup = new(ELeave) RWindowGroup( iWs );
    User::LeaveIfError( iWindowGroup->Construct( KNullWsHandle ) );

    iSelectionWindow = new(ELeave) RWindow( iWs );
    User::LeaveIfError( iSelectionWindow->Construct( *iWindowGroup, KNullWsHandle ) );    
    iSelectionWindow->SetVisible(false);
    iSelectionWindow->Activate();    
            
    // Load the font to be used for all text operations.
    TFontSpec fontSpec;
    fontSpec.iHeight = iFontSize;
    iTypefaceStore = CFbsTypefaceStore::NewL(NULL);
    
    User::LeaveIfError( iTypefaceStore->GetNearestFontToDesignHeightInPixels(iFont, fontSpec) );

    CalculateHelpWindowSize();

    iHelpWindowTopRight = TPoint(iDisplaySize.iWidth/2 - iHelpWindowSize.iWidth/2, 
                                 iDisplaySize.iHeight/2 - iHelpWindowSize.iHeight/2);

    iHelpWindow = new(ELeave) RWindow( iWs );
    User::LeaveIfError( iHelpWindow->Construct( *iWindowGroup, KNullWsHandle ) );
    iHelpWindow->SetExtent( iHelpWindowTopRight, iHelpWindowSize );
    iHelpWindow->SetTransparencyAlphaChannel();
    iHelpWindow->SetBackgroundColor(KRgbTransparent);
    iHelpWindow->SetVisible(false);
    iHelpWindow->Activate();
    iHelpWindow->PointerFilter(EPointerFilterDrag, 0);

    // Initialize soft key indices
    for( TInt i = 0; i < aPageCount; i++ )
        {
        iSoftkeyIndices[i] = iKeyMap[i].defaultSoftkeyIndex;
        }
    
    // Only need to draw the help text when the options page is changed.  Window is displayed later by
    // toggling the visibility of the window.
    DrawHelpText();
    
    // THE FOLLOWING CODE IS COMMENTED OUT BECAUSE IT CAUSES A CRASH IN NCP BUILDS.
    // THIS CAN BE ENABLED IN DFS BUILDS, TO ALLOW FOR TWO BUTTON OPERATION USING THE VOLUME KEYS.
    //
    // Since some phone have no keyboard or soft keys, treat the volume keys like the soft keys.
    // SetupVolumeKeysL();    
    }

void CTestAppBase::SetupVolumeKeysL()
    {
    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
    iInterfaceSelector->OpenTargetL();
    }

void CTestAppBase::StartMonitoringWindowEvents()
    {
    // Request notification for windows server events, to detect key presses.    
    SetActive();
    iWs.EventReady( &iStatus );
    }

TInt CTestAppBase::CurrentPageNumber()
    {
    return iCurrentPage+1;
    }

TPtrC CTestAppBase::CurrentPageName()
    {
    return TPtrC(iKeyMap[iCurrentPage].pageName);
    }

TPtrC CTestAppBase::CurrentSoftkeyName()
    {
    return TPtrC(iKeyMap[iCurrentPage].mapping[iSoftkeyIndices[iCurrentPage]].text);    
    }
    
TPtrC CTestAppBase::KeyMapText( TInt aIndex, TInt aPage )
    {
    return TPtrC(iKeyMap[aPage].mapping[aIndex].text);
    }

TInt CTestAppBase::KeyMapOperation(TInt aIndex, TInt aPage )
    {
    return iKeyMap[aPage].mapping[aIndex].operation;
    }

void CTestAppBase::IncrementKeymapIndex( TInt& aIndex, TInt aPage )
    {
    aIndex = (aIndex + 1) % KSupportedKeysCount;
    while( iKeyMap[aPage].mapping[aIndex].operation == KOperation_None )
        {
        aIndex = (aIndex + 1) % KSupportedKeysCount;
        }
    }

void CTestAppBase::DecrementKeymapIndex( TInt& aIndex, TInt aPage )
    {
    aIndex = (aIndex + KSupportedKeysCount - 1) % KSupportedKeysCount;
    while( iKeyMap[aPage].mapping[aIndex].operation == KOperation_None )
        {
        aIndex = (aIndex + KSupportedKeysCount - 1) % KSupportedKeysCount;
        }
    }

void CTestAppBase::CalculateHelpWindowSize()
    {
    iHelpWindowColumn1Width = 0;
    iHelpWindowColumn2Width = 0;
    
    // Find the widest strings for each column to determine the width of the window.
    for( TInt index = 0; index < KSupportedKeysCount; index++ )
        {
        TInt width = iFont->TextWidthInPixels(KeyName(index));
        if( width > iHelpWindowColumn1Width )
            {
            iHelpWindowColumn1Width = width;
            }
        
        for( TInt index2 = 0; index2 < iPageCount; index2++ )
            {
            width = iFont->TextWidthInPixels(KeyMapText(index,index2));
            if( width > iHelpWindowColumn2Width )
                {
                iHelpWindowColumn2Width = width;
                }                    
            }
        }    
    
    iHelpWindowSize.iWidth = 2*KHelpWindowBorderPixels + iHelpWindowColumn1Width + 
                             KHelpWindowSpaceBetweenColumns + iHelpWindowColumn2Width;
    
    iHelpWindowSize.iHeight = 2*KHelpWindowBorderPixels + iFontSize*KSupportedKeysCount + 
                              KHelpWindowSpaceBetweenRows*(KSupportedKeysCount-1);
    }

void CTestAppBase::StartReceivingPointerEvents()
    {
    iRoutePointerEventsToApp = true;
    }

void CTestAppBase::StopReceivingPointerEvents()
    {
    iRoutePointerEventsToApp = false;
    }

//virtual void HandlePointerEvent( const TPointerEvent& aEvent ) {}

CTestAppBase::TTestAppPointerEvent CTestAppBase::CharacterizePointerEvent( const TAdvancedPointerEvent& event )
    {
    TTestAppPointerEvent returnValue = EPointerEvent_None;
    
    RDebug::Printf( "POINTER EVENT:" );        
    RDebug::Printf( "iType=%i", event.iType );
    RDebug::Printf( "iModifiers=%x", event.iModifiers );
    RDebug::Printf( "iPosition=%i,%i", event.iPosition.iX, event.iPosition.iY );
    RDebug::Printf( "iParentPosition=%i,%i", event.iParentPosition.iX, event.iParentPosition.iY );
    RDebug::Printf( "PointerNumber=%i", event.PointerNumber() );
    RDebug::Printf( "Proximity=%i", event.Proximity() );
    RDebug::Printf( "Pressure=%i", event.Pressure() );
    RDebug::Printf( "ProximityAndPressure=%i", event.ProximityAndPressure() );
    RDebug::Printf( "Position3D=%i,%i,%i", event.Position3D().iX, event.Position3D().iY, event.Position3D().iZ );
    RDebug::Printf( "Pressure3D=%i,%i,%i", event.Pressure3D().iX, event.Pressure3D().iY, event.Pressure3D().iZ );
    RDebug::Printf( "PositionAndPressure3D=%i,%i,%i", event.PositionAndPressure3D().iX, event.PositionAndPressure3D().iY, event.PositionAndPressure3D().iZ );
    
    switch( event.iType )
        {
        case TPointerEvent::EButton1Down:
            {
            iPointerDownPosition = event.iPosition;            
            break;
            }
        case TPointerEvent::EButton1Up:
            {
            TInt xDelta = event.iPosition.iX - iPointerDownPosition.iX; 
            TInt yDelta = event.iPosition.iY - iPointerDownPosition.iY;
            
            TInt xMagnitude = xDelta;
            if( xMagnitude < 0 )
                {
                xMagnitude = -xMagnitude;
                }
            
            TInt yMagnitude = yDelta;
            if( yMagnitude < 0 )
                {
                yMagnitude = -yMagnitude;
                }

            const TInt KTapThreshold = 30;
            
            if( yMagnitude > xMagnitude )
                {
                if( yMagnitude < KTapThreshold )
                    {
                    RDebug::Printf( "POINTER EVENT ENTER x=%i y=%i", xDelta, yDelta );                            
                    returnValue = EPointerEvent_Select;
                    }
                else if( yDelta < 0 )
                    {
                    RDebug::Printf( "POINTER EVENT UP x=%i y=%i", xDelta, yDelta );        
                    returnValue = EPointerEvent_Up;
                    }
                else
                    {
                    RDebug::Printf( "POINTER EVENT DOWN x=%i y=%i", xDelta, yDelta );                            
                    returnValue = EPointerEvent_Down;
                    }                
                }
            else
                {
                if( xMagnitude < KTapThreshold )
                    {
                    RDebug::Printf( "POINTER EVENT ENTER x=%i y=%i", xDelta, yDelta );                            
                    returnValue = EPointerEvent_Select;
                    }
                else if( xDelta < 0 )
                    {
                    RDebug::Printf( "POINTER EVENT LEFT x=%i y=%i", xDelta, yDelta );        
                    returnValue = EPointerEvent_Left;
                    }
                else
                    {
                    RDebug::Printf( "POINTER EVENT RIGHT x=%i y=%i", xDelta, yDelta );                            
                    returnValue = EPointerEvent_Right;
                    }                                
                }
            break;
            }
        }
    
    return returnValue;
    }

void CTestAppBase::RunL()
    {
    if( iWait.IsStarted() )
        {
        // This is an event during synchronous list selection.  Stop the nested scheduler.
        iWait.AsyncStop();
        return;
        }    
    
    TWsEvent event;
    iWs.GetEvent(event);

    TInt operationIndex = -1;
    
    TInt keyCode = 0;
    bool processKeyCode = false;
    
    TInt operation = KOperation_None; 
    
    // Other potentially useful events are EEventKeyUp and EEventKeyDown.
    
    if( event.Type() == EEventKey )
        {
        keyCode = event.Key()->iCode;
                   
        RDebug::Printf( "key event keyCode=%x/%c scanCode=%x/%c", keyCode, keyCode, event.Key()->iScanCode, event.Key()->iScanCode );
        
        // Allow subclasses a chance to consume the key event directly.  If that happens, then
        // do not handle the key as normal.
        if( !ConsumeKeyEvent( keyCode ) )
            {
            processKeyCode = true;        
            }
        }
    else if( event.Type() == EEventPointer )
        {
        TAdvancedPointerEvent* p = event.Pointer();

        if( iRoutePointerEventsToApp  )
            {
            HandlePointerEvent( *p );
            }
        else
            {        
            TTestAppPointerEvent pointerEvent = CharacterizePointerEvent( *p );

            switch( pointerEvent )
                {
                case EPointerEvent_None:
                    // Do nothing.
                    break;
                case EPointerEvent_Up:
                    operation = KOperation_PreviousOption;
                    break;
                case EPointerEvent_Down:
                    operation = KOperation_NextOption;
                    break;
                case EPointerEvent_Left:
                    operation = KOperation_PreviousOptionPage;
                    break;
                case EPointerEvent_Right:
                    operation = KOperation_NextOptionPage;
                    break;
                case EPointerEvent_Select:
                    operation = KOperation_ExecuteOption;
                    break;
                }
            }
        }
    else
        {
        RDebug::Printf( "other event, type=%i", event.Type() );    
        }
        
    if( processKeyCode )
        {
        // If one of the softkeys were pressed then take the appropriate action.
        // This is to support a two button touch device with no numeric keypad.
        // Support 'A' and 'B' also, for the NCP emulator where a keyboard  is
        // not displayed.
        switch( keyCode )
            {
            case KLeftSoftKeyCode:
            case 'a':
            case 'A':
                {
                operation = KOperation_NextOption;
                break;
                } 
            case KRightSoftKeyCode:
            case 'b':
            case 'B':
                {
                // Execute softkey function.
                operation = KOperation_ExecuteOption;
                break;
                }
            default:
                {
                // Search for keycode in keymap.  If not found then the key was not a valid
                // key, so ignore.                
                TInt index = 0;
                while( (index < KSupportedKeysCount) && (operationIndex == -1))
                    {
                    if( KSupportedKeys[index].keyCode == keyCode )
                        {                
                        // Found!
                        operationIndex = index;
                        }
                    else
                        {
                        index++;
                        }
                    }
                break;
                }                
            }        
        }
    
    if( operation == KOperation_ExecuteOption )
        {
        operationIndex = iSoftkeyIndices[iCurrentPage];
        }
    
    if( operationIndex >= 0 )
        {
        operation = KeyMapOperation(operationIndex, iCurrentPage);    
        }
    
    if( operation != KOperation_None )
        {
        // Valid operation.

        switch( operation )
            {
            case KOperation_Exit:
                {                
                CActiveScheduler::Stop();
                break;
                }
            case KOperation_PreviousOption:
                {
                // Change softkey function.
                DecrementKeymapIndex( iSoftkeyIndices[iCurrentPage], iCurrentPage );
                
                // Redraw help text, since a new function should now be underlined.
                DrawHelpText();
                
                // Notify subclass that softkey function has been updated.
                SoftkeyFunctionUpdated();
                break;
                }
            case KOperation_NextOption:
                {
                // Change softkey function.
                IncrementKeymapIndex( iSoftkeyIndices[iCurrentPage], iCurrentPage );
                
                // Redraw help text, since a new function should now be underlined.
                DrawHelpText();
                
                // Notify subclass that softkey function has been updated.
                SoftkeyFunctionUpdated();
                break;
                }
            case KOperation_PreviousOptionPage:
                {
                iCurrentPage = (iCurrentPage + iPageCount - 1) % iPageCount;
                DrawHelpText();
                SoftkeyFunctionUpdated();                
                break;
                }
            case KOperation_NextOptionPage:
                {
                iCurrentPage = (iCurrentPage + 1) % iPageCount;
                DrawHelpText();
                SoftkeyFunctionUpdated();                
                break;
                }
            case KOperation_ToggleHelpVisibility:    
                {
                // Toggle help text on/off.
                iHelpActive = !iHelpActive;
                iHelpWindow->SetVisible(iHelpActive);
                break;
                }
            case KOperation_ToggleHelpTransparency:
                {
                iHelpSemitransparentBackgroundActive = !iHelpSemitransparentBackgroundActive;
                if( iHelpSemitransparentBackgroundActive )
                    {
                    // Turn on help if it is currently off.
                    iHelpActive = true;
                    iHelpWindow->SetVisible(true);
                    }
                DrawHelpText();
                break;            
                }
            default:
                {
                // Pass operation to subclass.
                TPtrC operatioName( KeyMapText(operationIndex, iCurrentPage) );
                ExecuteOperation( operation, operatioName );                        
                break;
                }
            }    
        }        
    
    SetActive();
    iWs.EventReady( &iStatus );
    }            

void CTestAppBase::DoCancel()
    {
    iWs.EventReadyCancel();
    }

// TODO: ALLOW SUBCLASS TO SPECIFY COLOR SELECTIONS

TInt CTestAppBase::SelectFromListL( TPoint aTopLeft,
                                    TSize aSize,
                                    const TDesC& aHeaderText, 
                                    RPointerArray<TDesC>& aSelectionList, 
                                    TInt aInitialSelectionIndex )
    {
    iSelectionWindow->SetExtent( aTopLeft, aSize );
    iSelectionWindow->SetVisible( true );

    const TInt KRowIncrement = iFontSize + 2;
    
    TInt entriesPerPage = aSize.iHeight / KRowIncrement - 4;
    
    TInt returnValue = -2;
    TInt startIndex = 0;
    TInt selected = aInitialSelectionIndex;     
    
    while( returnValue == -2 )
        {
        iGc->Activate(*iSelectionWindow);
    
        iSelectionWindow->Invalidate();
        iSelectionWindow->BeginRedraw();
    
        iGc->Reset();
        
        iGc->UseFont(iFont);                
        iGc->SetBrushColor(KRgbDarkBlue);
        
        iGc->Clear();

        // KRgbWhite seems to be having problems (0xffffff) in some emulators,
        // but 0xfefefe is working, so use that instead of white.        
        iGc->SetPenColor(0xfefefe);

        const TInt KHeaderColumn = 5;
        const TInt KEntryColumn = 15;
        
        TInt row = KRowIncrement;

        iGc->SetUnderlineStyle(EUnderlineOff);

        iGc->DrawText( aHeaderText, TPoint(KHeaderColumn, row) );
        row += (KRowIncrement + 5);
        
        TBool again = true;
        TInt backIndex = -1;
        TInt forwardIndex = -1;
        TInt offset = 0;
                    
        while( again )
            {
            if( selected == offset )
                {
                iGc->SetUnderlineStyle(EUnderlineOn);
                }
            else
                {
                iGc->SetUnderlineStyle(EUnderlineOff);            
                }
        
            if( (offset < entriesPerPage) && (startIndex+offset < aSelectionList.Count()) )
                {
                iGc->DrawText( *aSelectionList[startIndex+offset], TPoint(KEntryColumn, row) );
                row += KRowIncrement;

                offset++;
                }
            else
                {
                again = false;
                if( startIndex + offset < aSelectionList.Count() )
                    {
                    iGc->DrawText( _L("<page down>"), TPoint(KEntryColumn, row) );
                    row += KRowIncrement;
                    
                    forwardIndex = offset;
                    offset++;
                    }
                if( startIndex > 0 )
                    {
                    if( selected == offset )
                        {
                        iGc->SetUnderlineStyle(EUnderlineOn);
                        }
                    else
                        {
                        iGc->SetUnderlineStyle(EUnderlineOff);            
                        }
                
                    iGc->DrawText( _L("<page up>"), TPoint(KEntryColumn, row) );
                    row += KRowIncrement;

                    backIndex = offset;
                    offset++;
                    }
                }
            }
        
        iSelectionWindow->EndRedraw();
       
        iGc->Deactivate();

        TInt keyCode = WaitForAnyKey();        
        
        switch( keyCode )
            {
            case EKeyUpArrow:
                if( selected == 0 )
                    {
                    selected = offset-1;
                    }
                else
                    {
                    selected -= 1;
                    }
                break;
                
            case EKeyDownArrow:
            case KLeftSoftKeyCode:                        
            case 'a':
            case 'A':
                selected += 1;
                if( selected == offset )
                    {
                    selected = 0;
                    }
                break;

            case EKeyLeftArrow:
                if( backIndex >= 0 )
                    {
                    startIndex -= entriesPerPage;
                    selected = 0;                        
                    }
                else
                    {
                    returnValue = -1;
                    again = false;
                    }
                break;
                
            case EKeyRightArrow:
                if( forwardIndex >= 0 )
                    {
                    startIndex += entriesPerPage;
                    selected = 0;                    
                    }
                break;

            case EKeyEnter:
            case KRightSoftKeyCode:                        
            case 'b':
            case 'B':
                if( selected == forwardIndex )
                    {
                    startIndex += entriesPerPage;
                    selected = 0;                    
                    }
                else if( selected == backIndex )
                    {
                    startIndex -= entriesPerPage;
                    selected = 0;
                    }
                else
                    {
                    returnValue = startIndex+selected;
                    }
                break;
            }
        }

    iSelectionWindow->SetVisible( false );
    
    return returnValue;        
    }

TInt CTestAppBase::SelectFromListL( TPoint aTopLeft,
                                    TSize aSize,
                                    const TDesC& aHeaderText, 
                                    RPointerArray<HBufC>& aSelectionList, 
                                    TInt aInitialSelectionIndex )
    {
    RPointerArray<TDesC> aSelectionList2;
    
    for( TInt index = 0; index < aSelectionList.Count(); index++ )
        {
        aSelectionList2.Append( aSelectionList[index]);
        }
    
    TInt returnValue = SelectFromListL( aTopLeft, aSize, aHeaderText, aSelectionList2, aInitialSelectionIndex );
    
    aSelectionList2.Close();
    
    return returnValue;
    }

void CTestAppBase::BuildDriveListL( RPointerArray<HBufC>& aDrives )
    {
    TDriveList driveList;
    TInt err = iFs.DriveList( driveList );
    
    for( TInt index = 0; index < driveList.Length(); index++ )
        {
        if( driveList[index] != 0 )
            {
            HBufC* drive = HBufC::NewL( 2 );
            drive->Des().Format( _L("%c:"), 'A' + index);
            aDrives.Append( drive );
            }
        }
    }

bool CTestAppBase::SelectDriveL( TPoint aTopLeft,
                                 TSize aWindowSize,
                                 const TDesC& aHeaderText, 
                                 TDes& aDrive )
    {
    RPointerArray<HBufC> drives;
    BuildDriveListL( drives );
    
    TInt index = SelectFromListL( aTopLeft, aWindowSize, aHeaderText, drives );
    
    bool returnValue = false;
    
    if( index >= 0 )
        {
        returnValue = true;
        aDrive.Copy( *(drives[index]) );
        aDrive.Append( _L("\\") );
        }
    
    drives.ResetAndDestroy();
    
    return returnValue;
    }

bool CTestAppBase::SelectFileL( TPoint aTopLeft,
                                TSize aWindowSize,
                                const TDesC& aHeaderText, 
                                const TDesC& aDrive, 
                                TDes& aFullFilename )
    {
    TFileName directory;
    
    DoSelectFileL( aTopLeft, aWindowSize, aHeaderText, aDrive, 0, directory, aFullFilename );
    
    aFullFilename.Insert( 0, directory );
    
    return aFullFilename.Length() > 0;
    }

bool CTestAppBase::SelectFileWithHistoryL( TPoint aTopLeft,
                                           TSize aSize,
                                           TDes& aFullFilename,
                                           const TDesC& aHistoryFilename,
                                           TInt aMaxHistoryEntries,
                                           const TDesC& aPrompt )
    {
    RPointerArray<HBufC> drives;
    BuildDriveListL( drives );
        
    RPointerArray<TDesC> selections;

    for( TInt index = 0; index < drives.Count(); index++ )
        {
        selections.Append( drives[index]);
        }
    TInt drivesBaseIndex = 0;
        
    // Add file history to the end of the drive list.  Newest files are last, so add in reverse order.
    // For convenience, add the last history entry at the top, before the drive list.
    ReadFileHistory( aHistoryFilename );
    if( iFileHistory.Count() > 0 )
        {
        selections.Insert( iFileHistory[iFileHistory.Count()-1], 0 );        
        drivesBaseIndex++;
        }
    for(TInt index = iFileHistory.Count()-2; index >= 0; index-- )
        {        
        selections.Append( iFileHistory[index]);
        }

    bool done = false;
    bool selected = true;
    
    while( !done )
        {
        TInt index = SelectFromListL( aTopLeft, aSize, aPrompt, selections );

        if( index < 0 )
            {
            selected = false;
            done = true;
            }        
        else if( (index >= drivesBaseIndex) && (index - drivesBaseIndex < drives.Count()) )
            {
            TBuf<10> drive;
            drive.Copy( *(selections[index]) );
            drive.Append( _L("\\") );
            
            done = SelectFileL( aTopLeft, aSize, aPrompt, drive, aFullFilename );        
            }
        else
            {    
            // Remove the selected file from the history, so that it will pop up to the top of the list
            // as the most recently selected file.
            TInt historyIndex;
            if( index == 0 )
                {
                historyIndex = iFileHistory.Count() - 1;
                }
            else
                {
                historyIndex = iFileHistory.Count() - index + drives.Count() - 1;
                }
            aFullFilename.Copy( *(selections[index]) );
            delete iFileHistory[historyIndex];
            iFileHistory.Remove( historyIndex );            

            done = true;
            }
        }
    
    if( selected )
        {
        AddToFileHistory( aFullFilename, aHistoryFilename, aMaxHistoryEntries );        
        }
    
    selections.Reset();
    drives.ResetAndDestroy();
    
    return selected;
    }

bool CTestAppBase::SelectIntegerL( TPoint aTopLeft,
                                   TSize aSize,
                                   const TDesC& aHeaderText,
                                   TInt aMin,
                                   TInt aMax,
                                   TInt& aSelection )
    {
    // currently no way to exit out of this selection
    
    iSelectionWindow->SetExtent( aTopLeft, aSize );
    iSelectionWindow->SetVisible( true );

    bool done = false;
    
    while( !done )
        {
        iGc->Activate(*iSelectionWindow);
    
        iSelectionWindow->Invalidate();
        iSelectionWindow->BeginRedraw();
    
        iGc->Reset();
        
        iGc->UseFont(iFont);                
        iGc->SetBrushColor(KRgbDarkBlue);
        
        iGc->Clear();

        // KRgbWhite seems to be having problems (0xffffff) in some emulators,
        // but 0xfefefe is working, so use that instead of white.        
        iGc->SetPenColor(0xfefefe);

        TBuf<120> buffer;
        buffer.Copy( aHeaderText );        
        buffer.AppendFormat( _L(" %i"), aSelection );
        
        iGc->DrawText( buffer, TPoint(5, iFontSize+2) );

        iSelectionWindow->EndRedraw();
       
        iGc->Deactivate();

        TInt keyCode = WaitForAnyKey();
        
        switch( keyCode )
            {
            case EKeyUpArrow:
                aSelection -= 10;
                break;
                
            case EKeyDownArrow:
            case KLeftSoftKeyCode:                        
            case 'a':
            case 'A':
                aSelection += 10;
                break;

            case EKeyLeftArrow:
                aSelection--;
                break;
                
            case EKeyRightArrow:
                aSelection++;
                break;

            case EKeyEnter:
            case KRightSoftKeyCode:                        
            case 'b':
            case 'B':
                done = true;
                break;
            }
        
        if( aSelection > aMax )
            {
            aSelection = aMin;
            }
        else if( aSelection < aMin )
            {
            aSelection = aMax;
            }        
        }

    iSelectionWindow->SetVisible( false );
    
    return true;
    }

TInt CTestAppBase::WaitForAnyKey()
    {
    TInt returnValue = 0;
    
    bool done = false;
    
    while( !done )
        {        
        // Have to use this tricky nested active scheduler technique to allow the active object
        // used to remap volume keys to run.
        SetActive();
        iWs.EventReady( &iStatus );
        iWait.Start();
    
        TWsEvent event;
        iWs.GetEvent(event);
    
        // Other potentially useful events are EEventKeyUp and EEventKeyDown.
        
        if( event.Type() == EEventKey )
            {
            done = true;
            returnValue = event.Key()->iCode;
            }
        else if( event.Type() == EEventPointer )
            {
            TAdvancedPointerEvent* p = event.Pointer();

            TTestAppPointerEvent pointerEvent = CharacterizePointerEvent( *p );

            switch( pointerEvent )
                {
                case EPointerEvent_None:
                    // Do nothing.
                    break;
                case EPointerEvent_Up:
                    returnValue = EKeyUpArrow;
                    done = true;
                    break;
                case EPointerEvent_Down:
                    returnValue = EKeyDownArrow;
                    done = true;
                    break;
                case EPointerEvent_Left:
                    returnValue = EKeyLeftArrow;
                    done = true;
                    break;
                case EPointerEvent_Right:
                    returnValue = EKeyRightArrow;
                    done = true;
                    break;
                case EPointerEvent_Select:
                    returnValue = EKeyEnter;
                    done = true;
                    break;
                }
            }        
        }
    
    return returnValue;
    }

TInt CTestAppBase::ReadFile( const TDesC& aFilename, RPointerArray<HBufC>& aContents )
    {
    RFile inputFile;
    TInt err = inputFile.Open(iFs, aFilename, EFileShareReadersOnly | EFileStream | EFileRead );
    
    if( err == KErrNone )
        {
        TInt fileSize;
        inputFile.Size( fileSize );
        
        RBuf8 contents;
        contents.Create( fileSize );

        inputFile.Read( contents, fileSize );

        inputFile.Close();

        TPtrC8 remaining( contents );
        
        while( remaining.Length() > 0 )
            {
            // Removing leading CR/LFs, if any.
            while( remaining.Length() > 0 && (remaining[0] == '\n' || remaining[0] == '\r') )
                {
                remaining.Set(remaining.Right(remaining.Length()-1));            
                }

            if( remaining.Length() == 0 )
                {
                break;
                }

            // Find next CR/LF, or end of string.
            TInt separatorIndex = 1;
            while( (separatorIndex < remaining.Length()) && 
                   (remaining[separatorIndex] != '\n') &&
                   (remaining[separatorIndex] != '\r'))
                {
                separatorIndex++;
                }
                        
            HBufC* line = HBufC::NewL( separatorIndex );
            TPtrC8 line8 = remaining.Left( separatorIndex );
            line->Des().Copy( line8 );
        
            aContents.Append( line );

            TInt remainingLength = remaining.Length() - separatorIndex;            

            remaining.Set(remaining.Right(remainingLength));            
            }        
        
        contents.Close();
        }
    
    return err;
    }

void CTestAppBase::ReadFileHistory( const TDesC& aHistoryFilename )
    {
    iFileHistory.ResetAndDestroy();
    
    ReadFile( aHistoryFilename, iFileHistory );
    }

void CTestAppBase::AddToFileHistory( const TDesC& aFilename, const TDesC& aHistoryFilename, TInt aMaxHistoryEntries )
    {
    HBufC* filename = HBufC::NewL( aFilename.Length() );
    filename->Des().Copy( aFilename );
    iFileHistory.Append( filename );

    while( iFileHistory.Count() > aMaxHistoryEntries )
        {
        delete iFileHistory[0];
        iFileHistory.Remove( 0 );
        }    
    
    RFile historyFile;
    TInt err = historyFile.Create(iFs, aHistoryFilename, EFileStream | EFileWrite );    
    if( err == KErrAlreadyExists )
        {
        err = historyFile.Open(iFs, aHistoryFilename, EFileStream | EFileWrite );
        historyFile.SetSize(0);
        }
    
    if( err == KErrNone )
        {
        for( TInt index = 0; index < iFileHistory.Count(); index++ )
            {
            TBuf8<KMaxFileName> filename8;
            filename8.Copy( iFileHistory[index]->Des() );
            historyFile.Write( filename8 );
            historyFile.Write( _L8("\n") );
            }
        }
    
    historyFile.Close();    
    }

void CTestAppBase::DoSelectFileL( TPoint aTopRight,
                                  TSize aWindowSize,
                                  const TDesC& aHeaderText, 
                                  const TFileName& aDirectory, 
                                  TInt aDirectoryLevel, 
                                  TDes& aSelectedDirectory, 
                                  TDes& aSelectedFilename )     
    {
    RPointerArray<TDesC> fileNames;
    
    ReadDirectoryEntriesL( aDirectory, fileNames );

    TInt initialSelectionIndex = 0;
    
    _LIT( KUp, ".." );        
    if( aDirectoryLevel > 0 )
        {
        TFileName* newEntry = new(ELeave) TFileName;
        newEntry->Copy( KUp );            
        fileNames.Insert( newEntry, 0 );
        initialSelectionIndex++;
        }
    
    bool done = false;    
    
    while( !done && (aSelectedFilename.Length() == 0) )
        {
        TInt index = SelectFromListL( aTopRight, aWindowSize, aHeaderText, fileNames, initialSelectionIndex );

        if( index < 0 || index >= fileNames.Count() )
            {
            done = true;
            }
        else if( fileNames[index]->Compare(KUp) == 0 )
            {
            // Go up one directory.  Return to caller without setting aFilename
            break;
            }
        else if( (*fileNames[index])[fileNames[index]->Length()-1] == '\\' )
            {
            // Directory selected.
            TFileName directory;
            directory.Copy( aDirectory );
            directory.Append( *fileNames[index] );
            DoSelectFileL( aTopRight, aWindowSize, aHeaderText, directory, aDirectoryLevel+1, aSelectedDirectory, aSelectedFilename );
            }
        else
            {
            // File selected.                            
            aSelectedDirectory.Copy( aDirectory );
            aSelectedFilename.Copy( *fileNames[index] );
            done = true;
            }
        
        }

    fileNames.ResetAndDestroy();
    }

void CTestAppBase::ReadDirectoryEntriesL( const TFileName& aDirectoryName, RPointerArray<TDesC>& aFileNames )
    {
    aFileNames.ResetAndDestroy();
    
    RDir dir;
    User::LeaveIfError( dir.Open( iFs, aDirectoryName, KEntryAttNormal|KEntryAttDir) );
    
    TEntryArray entries;
    TInt err = KErrNone;
    TInt directoryCount = 0;
    while( err == KErrNone )
        {
        err = dir.Read( entries );
                
        for( TInt index = 0; index < entries.Count(); index++ )
            {
            // It was observed that not all devices sorted the list by default, so do an
            // insertion sort here.            
            TFileName* newEntry = new(ELeave) TFileName;
            newEntry->Copy( entries[index].iName );
            
            // Put the directories first in the list.
            if( entries[index].IsDir())
                {
                newEntry->Append( _L("\\") );
                
                TInt insertionIndex = 0;
                while( insertionIndex < directoryCount && newEntry->CompareF(*aFileNames[insertionIndex]) > 0 )
                    {
                    insertionIndex++;
                    }
                aFileNames.Insert( newEntry, insertionIndex );
                
                directoryCount++;
                }
            else
                {
                TInt insertionIndex = directoryCount;
                while( insertionIndex < aFileNames.Count() && newEntry->CompareF(*aFileNames[insertionIndex]) > 0 )
                    {
                    insertionIndex++;
                    }
                aFileNames.Insert( newEntry, insertionIndex );
                }
            }        
        }      
    
    dir.Close();
    }

void CTestAppBase::DrawHelpText()
    {
    iGc->Activate(*iHelpWindow);

    iHelpWindow->Invalidate();
    iHelpWindow->BeginRedraw();

    iGc->Reset();
    
    iGc->UseFont(iFont);                
    iGc->SetBrushColor(KRgbTransparent);
    
    iGc->Clear();

    if( iHelpSemitransparentBackgroundActive )
        {
        iGc->SetPenColor(KRgbTransparent);
        iGc->SetBrushStyle(CWindowGc::ESolidBrush);
        iGc->SetBrushColor( TRgb(0x7f7f7faf) );
        iGc->DrawRect( TRect(iHelpWindowSize) );
        }

    // KRgbWhite seems to be having problems (0xffffff) in some emulators,
    // but 0xfefefe is working, so use that instead of white.        
    iGc->SetPenColor(0xfefefe);

    const TInt KColumn1 = KHelpWindowBorderPixels;
    const TInt KColumn2 = KColumn1 + iHelpWindowColumn1Width + KHelpWindowSpaceBetweenColumns;
    const TInt KRowIncrement = iFontSize + KHelpWindowSpaceBetweenRows;
    
    TInt row = iFontSize + KHelpWindowBorderPixels;

    for( TInt index = 0; index < KSupportedKeysCount; index++ )
        {
        iGc->SetUnderlineStyle(EUnderlineOff);        
    
        TPtrC text = KeyMapText( index, iCurrentPage ); 

        iGc->DrawText( KeyName(index), TPoint(KColumn1, row) );

        if( index == iSoftkeyIndices[iCurrentPage] )
            {
            iGc->SetUnderlineStyle(EUnderlineOn);
            }
        else
            {
            iGc->SetUnderlineStyle(EUnderlineOff);        
            }

        iGc->DrawText( text, TPoint(KColumn2, row) );
        
        row += KRowIncrement;
        }

    iHelpWindow->EndRedraw();
   
    iGc->Deactivate();
    }

void CTestAppBase::MrccatoCommand( TRemConCoreApiOperationId aOperationId,
                                   TRemConCoreApiButtonAction /*aButtonAct*/ )
    {
    // Treat volume up like the right soft key, and volume down like the left soft key.
    TKeyEvent keyEvent;
    keyEvent.iCode = 0;
    keyEvent.iScanCode = 0;
    keyEvent.iModifiers = 0;
    keyEvent.iRepeats = 0;
    
    switch (aOperationId)
        {
        case ERemConCoreApiVolumeUp:
            keyEvent.iCode = KRightSoftKeyCode;
            iWs.SimulateKeyEvent(keyEvent);
            iWs.Flush();
            break;
        case ERemConCoreApiVolumeDown:
            keyEvent.iCode = KLeftSoftKeyCode;
            iWs.SimulateKeyEvent(keyEvent);
            iWs.Flush();
            break;
        default:
            break;
        }
    }
