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
* Description:  CCenRepAttributesNonMasked class declaration.
*
*/



#ifndef C_CENREPATTRIBUTESNONMASKED_H
#define C_CENREPATTRIBUTESNONMASKED_H

#include "cenrepattributes.h"

/**
 *  Handles non masked attributes
 *
 *  @lib CenRepSourcePlugIn.dll
 *  @since S60 5.0
 */

NONSHARABLE_CLASS( CCenRepAttributesNonMasked ) : public CCenRepAttributes
    {

public:

    /**
     * Two-phased constructors.
     *
     * @since S60 5.0
     * @param aCF Interface for contexts
     * @param aSettings Sensor source settings
     */
    static CCenRepAttributesNonMasked* NewL(
               MCFContextInterface& aCF,
               CCFContextSourceSettingArray& aSettings );

    static CCenRepAttributesNonMasked* NewLC(
               MCFContextInterface& aCF,
               CCFContextSourceSettingArray& aSettings );

    virtual ~CCenRepAttributesNonMasked();

// from base class CenRepAttributes

    /**
    * From CenRepAttributes
    * Search context values from parameter array and updates context
    *
    * @since S60 5.0
    * @param aKeyValue Parameter key value for context values to search
    * @return void
    */
    void UpdateContextL( const TInt aKeyValue );

protected:

private:

    CCenRepAttributesNonMasked( MCFContextInterface& aCF,
                                CCFContextSourceSettingArray& aSettings );
    void ConstructL();

public:    // Data

protected: // Data

private:   // Data

    };

#endif // C_CENREPATTRIBUTESNONMASKED_H
