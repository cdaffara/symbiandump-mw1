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

#ifndef ICONCACHE_H
#define ICONCACHE_H

// System includes
#include <QCache>
#include <HbIcon>

// User includes
#include "canotifier.h"

// Forward declarations
class CaEntry;
class CaService;

// Class declaration
class CaIconCache: public QObject
{
    Q_OBJECT
public:

    static CaIconCache *cache();
    HbIcon icon(const CaEntry &entry, const QSizeF &size);
    void insert(const CaEntry &entry, const QSizeF &size, const HbIcon &icon);

private:

    explicit CaIconCache(QObject *parent = 0);
    ~CaIconCache();
    QString key(const CaEntry &entry, const QSizeF &size = QSizeF());

private slots:

    void remove(const CaEntry &entry, ChangeType changeType);

private:
    static QSharedPointer<CaIconCache> mInstance;

    QCache<QString,HbIcon> mCache;

    QSharedPointer<CaService> mService;

    CaNotifier *mNotifier;

    friend class QtSharedPointer::ExternalRefCount<CaIconCache>;
};

#endif // ICONCACHE_H
