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
* Description:  CFContextSourceSettingsManager class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGSMANAGER_H
#define C_CFCONTEXTSOURCESETTINGSMANAGER_H


#include <e32base.h>

class RFs;
class RFile;
class CCFContextSourceSettingArray;

/**
* Context source settings manager.
* Provides an interface through from a specified file can be parsed
* XML format context source settings.
* Returns settings fully parsed in an array.
*
* Context source setting is defined from following XML:
* <ContextSourceSettings>
*   <SettingName attribute1="value1">
*     <ParamName param1="value1"/>
*     <ParamName param1="value1" param2="value2"/>
*   </SettingName>
* </ContextSourceSettings>
*
* <ContextSourceSettings>: Mandatory tag which identifies this XML block
* as context source settings.
*
* <SettingName>: Defines a name for the setting item. Can be name
* of the context source. All general attributes for this
* specific setting are set as attributes.
*
* <ParamName>: Defines a parameter for this setting item.
* Parameter count is not restricted.
*
* Below is few examples:
* <ContextSourceSettings>
*   <CenRepKey Category="0x12345678" Key="0x00000001">
*     <Context KeyValue="0" Source="Source" Type="Type" Value="Off"/>
*     <Context KeyValue="1" Source="Source" Type="Type" Value="On"/>
*   </CenRepKey>
* </ContextSourceSettings>
*
* <ContextSourceSettings>
*   <SomeSetting Param1="Value1" Param2="Value2"/>
* </ContextSourceSettings>
*
* @lib CFContextSourceSettingsManager.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingsManager ): public CBase
    {
    public:

        // Two phased constrcutors
        IMPORT_C static CCFContextSourceSettingsManager* NewL( RFs& aFs );
        IMPORT_C static CCFContextSourceSettingsManager* NewLC( RFs& aFs );
        
    public: // New functions
    
        /**
        * Parses settings from a specified setting file.
        * Result is given in an array.
        * Array is not emptied. All found setting items will be appended
        * in the end of the array.
        * 
        * @since S60 5.0
        * @param aSettingsFile File to parse.
        * @param aSettings Found and parsed settings.
        * @return None
        */
        virtual void ParseSettingsL( const TDesC& aSettingsFile,
            CCFContextSourceSettingArray& aSettings ) = 0;

        /**
        * Parses settings using an open file handle.
        * Result is given in an array.
        * Array is not emptied. All found setting items will be appended
        * in the end of the array.
        * 
        * @since S60 5.0
        * @param aSettingsFile File to parse. The settings manager takes
        *   ownership of the open file handle and will close handle when
        *   completed.
        * @param aSettings Found and parsed settings.
        * @return None
        */
        virtual void ParseSettingsL( RFile& aSettingsFile,
            CCFContextSourceSettingArray& aSettings ) = 0;
    };

#endif
