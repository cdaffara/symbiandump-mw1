/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <e32base.h>
#include "tstaskmonitorglobals.h"

class MTsDataStorage {
public:
    virtual TBool IsSupported(TInt aFunction) const =0;
    virtual void HandleDataL(TInt aFunction, RReadStream& aDataStream) =0;
};

#endif //DATASTORAGE_H
