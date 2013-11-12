/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CNspsWsPlugin method definitions
*
*/


// INCLUDE FILES
#include <e32property.h>
#include "coreapplicationuisinternalpskeys.h"
#include "NspsWsPluginDllStd.h"
#include "NspsWsPlugin.h"         // this

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNspsWsPlugin::CNspsWsPlugin()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------

CNspsWsPlugin::CNspsWsPlugin()
    {
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::~CNspsWsPlugin()
// -----------------------------------------------------------------------------

CNspsWsPlugin::~CNspsWsPlugin()
    {
    if ( iFunctions )
        {
        iFunctions->GetRawEvents( EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::CommandReplyL( TInt /*aOpcode*/, TAny* /*aArgs*/ )
// -----------------------------------------------------------------------------

TInt CNspsWsPlugin::CommandReplyL( TInt /*aOpcode*/, TAny* /*aArgs*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::Command( TInt aOpcode, TAny* /*aArgs*/ )
// -----------------------------------------------------------------------------

void CNspsWsPlugin::Command( TInt aOpcode, TAny* /*aArgs*/ )
    {
    TRACES( RDebug::Print(_L("CNspsWsPlugin::Command( aOpcode:%d"), aOpcode) );
    switch ( aOpcode )
        {
        case EActivateNspsKeyeventForwarding:
            if( !iForwardRawKeyeventsToLights && !iForwardRawKeyeventsToNcn && !iForwardRawKeyeventsToSysAp )
                {
                iFunctions->GetRawEvents( ETrue );
                }
            iForwardRawKeyeventsToSysAp = ETrue;
            break;
        case EDeActivateNspsKeyeventForwarding:
            if( ! ( iForwardRawKeyeventsToNcn || iForwardRawKeyeventsToLights ) )
                {
                iFunctions->GetRawEvents( EFalse );
                }
            iForwardRawKeyeventsToSysAp = EFalse;
            break;
        
        case EActivateKeyeventForwardingForNcn:
            if( !iForwardRawKeyeventsToLights && !iForwardRawKeyeventsToNcn && !iForwardRawKeyeventsToSysAp )
                {
                iFunctions->GetRawEvents( ETrue );
                }
            iForwardRawKeyeventsToNcn = ETrue;
            break;
        case EDeActivateKeyeventForwardingForNcn:
            if( !( iForwardRawKeyeventsToSysAp || iForwardRawKeyeventsToLights ) )
                {
                iFunctions->GetRawEvents( EFalse );
                }
            iForwardRawKeyeventsToNcn = EFalse;
            break;
            
        case EActivateKeyeventForwardingForLights:
            if( !iForwardRawKeyeventsToLights && !iForwardRawKeyeventsToNcn && !iForwardRawKeyeventsToSysAp )
                {
                iFunctions->GetRawEvents( ETrue );
                }
            iForwardRawKeyeventsToLights = ETrue;
            break;
        case EDeActivateKeyeventForwardingForLights:
            if( !( iForwardRawKeyeventsToSysAp || iForwardRawKeyeventsToNcn ) )
                {
                iFunctions->GetRawEvents( EFalse );
                }
            iForwardRawKeyeventsToLights = EFalse;
            break;
            
        default:
            // do nothing
            break;
        }
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::Animate( TDateTime* /*aDateTime*/ )
// -----------------------------------------------------------------------------

void CNspsWsPlugin::Animate( TDateTime* /*aDateTime*/ )
    {
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::ConstructL( TAny* /*aArgs*/, TBool /*aHasFocus*/ )
// -----------------------------------------------------------------------------

void CNspsWsPlugin::ConstructL( TAny* /*aArgs*/, TBool /*aHasFocus*/ )
    {
    TRACES( RDebug::Print( _L( "CNspsWsPlugin::ConstructL" ) ) );
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::Redraw()
// -----------------------------------------------------------------------------

void CNspsWsPlugin::Redraw()
    {
    }

// -----------------------------------------------------------------------------
// CNspsWsPlugin::FocusChanged( TBool /*aState*/ )
// -----------------------------------------------------------------------------

void CNspsWsPlugin::FocusChanged( TBool /*aState*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSysApAppUi::CNspsWsPlugin::OfferRawEvent( const TRawEvent& aRawEvent )
// -----------------------------------------------------------------------------

TBool CNspsWsPlugin::OfferRawEvent( const TRawEvent& aRawEvent )
    {
    if ( aRawEvent.Type() == TRawEvent::EKeyDown )
        {
        TRACES( RDebug::Print( _L( "CNspsWsPlugin::OfferRawEvent:TRawEvent::EKeyDown" ) ) );

        if ( iForwardRawKeyeventsToLights )
            {
            TRACES( RDebug::Print(_L("CNspsWsPlugin::OfferRawEvent:TRawEvent::EKeyDown, Forwarded to Lights Controller" ) ) );
            RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsLightsRawKeyEvent, ECoreAppUIsKeyEvent );
            }

        if ( iForwardRawKeyeventsToSysAp )
            {
            TRACES( RDebug::Print(_L("CNspsWsPlugin::OfferRawEvent:TRawEvent::EKeyDown, Forwarded to SysAp" ) ) );
            RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsNspsRawKeyEvent, ECoreAppUIsKeyEvent );
            }

        if ( iForwardRawKeyeventsToNcn )
            {
            TRACES( RDebug::Print(_L("CNspsWsPlugin::OfferRawEvent:TRawEvent::EKeyDown, Forwarded to MessageToneQuitting" ) ) );
            RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit, ECoreAppUIsStopTonePlaying );
            }
        }
    
    if ( aRawEvent.Type() == TRawEvent::EButton1Down ) // tapping screen also silents message tone
        {
        if ( iForwardRawKeyeventsToNcn )
            {
            TRACES( RDebug::Print(_L("CNspsWsPlugin::OfferRawEvent:TRawEvent::EPointerSwitchOn, Forwarded to MessageToneQuitting" ) ) );
            RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsMessageToneQuit, ECoreAppUIsStopTonePlaying );
            }
        }
            
    return EFalse;
    }

// End of File


