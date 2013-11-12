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
 * Description:
 *
 */

#ifndef C_CPSTORAGE_FACTORY_H
#define C_CPSTORAGE_FACTORY_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CCaStorage;

// CLASS DECLARATION
/**
 *  @ingroup group_catorage.
 *  Class with one static method used by CaStorage to
 *  instantiate storage database.
 */
class CaStorageFactory
    {
public:
    /**
     * Factory method to create CCaStorage.
     *
     * @return Instance of created storage.
     */
    IMPORT_C static CCaStorage* NewDatabaseL();
    };

#endif //C_CPSTORAGE_FACTORY_H
