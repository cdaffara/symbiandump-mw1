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

#ifndef CTSBACKSTEPPINGACTIVACTION_H
#define CTSBACKSTEPPINGACTIVACTION_H

#include <e32base.h>
#include <e32property.h>

class MTsWindowGroupsMonitor;


class CTsBacksteppingActivation : public CActive
    {
public:
    IMPORT_C static CTsBacksteppingActivation* NewL(
                                            MTsWindowGroupsMonitor &aMonitor);
    IMPORT_C virtual ~CTsBacksteppingActivation();

protected:
    void RunL();
    TInt RunError(TInt);
    void DoCancel();  

private:
    CTsBacksteppingActivation(MTsWindowGroupsMonitor &aMonitor);
    void ConstructL();
    void Subscribe();

private:
    MTsWindowGroupsMonitor &iMonitor;
    RProperty iProperty;
    CBase* iEngine;
    };

#endif /* CTSBACKSTEPPINGACTIVACTION_H*/
