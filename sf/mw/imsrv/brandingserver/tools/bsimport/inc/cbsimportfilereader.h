/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Import reader opens and reads the XML file to CMDXMLDocument 
*                object
*
*/


#ifndef CBSIMPORTFILEREADER_H
#define CBSIMPORTFILEREADER_H

#include <e32base.h>
#include <gmxmlparser.h>

class CMDXMLDocument;
class CActiveScheduler;

/**
 *  Import reader opens and reads the XML file to CMDXMLDocument object
 *
 *  @lib brandimporter.exe
 *  @since S60 v3.2
 */
class CBSImportFileReader : public CBase,
                            public MMDXMLParserObserver
    {
public:

    static CBSImportFileReader* NewL( TBool aUseScheduler );
    
    static CBSImportFileReader* NewLC( TBool aUseScheduler );
    
    ~CBSImportFileReader();

    /**
     * Reads XML file to CMDXMLDocument object
     *
     * @since S60 v3.2
     * @param aFs file system handle
     * @param aFileName XML file to read
     */
    CMDXMLDocument* ReadXMLFileL( RFs& aFs, const TDesC& aFileName );
    
private:

    void ConstructL();

    CBSImportFileReader( TBool aUseScheduler );

// from base class MMDXMLParserObserver

    /**
     * From MMDXMLParserObserver.
     * Writes text to log
     *
     * @since S60 v3.2
     * @param aText text to write
     */
    void ParseFileCompleteL();
            
private: // Data
    
    // Own. XML parser
    CMDXMLParser* iParser;
    
    // Own. XML document from parser
    CMDXMLDocument* iDocument;

    // Own. ActiveScheduler
    CActiveScheduler* iScheduler;

    // Own. Scheduler waiter. Used when we can't use scheduler directly.    
    CActiveSchedulerWait* iWaiter;
    
    // Wheter we use CActiveScheduler or CActiveSchedulerWait
    TBool iUseScheduler;
    };

#endif //CBSIMPORTFILEREADER_H
