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
* Description:   Import manager handles the reading, parsing and writing of
*                Brand data.
*
*/


#ifndef CBSIMPORTMANAGER_H
#define CBSIMPORTMANAGER_H

#include <e32base.h>
#include <f32file.h>

class CMDXMLDocument;
class MDesCArray;

/**
 *  Import manager handles the reading, parsing and writing of
 *  Brand data.
 *
 *  @lib brandimporter.exe
 *  @since S60 v3.2
 */
class CBSImportManager : public CBase
{
public:

    static CBSImportManager* NewL();
    
    static CBSImportManager* NewLC();
    
    ~CBSImportManager();

    /**
     * Import XML file and write binary brand file from it.
     * Overloaded function takes multiple xml files.
     *
     * @since S60 v3.2
     * @param aSrcFile source xml file
     * @return none
     */
    void ImportFileL( const TDesC& aSrcFile );
    void ImportFileL( const MDesCArray& aSrcFiles );

private:

    void ConstructL();
    
    CBSImportManager();

private: // data
        
    // Own. Filesystem handle
    RFs iFs;
    
    // Own. Parsed source XML document
    CMDXMLDocument* iDocument;
};

#endif //CBSIMPORTMANAGER_H
