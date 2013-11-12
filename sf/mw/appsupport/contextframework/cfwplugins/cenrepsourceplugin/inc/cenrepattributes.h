/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCenRepAttributes class declaration.
*
*/


#ifndef C_CENREPATTRIBUTES_H
#define C_CENREPATTRIBUTES_H

#include <e32base.h>
#include <cfcontextinterface.h>
#include <cfcontextsourceplugin.h>

#include "cfcontextsourcesettingparameter.h"

class CCFContextSourceSettingArray;

_LIT( KUID, "UID" );
_LIT( KMode, "Mode" );
_LIT( KMasked, "Masked" );
_LIT( KNonMasked, "NonMasked" );

_LIT( KKey, "Key" );
_LIT( KValue, "Value" );
_LIT( KContextSource, "ContextSource" );
_LIT( KContextType, "ContextType" );
_LIT( KContextValue, "ContextValue" );
_LIT( KContextValueSet, "ContextValueSet" );
_LIT( KContextValueUnSet, "ContextValueUnSet" );

/**
 *  Base class for attribute and parameter handling
 *
 *  @lib CenRepSourcePlugIn.dll
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCenRepAttributes ) : public CBase
    {
public:

    virtual ~CCenRepAttributes();
    
public: // New methods

    /**
    * Pure virtual definition
    * Search context values from parameter array and updates context
    *
    * @since S60 5.0
    * @param aKeyValue Parameter key value for context values to search
    * @return void
    */
    virtual void UpdateContextL( const TInt aKeyValue ) = 0;
    
    /**
    * Converts string to hexadecimal.
    *
    * @since S60 5.0
    * @param aString The string to convert
    * @param aValue Converted value
    * @return KErrNotFound if not an int to be converted, else KErrNone
    */
    static TInt ConvertToHex( const TDesC& aString, TUint32& aValue );

    /**
    * Initializes object and triggers to define context.
    *
    * @since S60 5.0
    * @param aIndex Index of RContextSourceSettingArray item
    * @return Central repository key
    */
    TUint32 InitCenRepKeyL( TInt aIndex );

protected:

    CCenRepAttributes( MCFContextInterface& aCF,
                       CCFContextSourceSettingArray& aSettings );

    /**
    * Converts string to integer.
    *
    * @since S60 5.0
    * @param aString The string to convert
    * @param aValue Converted value
    * @return KErrNotFound if not an int to be converted, else KErrNone
    */
    TInt ConvertToInt( const TDesC& aString, TInt& aValue ) const;

    /**
    * Publish context
    *
    * @since S60 5.0
    * @param
    * @return void
    */
    void PublishContext();

private:

    void DefineContext();

public:    // Data

protected: // Data

        // Sensor source settings
        CCFContextSourceSettingArray& iSettings;

        // Ref: CF interface for contexts
        MCFContextInterface& iCF;

        // Own: Pointer to a Context object
        CCFContextObject* iContext;

        // Own: Parameter array of settings
        RSettingParameterArray iParamArray;

        // Parameter count in parameter array
        TInt iParamCount;

private:   // Data

    };

#endif // C_CENREPATTRIBUTES_H
