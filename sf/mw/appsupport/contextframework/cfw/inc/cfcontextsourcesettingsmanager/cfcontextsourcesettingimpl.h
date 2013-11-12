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
* Description:  CFContextSourceSettingImpl class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGIMPL_H
#define C_CFCONTEXTSOURCESETTINGIMPL_H

#include "cfcontextsourcesetting.h"

#include <cfkeyvaluepair.h>

/**
*  Context source setting class implementation.
*
*  @lib -
*  @since S60 4.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingImpl ):
    public CCFContextSourceSetting
    {
    public:

        // Two phased constrcutors
        static CCFContextSourceSettingImpl* NewL();
        static CCFContextSourceSettingImpl* NewLC();

        // Destructor
        ~CCFContextSourceSettingImpl();
        
    public: // From base classes

        // @see CCFContextSourceSetting
        const TDesC& Name() const;
        
        // @see CCFContextSourceSetting
        const RKeyValueArray& Attributes() const;
        
        // @see CCFContextSourceSetting
        const RSettingParameterArray& Parameters() const;
        
        // @see CCFContextSourceSetting
        void SetNameL( const TDesC& aName );
        
        // @see CCFContextSourceSetting
        void AddAttributeL( CCFKeyValuePair* aAttribute );

        // @see CCFContextSourceSetting
        void AddParameterL( CCFContextSourceSettingParameter* aParameter );

    private:
    
        CCFContextSourceSettingImpl();
        void ConstructL();
        
    private: // Data
    
        // Own: Name tag
        HBufC* iName;
        
        // Own: Attributes
        RKeyValueArray iAttributes;
        
        // Own: Parameters
        RSettingParameterArray iParameters;
    };

#endif
