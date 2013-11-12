/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base class for Presence Framework protocol plug-ins.
*
*/

#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
// CXIMPProtocolPluginBase::CXIMPProtocolPluginBase()
// C++ default constructor.
// -----------------------------------------------------------------------------
//
inline CXIMPProtocolPluginBase::CXIMPProtocolPluginBase()
    {
    }


// -----------------------------------------------------------------------------
// CXIMPProtocolPluginBase::~CXIMPProtocolPluginBase()
// Destructor notifies the ECom from plug-in implementation destruction.
// -----------------------------------------------------------------------------
//
inline CXIMPProtocolPluginBase::~CXIMPProtocolPluginBase()
    {
    REComSession::DestroyedImplementation( iEcomDtorID );
    }


// -----------------------------------------------------------------------------
// CXIMPProtocolPluginBase::SetDestructorId()
// -----------------------------------------------------------------------------
//
inline void CXIMPProtocolPluginBase::SetDestructorId( TUid aId )
    {
    iEcomDtorID = aId;
    }
