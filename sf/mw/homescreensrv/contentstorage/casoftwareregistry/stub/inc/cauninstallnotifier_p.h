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
 * Description: canotifier.h
 *
 */

#ifndef CAUNINSTALLNOTIFIER_P_H
#define CAUNINSTALLNOTIFIER_P_H

#include <QObject>
#include <QSharedPointer>
class CaUninstallObserver;
class CaUninstallNotifier;
class CaSoftwareRegistry;

class CaUninstallNotifierPrivate: public QObject
{
Q_OBJECT

public:

    explicit CaUninstallNotifierPrivate();

    virtual ~CaUninstallNotifierPrivate();
    
    void makeConnect();

private:
    CaUninstallObserver* mUninstallObserver;
    QSharedPointer<CaSoftwareRegistry> mCaSoftwareRegistry;
    
    const CaUninstallNotifier *m_q;
    friend class CaUninstallNotifier;

};


#endif // CAUNINSTALLNOTIFIER_P_H
