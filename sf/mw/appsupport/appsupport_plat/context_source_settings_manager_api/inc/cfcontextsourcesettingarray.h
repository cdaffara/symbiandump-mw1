/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingArray class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGARRAY_H
#define C_CFCONTEXTSOURCESETTINGARRAY_H


#include <e32base.h>

class CCFContextSourceSetting;

/**
*  Describes context source setting array.
*  Context source setting array consists of CCFContextSourceSetting
*  object instances.
*
*  @see CCFContextSourceSettingsManager
*
*  @lib CCFContextSourceSettingsManager.lib
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingArray ): public CBase
    {
    public:
    
        // Two phased constructors
        IMPORT_C static CCFContextSourceSettingArray* NewL();
        IMPORT_C static CCFContextSourceSettingArray* NewLC();
    
    public: // New functions
    
        /**
        * Returns number of setting items found in the array.
        * 
        * @since S60 5.0
        * @param None.
        * @return TInt.
        */
        virtual TInt Count() const = 0;

        /**
        * Returns settings item.
        * 
        * @since S60 5.0
        * @param aIndex Index of the setting item.
        * @return const CCFContextSourceSetting&
        */
        virtual const CCFContextSourceSetting& Item( TInt aIndex ) const = 0;

        /**
        * Appends a new setting item in the array.
        *
        * @since S60 5.0
        * @param aItem Setting item to append.
        * @return None.
        */
        virtual void AppendItemL( CCFContextSourceSetting* aItem ) = 0;
    };

#endif
