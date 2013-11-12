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
* Description:  CFContextSourceSettingParameter class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGPARAMETER_H
#define C_CFCONTEXTSOURCESETTINGPARAMETER_H


#include <e32base.h>
#include <cfkeyvaluepair.h>

/**
*  Describes context source setting parameter.
*  Context source setting parameter is defined with parameter name
*  and a set of attributes. Attributes are defined as key valeu pairs.
*
*  @see CCFContextSourceSettingManager
*
*  @lib CFContextSourceSettingsManager.lib
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingParameter ): public CBase
    {
    public: // New functions
    
        /**
        * Returns name of the parameter block,
        * 
        * @since S60 5.0
        * @param None.
        * @return const TDesC&
        */
        virtual const TDesC& Name() const = 0;
        
        /**
        * Returns all attributes defined for this parameter.
        * 
        * @since S60 5.0
        * @param None.
        * @return const RKeyValueArray&
        */
        virtual const RKeyValueArray& Attributes() const = 0;

        /**
        * Sets name for this parameter block.
        *
        * @since S60 5.0
        * @param aName Name for the parameter block.
        * @return None.
        */
        virtual void SetNameL( const TDesC& aName ) = 0;
        
        /**
        * Adds an attribute for the paramter block.
        *
        * @since S60 5.0
        * @param aAttribute Attribute to add.
        * @return None.
        */
        virtual void AddAttributeL( CCFKeyValuePair* aAttribute ) = 0;
    };
    
typedef RPointerArray<CCFContextSourceSettingParameter> RSettingParameterArray;

#endif
