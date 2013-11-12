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
#ifndef TSSERVICEOBSERVER_H
#define TSSERVICEOBSERVER_H
#include <QObject>

#include "tsdataobserver.h"
class  TsServiceObserver: public QObject
{
    Q_OBJECT

public:
    TsServiceObserver(MTsDataObserver& observer);

public slots:
    void dataChanged();

private:
    MTsDataObserver &mObserver;
};
#endif //TSSERVICEOBSERVER_H
