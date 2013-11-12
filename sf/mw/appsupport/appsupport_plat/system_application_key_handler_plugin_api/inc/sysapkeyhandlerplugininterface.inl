/*
* Copyright (c) 2005, 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Defines an ECom plug-in interface
*
*/



// ---------------------------------------------------------------------------
// CSysapKeyHandlerPlugin::CSysapKeyHandlerPlugin
//
// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concrete plugin
// implementation.
// ---------------------------------------------------------------------------
//
inline CSysapKeyHandlerPlugin* CSysapKeyHandlerPlugin::NewL( TUid aUid,  MSysapCallback* aCallback )
    {
    TAny* interface = REComSession::CreateImplementationL( aUid,_FOFF (CSysapKeyHandlerPlugin, iDtor_ID_Key) );

    CSysapKeyHandlerPlugin* self = reinterpret_cast <CSysapKeyHandlerPlugin*> (interface);

    self->iCallback = aCallback;
    
    self->PostConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// CSysapKeyHandlerPlugin::ProvideKeyEventsL
// ---------------------------------------------------------------------------
//
inline void CSysapKeyHandlerPlugin::ProvideKeyEventsL( RArray<TKeyEvent>& aKeyEventArray )
    {
    aKeyEventArray.Reset();
    }

// ---------------------------------------------------------------------------
// CSysapKeyHandlerPlugin::SysapCallback
// ---------------------------------------------------------------------------
//
inline MSysapCallback* CSysapKeyHandlerPlugin::SysapCallback()
    {
    __ASSERT_DEBUG( iCallback, User::Panic( _L("CSysapKeyHandlerPlugin::SysapCallback()"), KErrGeneral ) );
    
    return iCallback;
    }
    
// ---------------------------------------------------------------------------
// CSysapKeyHandlerPlugin::~CSysapKeyHandlerPlugin
//
// Interface's (abstract base class's) destructor
// ---------------------------------------------------------------------------
//
inline CSysapKeyHandlerPlugin::~CSysapKeyHandlerPlugin()
    {
    // If in the NewL some memory is reserved for member data, it must be
    // released here. This interface does not have any instance variables so
    // no need to delete anything.

    // Inform the ECOM framework that this specific instance of the
    // interface has been destroyed.
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

