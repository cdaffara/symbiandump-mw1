/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Light control extension plugin interface inline methods
*
*/


#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
// Destructor informs ECom that implementation has been destroyed.
// -----------------------------------------------------------------------------
//
inline CSysApLightControlPlugin::~CSysApLightControlPlugin()
    {
    REComSession::DestroyedImplementation(iDestructorIDKey);
    }

// -----------------------------------------------------------------------------
// Factory method to create implementation. Implementation is found via ECom.
// -----------------------------------------------------------------------------
//
inline CSysApLightControlPlugin* CSysApLightControlPlugin::NewL()
    {
    TEComResolverParams resolverParams; // empty set of resolver parameters
    
    TAny* interface = REComSession::CreateImplementationL( TUid::Uid(KSysApLightControlPluginInterfaceUid), 
                                                           _FOFF(CSysApLightControlPlugin, iDestructorIDKey), 
                                                           resolverParams,
                                                           KRomOnlyResolverUid );

    return reinterpret_cast<CSysApLightControlPlugin*>(interface);
    }
            
// End of File
