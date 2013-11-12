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
 * Description: casrvplugin.inl
 *
 */

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline CCaSrvPlugin::CCaSrvPlugin()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline CCaSrvPlugin::~CCaSrvPlugin()
    {
    // Destroy any instance variables and then
    // inform the framework that this specific
    // instance of the interface has been destroyed.
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline CCaSrvPlugin* CCaSrvPlugin::NewL( TUid aUid,
        TPluginParams* aPluginParam )
    {
    return REINTERPRET_CAST( CCaSrvPlugin*,
            REComSession::CreateImplementationL( aUid,
                    _FOFF( CCaSrvPlugin, iDtor_ID_Key ), aPluginParam) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void CCaSrvPlugin::ListAllImplementationsL(
        RImplInfoPtrArray& aImplInfoArray )
    {
    REComSession::ListImplementationsL( TUid::Uid( KCaSrvInterfaceUid ),
            aImplInfoArray );
    }

// End of File
