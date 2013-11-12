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

#ifndef __CXMPPPARAMETERS_H__
#define __CXMPPPARAMETERS_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

const TUint32 KMaxLength = 127;

/**
 * CXmppParameters
 *
 * parameters for xmpp
 *
 */
class CXmppParameters: public CBase
    {
public:
		
	 /**
	  * Two Phase Constructor
	  */
    IMPORT_C static CXmppParameters* NewL();
    
   /**
    * Destructor
    */
    virtual ~CXmppParameters();


private:
		
	 /**
	  * Default Constructor
	  */	
    CXmppParameters();
    void ConstructL();


public: 

    /**
     * Sets the value of the member variable iIapId
     *
     * @param aIapId value to set
     */
	void SetIapId(TInt32 aIapId);

    /**
     * Sets the value of the member variable iSettingsId
     *
     * @param aSettingsId value to set
     */
	void SetSettingsId(TUint32 aSettingsId);

    /**
     * Sets the value of the member variable iServerAddress
     *
     * @param aServerAddress value to set
     */
	void SetServerAddress(const TDesC& aServerAddress);

    /**
     * Sets the value of the member variable iUserName
     *
     * @param aUserName value to set
     */
	void SetUserName(const TDesC& aUserName);

    /**
     * Sets the value of the member variable iPassword
     *
     * @param aPassword value to set
     */
	void SetPasswd(const TDesC& aPassword);

    /**
     * Sets the value of the member variable iResource
     *
     * @param aResource value to set
     */
	void SetResource(const TDesC& aResource);

    /**
     * Sets the value of the member variable iProxyServer
     *
     * @param aProxyServer value to set
     */
	void SetProxyServer(const TDesC& aProxyServer);

    /**
     * Sets the value of the member variable iServerPort
     *
     * @param aServerPort value to set
     */
	void SetServerPort(TInt32 aServerPort);

    /**
     * Sets the value of the member variable iProxyPort
     *
     * @param aProxyPort value to set
     */
	void SetProxyPort(TInt32 aProxyPort);

    /**
     * Sets the value of the member variable iSsl
     *
     * @param aSsl value to set
     */
	void SetSsl(TBool aSsl);
	
	/**
     * Sets the value of the member variable iConnMgrBus
     *
     * @param aSsl value to set
     */
	void CXmppParameters::SetConnMgrBus(const TDesC& aConnMgrBus);
	
	/**
     * Sets the value of the member variable iConnMgrPath
     *
     * @param aSsl value to set
     */
	void CXmppParameters::SetConnMgrPath(const TDesC& aConnMgrPath);
	
	/**
     * Sets the value of the member variable iProtocol
     *
     * @param aSsl value to set
     */
	void CXmppParameters::SetProtocol(const TDesC& aProtocol);

    /**
     * Returns the value of the member variable iIapId
     *
     * @param None
     */
	IMPORT_C TInt32 IapId();

    /**
     * Returns the value of the member variable iSettingsId
     *
     * @param None
     */
	IMPORT_C TUint32 SettingsId();

    /**
     * Returns the value of the member variable iServerAddress
     *
     * @param None
     */
    IMPORT_C const TDesC& ServerAddress();

    /**
     * Returns the value of the member variable iUserName
     *
     * @param None
     */
    IMPORT_C const TDesC& UserName();

    /**
     * Returns the value of the member variable iPassword
     *
     * @param None
     */
    IMPORT_C const TDesC& Passwd();

    /**
     * Returns the value of the member variable iResource
     *
     * @param None
     */
    IMPORT_C const TDesC& Resource();

    /**
     * Returns the value of the member variable iProxyServer
     *
     * @param None
     */
    IMPORT_C const TDesC& ProxyServer();

    /**
     * Returns the value of the member variable iServerPort
     *
     * @param None
     */
	IMPORT_C TInt32 ServerPort();

    /**
     * Returns the value of the member variable iProxyPort
     *
     * @param None
     */
	IMPORT_C TInt32 ProxyPort();

    /**
     * Returns the value of the member variable iSsl
     *
     * @param None
     */
	IMPORT_C TBool Ssl();
	
	/**
     * Returns the value of the member variable iConnMgrBus
     *
     * @param None
     */
	IMPORT_C const TDesC& CXmppParameters::ConnMgrBus();
	
	/**
     * Returns the value of the member variable iConnMgrPath
     *
     * @param None
     */
	IMPORT_C const TDesC& CXmppParameters::ConnMgrPath();
	
	/**
     * Returns the value of iProtocol
     *
     * @param None
     */
	IMPORT_C const TDesC& CXmppParameters::Protocol();
	
private: // data

    /**
     * IapId to use for the xmpp connection
     */
    TInt32 iIapId;

    /**
     * settingsid in the xmpp settings store
     */
    TInt32 iSettingsId;
    
    /**
     * xmpp server to connect to
     */
    HBufC* iServerAddress;

    /**
     * username to use for the xmpp connection
     */
    HBufC* iUserName;

    /**
     * password of the user
     */
    HBufC* iPassword;

    /**
     * resource to use for the xmpp connection
     */
    HBufC* iResource;

    /**
     * proxy server to use for the xmpp connection
     */
    HBufC* iProxyServer;

    /**
     * port number to connect to in the xmpp server
     */
    TInt32 iServerPort;

    /**
     * port number of the proxy server to connect to
     */
    TInt32 iProxyPort;

    /**
     * indicates whether to use ssl or not for the connection
     */
    TBool iSsl;
    
    /**
    * Connection manager bus : Protocol specific
    */
    HBufC* iConnMgrBus;
	
	/**
    * Connection manager path : Protocol specific
    */
    HBufC* iConnMgrPath;
    
    /**
    * Protocol name
    */
    HBufC* iProtocol;
   };


#endif // __CXMPPSETTINGSFETCHER_H__

