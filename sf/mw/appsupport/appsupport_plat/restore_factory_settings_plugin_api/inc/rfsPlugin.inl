/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Defines RFS ECom interface
*
*/


inline CRFSPlugin::CRFSPlugin()
    {
    }

inline CRFSPlugin::~CRFSPlugin()
    {
    // Destroy any instance variables and then
    // inform the framework that this specific
    // instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }

inline CRFSPlugin* CRFSPlugin::NewL( const TUid& aImplementationUid)
    {
    
    CRFSPlugin* plugin =
        reinterpret_cast<CRFSPlugin*> (
            REComSession::CreateImplementationL(aImplementationUid,
            _FOFF(CRFSPlugin, iDtor_ID_Key)));

    return plugin;    
    }

inline void CRFSPlugin::ListAllImplementationsL(RImplInfoPtrArray& aImplInfoArray)
    {
    REComSession::ListImplementationsL( KRfsPluginIfUid, aImplInfoArray );    
    }

