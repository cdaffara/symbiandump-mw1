/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFEComPluginManager class declaration.
*
*/


#ifndef C_CFECOMPLUGINMANAGER_H
#define C_CFECOMPLUGINMANAGER_H

#include <e32base.h>
#include <ecom/implementationinformation.h>

/**
*
* @lib CFServices.lib
* @since S60 5.0
*/
class CCFEComPluginManager: public CBase
    {
    public:

        // Destructior
        IMPORT_C virtual ~CCFEComPluginManager();

    protected: // New methods

        IMPORT_C TInt AddToBlackList( const TUid& aImplUid );
        IMPORT_C void RemoveBlackListed( RImplInfoPtrArray& aImplInfoArray );

    protected:

        // Constructor
        IMPORT_C CCFEComPluginManager();

    private: // Data

        // List of implementation UIDs of ECom plug-ins which failed to load
        // during boot.
        RArray<TUid> iUidArray;

    };

#endif // C_CFECOMPLUGINMANAGER_H
