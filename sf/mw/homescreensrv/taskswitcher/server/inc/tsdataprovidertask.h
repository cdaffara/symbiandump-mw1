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
#ifndef DATAPROVIDERTASK_H
#define DATAPROVIDERTASK_H

#include "tsdataprovider.h"

class TsDataProviderTask 
{
public:
    static void ExecuteL( const MTsDataProvider& aProvider, 
                          const RMessage2& aMsg);
};

#endif //DATAPROVIDER_H
