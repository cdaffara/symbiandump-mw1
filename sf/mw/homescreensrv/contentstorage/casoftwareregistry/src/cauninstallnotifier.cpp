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
 * Description: canotifier.cpp
 *
 */


#include <QCoreApplication>
#include "cauninstallnotifier.h"
#include "cauninstallnotifier_p.h"

QSharedPointer<CaUninstallNotifier> CaUninstallNotifier::mInstance(0);

/*!
 Constructor.
 \param entryPrivate pointer to private implementation.
 */
CaUninstallNotifier::CaUninstallNotifier(CaUninstallNotifierPrivate *const notifierPrivate) :
    QObject(0), m_d(notifierPrivate)
{
    m_d->m_q = this;
    m_d->makeConnect();
}

/*!
 Destructor.
 */
CaUninstallNotifier::~CaUninstallNotifier()
{
    delete m_d;
}

CaUninstallNotifier* CaUninstallNotifier::notifier()
{
    if (!mInstance.data()) {
        mInstance = QSharedPointer<CaUninstallNotifier>(
                new CaUninstallNotifier(new CaUninstallNotifierPrivate()));
        mInstance->setParent(QCoreApplication::instance());
    }
    return mInstance.data();
}


