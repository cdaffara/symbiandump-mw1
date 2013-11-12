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
#ifndef TSSERVICESPROVIDER_H
#define TSSERVICESPROVIDER_H

#include "tsmodel.h"
#include "tsservicesproviderconfig.h"

class CTsService;
class MTsResourceManager;

class CTsServiceProvider: public CBase
{
public:
    static CTsServiceProvider* NewL( MTsResourceManager& aResources, 
                                     const CTsServiceProviderConfig& aConfig );
    ~CTsServiceProvider();
    MTsModel& operator[]( TInt aOffset ) const;
    TInt Count() const;

private:
    CTsServiceProvider();
    void Construct( MTsResourceManager& aResources,
                    const CTsServiceProviderConfig& aConfig );

private:
    RPointerArray<CTsService> iServices;
};
#endif //TSSERVICESPROVIDER_H
