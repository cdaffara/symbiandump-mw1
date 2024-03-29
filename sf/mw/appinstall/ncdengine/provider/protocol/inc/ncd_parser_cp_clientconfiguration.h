/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   CNcdConfigurationProtocolClientConfigurationParser declaration
*
*/


#ifndef C_NCDCONFIGURATIONPROTOCOLCLIENTCONFIGURATIONPARSER_H
#define C_NCDCONFIGURATIONPROTOCOLCLIENTCONFIGURATIONPARSER_H

#include "ncdsubparser.h"

class MNcdConfigurationProtocolClientConfiguration;
class CNcdConfigurationProtocolClientConfigurationImpl;

class CNcdConfigurationProtocolClientConfigurationParser : public CNcdSubParser                                                           
    {
public:
    static CNcdConfigurationProtocolClientConfigurationParser* NewL( MNcdParserObserverBundle& aObservers, 
                                                                     MNcdSubParserObserver& aSubParserObserver,
                                                                     TInt aDepth,
                                                                     const Xml::RTagInfo& aElement,
                                                                     const Xml::RAttributeArray& aAttributes,
                                                                     const TDesC& aParentId );

    static CNcdConfigurationProtocolClientConfigurationParser* NewL( MNcdParserObserverBundle& aObservers, 
                                                                     MNcdSubParserObserver& aSubParserObserver,
                                                                     TInt aDepth );

    virtual ~CNcdConfigurationProtocolClientConfigurationParser();

    MNcdConfigurationProtocolClientConfiguration* ClientConfiguration();

private:
    CNcdConfigurationProtocolClientConfigurationParser( MNcdParserObserverBundle& aObservers, 
                                                        MNcdSubParserObserver& aSubParserObserver,
                                                        TInt aDepth );
    void ConstructL( const Xml::RTagInfo& aElement,
                     const Xml::RAttributeArray& aAttributes,
                     const TDesC& aParentId );

    void ConstructL();
                     
protected:
    virtual void OnStartElementL( const Xml::RTagInfo& aElement, 
                          const Xml::RAttributeArray& aAttributes, 
                          TInt aErrorCode );

    virtual void OnEndElementL( const Xml::RTagInfo& aElement, 
                                TInt aErrorCode );

    virtual void SubParserFinishedL( const TDesC8& aTag, TInt aErrorCode );

private:
    /**
     * The data object being built
     */
    CNcdConfigurationProtocolClientConfigurationImpl* iClientConfiguration;
    };

#endif
