/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  stores the parameters needed for xmpp
*
*/

#include "xmppparameters.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXmppParameters::CXmppParameters
// ---------------------------------------------------------------------------
//
CXmppParameters::CXmppParameters()
    {
    }


// ---------------------------------------------------------------------------
// CXmppParameters::ConstructL
// ---------------------------------------------------------------------------
//
void CXmppParameters::ConstructL()
    {
    iServerAddress = HBufC::NewL(KMaxLength);
    iUserName = HBufC::NewL(KMaxLength);
    iPassword = HBufC::NewL(KMaxLength);
    iResource = HBufC::NewL(KMaxLength);
    iProxyServer = HBufC::NewL(KMaxLength);
    
    iConnMgrBus = HBufC::NewL(KMaxLength);
    iConnMgrPath = HBufC::NewL(KMaxLength);
    iProtocol = HBufC::NewL(KMaxLength);
    
    }


// ---------------------------------------------------------------------------
// CXmppParameters::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CXmppParameters* CXmppParameters::NewL()
    {
 
    CXmppParameters* self = new( ELeave )
                            CXmppParameters();
                            
    CleanupStack::PushL( self );
    
    self->ConstructL();
    CleanupStack::Pop( self );
	
    return self;
    }


// ---------------------------------------------------------------------------
// CXmppParameters::~CXmppParameters
// ---------------------------------------------------------------------------
//
CXmppParameters::~CXmppParameters()
    {
    delete iServerAddress;
    delete iUserName;
    delete iPassword;
    delete iResource;
    delete iProxyServer;
    
    delete iProtocol;
    delete iConnMgrPath;
    delete iConnMgrBus;
    // before destory cancel each of the active objects request
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetIapId
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetIapId(TInt32 aIapId)
    {
    iIapId = aIapId;
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetSettingsId
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetSettingsId(TUint32 aSettingsId)
    {
    iSettingsId = aSettingsId;
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetServerAddress
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetServerAddress(const TDesC& aServerAddress)
    {
    iServerAddress->Des().Copy(aServerAddress);
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetUsername
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetUserName(const TDesC& aUserName)
    {
    iUserName->Des().Copy(aUserName);
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetPasswd
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetPasswd(const TDesC& aPassword)
    {
    iPassword->Des().Copy(aPassword);
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetResource
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetResource(const TDesC& aResource)
    {
    iResource->Des().Copy(aResource);
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetProxyServer
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetProxyServer(const TDesC& aProxyServer)
    {
    iProxyServer->Des().Copy(aProxyServer);
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetServerPort
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetServerPort(TInt32 aServerPort)
    {
    iServerPort = aServerPort;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetProxyPort
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetProxyPort(TInt32 aProxyPort)
    {
    iProxyPort = aProxyPort;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::SetSsl
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetSsl(TBool aSsl)
    {
    iSsl = aSsl;
    }


// ---------------------------------------------------------------------------
// CXmppParameters::SetProxyServer
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetConnMgrBus(const TDesC& aConnMgrBus)
    {
    iConnMgrBus->Des().Copy(aConnMgrBus);
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetProxyServer
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetConnMgrPath(const TDesC& aConnMgrPath)
    {
    iConnMgrPath->Des().Copy(aConnMgrPath);
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SetProxyServer
// ---------------------------------------------------------------------------
//
void CXmppParameters::SetProtocol(const TDesC& aProtocol)
    {
    iProtocol->Des().Copy(aProtocol);
    }        
// ---------------------------------------------------------------------------
// CXmppParameters::IapId
// ---------------------------------------------------------------------------
//
EXPORT_C TInt32 CXmppParameters::IapId()
    {
    return iIapId;
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::SettingsId
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CXmppParameters::SettingsId()
    {
    return iSettingsId;
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::ServerAddress
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::ServerAddress()
    {
    return *iServerAddress;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::Username
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::UserName()
    {
    return *iUserName;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::Passwd
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::Passwd()
    {
    return *iPassword;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::Resource
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::Resource()
    {
    return *iResource;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::ProxyServer
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::ProxyServer()
    {
    return *iProxyServer;
    }
    
// ---------------------------------------------------------------------------
// CXmppParameters::ServerPort
// ---------------------------------------------------------------------------
//
EXPORT_C TInt32 CXmppParameters::ServerPort()
    {
    return iServerPort;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::ProxyPort
// ---------------------------------------------------------------------------
//
EXPORT_C TInt32 CXmppParameters::ProxyPort()
    {
    return iProxyPort;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::Ssl
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CXmppParameters::Ssl()
    {
    return iSsl;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::ConnMgrBus
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::ConnMgrBus()
    {
    return *iConnMgrBus;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::ConnMgrPath
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::ConnMgrPath()
    {
    return *iConnMgrPath;
    }

// ---------------------------------------------------------------------------
// CXmppParameters::Protocol
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CXmppParameters::Protocol()
    {
    return *iProtocol;
    }


// End of File

