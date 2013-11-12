/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingsManagerImpl class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGSMANAGERIMPL_H
#define C_CFCONTEXTSOURCESETTINGSMANAGERIMPL_H


#include "cfcontextsourcesettingsmanager.h"

#include <gmxmlparser.h>

class RFs;
class CMDXMLNode;
class CCFContextSourceSettingImpl;
class CCFContextSourceSettingArrayImpl;

/**
* Context source settings manager.
* Provides an interface through from a specified file can be parsed
* XML format context source settings.
* Returns settings fully parsed in an array.
* 
* @lib CFContextSourceSettingsManagerImpl.lib
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingsManagerImpl ):
    public CCFContextSourceSettingsManager,
    public MMDXMLParserObserver
    {
    public:

        // Two phased constrcutors
        IMPORT_C static CCFContextSourceSettingsManagerImpl* NewL( RFs& aFs );
        IMPORT_C static CCFContextSourceSettingsManagerImpl* NewLC( RFs& aFs );
        
        // Destructor
        ~CCFContextSourceSettingsManagerImpl();
        
    private: // From base classes

        // @see CCFContextSourceSettingsManager
        void ParseSettingsL( const TDesC& aSettingsFile,
            CCFContextSourceSettingArray& aSettings );

        // @see CCFContextSourceSettingsManager
        void ParseSettingsL( RFile& aSettingsFile,
                CCFContextSourceSettingArray& aSettings );

        // @see MMDXMLParserObserver
        void ParseFileCompleteL();
        
    private: // New methods
    
        // Parses setting block
        void ParseSettingBlockL( CMDXMLElement& aElement );
        
        // Parses attributes
        void ParseAttributesL( CMDXMLElement& aElement,
            CCFContextSourceSettingImpl& aSetting );
            
        // Parse parameters
        void ParseParametersL( CMDXMLElement& aElement,
            CCFContextSourceSettingImpl& aSetting );
        
    private:
    
        CCFContextSourceSettingsManagerImpl( RFs& aFs );
        void ConstructL();
        
    private: // Data
    
        // Own: Wait to synchronize async XML file parsing
        CActiveSchedulerWait* iWait;
        
        // Own: Fs Session
        RFs& iFs;
        
        // Own: XML parser
        CMDXMLParser* iParser;
        
        // Ref: Pointer to settings array to fill
        CCFContextSourceSettingArrayImpl* iSettings;
    };

#endif
