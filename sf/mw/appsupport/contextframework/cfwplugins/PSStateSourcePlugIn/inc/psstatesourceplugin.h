/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPSStateSourcePlugIn class declaration.
*
*/



#ifndef C_PSSTATESOURCEPLUGIN_H
#define C_PSSTATESOURCEPLUGIN_H

#include <badesca.h>
#include <cfcontextsourceplugin.h>
#include <cfcontextsourcesetting.h>

class CPSStateMapper;
class CPSStateMapperParams;
class CPSStateMapperParamsInt;
class CCFContextSourceSettingArray;

/**
* P&S state source plug-in.
* CPSStateSourcePlugIn is responsible for mapping defined P&S keys
* into contexts.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CPSStateSourcePlugIn ):
    public CCFContextSourcePlugIn
    {
    public:
    
        // Two phased constructors
        static CPSStateSourcePlugIn* NewL( 
        	TContextSourceInitParams* aParams );
        static CPSStateSourcePlugIn* NewLC( 
        	TContextSourceInitParams* aParams );
            
        // Destructor
        ~CPSStateSourcePlugIn();
    
    public: // From base classes
    
        // @see CCFContextSourcePlugIn
    	void HandleSettingL(
             CCFContextSourceSettingArray* aSettingList );
            
        // @see CCFContextSourcePlugIn
    	void DefineContextsL();
    	
        // @see CCFContextSourcePlugIn
    	void InitializeL();
    	
    private: // New methods
        
        TInt ConvertHexToTUint( const TDesC& aString, TUint32& aValue );
    	
    	void ParseIntMapperParamsL(CPSStateMapper& aMapper,
			const RSettingParameterArray& aParameters);
			
		void ParseTextMapperParamsL(CPSStateMapper& aMapper,
			const RSettingParameterArray& aParameters);
    	
    	void ConvertDescToSecurityPolicy( const TDesC& aString, 
    		TSecurityPolicy& aSecurityPolicy );
    		
    	TCapability ConvertToCapabilityNumber( const TDesC& aString );
    	
    private:
    
        CPSStateSourcePlugIn( TContextSourceInitParams* aParams );
        
    private: // Data
    
        // Own: P&S state mappers
        RPointerArray<CPSStateMapper> iMappers;
        
        // My settings
         CCFContextSourceSettingArray* iSettings;
    };

#endif
