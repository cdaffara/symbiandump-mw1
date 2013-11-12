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
#ifndef TSSERVICESPROVIDERCONFIG_H
#define TSSERVICESPROVIDERCONFIG_H

#include <e32base.h>
class QObject;
class CTsServiceProviderConfig: public CBase
{
public:
    static CTsServiceProviderConfig* NewLC(QObject *aEnv);
    ~CTsServiceProviderConfig();

    TInt Count() const;
    QObject* LoadL(TInt aOffset)const;
private:
    CTsServiceProviderConfig(QObject* aEnv);
    void ConstructL();

private:
    QObject* iEnv;
    RPointerArray<TAny> iServices;
};

#endif //TSSERVICESPROVIDERCONFIG_H
