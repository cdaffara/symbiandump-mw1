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
* Description:   CNcdPreminetProtocolPurchaseInformationParser declaration
*
*/


#ifndef NCD_PREMINET_PROTOCOL_PURCHASEINFORMATION_PARSER_H
#define NCD_PREMINET_PROTOCOL_PURCHASEINFORMATION_PARSER_H

#include "ncdsubparser.h"

class CNcdPreminetProtocolPurchaseInformationImpl;
class MNcdPreminetProtocolPurchaseInformation;

/**
 * Parser for 'purchaseInformation' element
 */
class CNcdPreminetProtocolPurchaseInformationParser : public CNcdSubParser
    {
public:
    static CNcdPreminetProtocolPurchaseInformationParser* NewL( 
        MNcdParserObserverBundle& aObservers, 
        MNcdSubParserObserver& aSubParserObserver,
        TInt aDepth,
        const Xml::RTagInfo& aElement,
        const Xml::RAttributeArray& aAttributes,
        const TDesC& aParentId );
    
    virtual ~CNcdPreminetProtocolPurchaseInformationParser();

    /**
     * @param aParentId Parent id for the entity being generated. 
     *                  Set to KNullDesC8 if no parent.
     */
//     virtual void StartL( const Xml::RTagInfo& aElement, 
//                          const Xml::RAttributeArray& aAttributes, 
//                          TInt aErrorCode,
//                          const TDesC8& aParentId );

public:

    /**
     * Retrieves the finished purchase option object. Ownership is transferred.
     * Valid when the parser is finished.
     * @return Purchase option data or 0.
     */
    MNcdPreminetProtocolPurchaseInformation* Information();



private:
    CNcdPreminetProtocolPurchaseInformationParser( 
        MNcdParserObserverBundle& aObservers, 
        MNcdSubParserObserver& aSubParserObserver,
        TInt aDepth );
    
    void ConstructL(const Xml::RTagInfo& aElement,
                    const Xml::RAttributeArray& aAttributes,
                    const TDesC& aParentId );

protected:
    virtual void OnStartElementL( const Xml::RTagInfo& aElement, 
                                  const Xml::RAttributeArray& aAttributes, 
                                  TInt aErrorCode );

    virtual void OnEndElementL( const Xml::RTagInfo& aElement, 
                                TInt aErrorCode );

    virtual void SubParserFinishedL( const TDesC8& aTag, TInt aErrorCode );

protected:
    /**
     * The data object being built
     */
    CNcdPreminetProtocolPurchaseInformationImpl* iInformation;
    
    };

#endif //NCD_PREMINET_PROTOCOL_PURCHASE_PARSER_H
