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
* Description:  CFContextSourceSettingParameterImpl class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGPARAMETERIMPL_H
#define C_CFCONTEXTSOURCESETTINGPARAMETERIMPL_H


#include "cfcontextsourcesettingparameter.h"

/**
* Setting parameter implementation.
* 
* @lib CFContextSourceSettingParameterImplsManager.lib
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingParameterImpl ):
    public CCFContextSourceSettingParameter
    {
    public:
    
        // Two phased constructors
        static CCFContextSourceSettingParameterImpl* NewL();
        static CCFContextSourceSettingParameterImpl* NewLC();
        
        // Destructor
        ~CCFContextSourceSettingParameterImpl();
    
    public: // From base classes
    
        // @see CCFContextSourceSettingParameter
        const TDesC& Name() const;
        
        // @see CCFContextSourceSettingParameter
        const RKeyValueArray& Attributes() const;
        
        // @see CCFContextSourceSettingParameter
        void SetNameL( const TDesC& aName );
        
        // @see CCFContextSourceSettingParameter
        void AddAttributeL( CCFKeyValuePair* aAttribute );

    private:
    
        CCFContextSourceSettingParameterImpl();
        void ConstructL();
        
    private: // Data
    
        // Own: Name
        HBufC* iName;
        
        // Own: Paramters
        RKeyValueArray iAttributes;
    };
    
#endif
