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
* Description:  CFContextSourceSettingArrayImpl class declaration.
*
*/



#ifndef C_CFCONTEXTSOURCESETTINGARRAYIMPL_H
#define C_CFCONTEXTSOURCESETTINGARRAYIMPL_H


#include "cfcontextsourcesettingarray.h"

/**
*  Context source setting array implementation.
*
*  @see CCFContextSourceSettingsManager
*
*  @lib CCFContextSourceSettingsManager.lib
*  @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSourceSettingArrayImpl ): public CCFContextSourceSettingArray
    {
    public:
    
        // Two phased constructors
        static CCFContextSourceSettingArrayImpl* NewL();
        static CCFContextSourceSettingArrayImpl* NewLC();
        
        // Destructor
        ~CCFContextSourceSettingArrayImpl();
    
    public: // From base classes

        // @see CCFContextSourceSettingArray
        TInt Count() const;

        // @see CCFContextSourceSettingArray
        const CCFContextSourceSetting& Item( TInt aIndex ) const;
        
        // @see CCFContextSourceSettingArray
        void AppendItemL( CCFContextSourceSetting* aItem );
        
    private:
    
        CCFContextSourceSettingArrayImpl();
        void ConstructL();
        
    private: // Data
    
        /** Pointer array of setting items */
        RPointerArray<CCFContextSourceSetting> iItems;
    };

#endif
