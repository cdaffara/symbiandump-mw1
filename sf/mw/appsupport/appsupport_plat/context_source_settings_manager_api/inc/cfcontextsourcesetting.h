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
* Description:  CFContextSourceSetting class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTING_H
#define C_CFCONTEXTSOURCESETTING_H


#include <e32base.h>
#include <cfkeyvaluepair.h>
#include <cfcontextsourcesettingparameter.h>

/**
*  Describes context source setting item.
*  Setting item is defined with name, attributes and set of parameters.
*
*  @see CCFContextSourceSettingsManager
*
*  @lib CFContextSourceSettingsManager.lib
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSourceSetting ): public CBase
    {
    public: // New functions
    
        /**
        * Returns name of the setting.
        * Can be used to identify the user of these settings.
        * e.g. context source, plug-in name etc.
        * 
        * @since S60 5.0
        * @param None.
        * @return const TDesC&
        */
        virtual const TDesC& Name() const = 0;
        
        /**
        * Returns general attributes defined for the setting.
        * 
        * @since S60 5.0
        * @param None.
        * @return const RKeyValueArray&
        */
        virtual const RKeyValueArray& Attributes() const = 0;
        
        /**
        * Returns all defined parameters for the setting.
        * 
        * @since S60 5.0
        * @param None.
        * @return const RSettingParameterArray&
        */
        virtual const RSettingParameterArray& Parameters() const = 0;

        /**
        * Sets name for the settings.
        * 
        * @since S60 5.0
        * @param aName Name for the setting.
        * @return None
        */
        virtual void SetNameL( const TDesC& aName ) = 0;
        
        /**
        * Adds a new attribute for the setting.
        * 
        * @since S60 5.0
        * @param aAttribute New attribute in key value pair format.
        * @return None
        */
        virtual void AddAttributeL( CCFKeyValuePair* aAttribute ) = 0;

        /**
        * Adds a new parameter for the setting.
        * 
        * @since S60 5.0
        * @param aParameter New parameter in key value pair format.
        * @return None
        */
        virtual void AddParameterL( CCFContextSourceSettingParameter* aParameter ) = 0;
    };

#endif
