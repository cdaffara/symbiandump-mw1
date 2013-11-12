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
#ifndef TSTASK_H
#define TSTASK_H

#include <QPixmap>
#include <QString>
#include <QSharedPointer>

#include "tstaskmonitor_global.h"

class TsTaskContent;
class TsTaskLauncher;

class TsTask
{
    
public:
    TsTask(const QSharedPointer<TsTaskContent> &content, TsTaskLauncher &launcher);
    TSTASKMONITOR_EXPORT ~TsTask();
    
    TSTASKMONITOR_EXPORT void open();
    TSTASKMONITOR_EXPORT void close();
    
    TSTASKMONITOR_EXPORT bool isClosable() const;
    TSTASKMONITOR_EXPORT bool isActive() const;
    TSTASKMONITOR_EXPORT QPixmap screenshot() const;
    TSTASKMONITOR_EXPORT QString name() const;
    
private:
    Q_DISABLE_COPY(TsTask)
    
    QSharedPointer<TsTaskContent> mContent;
    TsTaskLauncher &mLauncher;

};

#endif //TSTASK_H
