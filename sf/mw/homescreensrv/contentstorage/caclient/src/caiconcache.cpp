/*
 * Copyright (c)2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#include <QCoreApplication>
#include "caiconcache.h"
#include "caservice.h"
#include "caentry.h"
#include "caobjectadapter.h"
#include "caclienttest_global.h"

// Constants
static const int maxCost = 250;
static const QChar separator = '*';

QSharedPointer<CaIconCache> CaIconCache::mInstance(0);

/*!
 \class CaIconCache
 \brief This class provides icon caching mechanism
 Class implements singleton design patern. Use cache() method
 to get an instance of CaIconCache
 */

/*!
 \var CaIconCache::mCache
 Own.
 */

/*!
 \var CaIconCache::mService
 Not own.
 */

/*!
 \var CaIconCache::mNotifier
 Own.
 */

/*!
 Returns an instance of CaIconCache
 \retval CaIconCache instance

 */
CaIconCache *CaIconCache::cache()
{
    if (!mInstance.data()) {
        mInstance = QSharedPointer<CaIconCache>(new CaIconCache());
        mInstance->setParent(QCoreApplication::instance());
    }
    return mInstance.data();
}

/*!
Destructor
 */
CaIconCache::~CaIconCache()
{
    CACLIENTTEST_FUNC_ENTRY("CaIconCache::~CaIconCache");
    mCache.clear();
    delete mNotifier;
    CACLIENTTEST_FUNC_EXIT("CaIconCache::~CaIconCache");
}

/*!
Constructor
 */
CaIconCache::CaIconCache(QObject *parent): QObject(parent),
    mCache(), mService(CaService::instance()), mNotifier(0)
{
    mCache.setMaxCost(maxCost);
    CaNotifierFilter filter;
    mNotifier = mService->createNotifier(filter);
    connect(mNotifier, SIGNAL(entryChanged(const CaEntry &,ChangeType)),
            this, SLOT(remove(const CaEntry &,ChangeType)));

}

/*!
 Returns an icon from a cache
 \param entry an entry
 \param size size of an icon
 \retval icon

 */

HbIcon CaIconCache::icon(const CaEntry &entry, const QSizeF &size)
{
    CACLIENTTEST_FUNC_ENTRY("CaIconCache::icon");
    HbIcon result;
    if (HbIcon* tmp = mCache.object(key(entry,size))) {
        result = *tmp;
    }
    CACLIENTTEST_FUNC_EXIT("CaIconCache::icon");
    return result; 
}

/*!
 Insert an icon to a cache
 \param entry an entry
 \param size size of an icon
 \param icon icon to be cached

 */
void CaIconCache::insert(const CaEntry &entry, const QSizeF &size,
                         const HbIcon &icon)
{
    CACLIENTTEST_FUNC_ENTRY("CaIconCache::insert");
    mCache.insert(key(entry,size),new HbIcon(icon));
    CACLIENTTEST_FUNC_EXIT("CaIconCache::insert");
}

/*!
 Removes icon from a cache
 \param entry an entry
 \param changeTypa indicates if entry was updated, removed or added
 */
void CaIconCache::remove(const CaEntry &entry, ChangeType changeType)
{
    CACLIENTTEST_FUNC_ENTRY("CaIconCache::remove");
    if (changeType != AddChangeType && !(entry.flags() & UninstallEntryFlag)) {
        QString entryKey = key(entry);
        entryKey.append(separator);
        QList<QString> keys = mCache.keys();
        foreach(QString cacheKey,keys) {
            if (cacheKey.contains(entryKey)) {
                mCache.remove(cacheKey);
            }
        }
    }
    CACLIENTTEST_FUNC_EXIT("CaIconCache::remove");
}

/*!
 Generates a key
 \param entry an entry
 \return key
 */

QString CaIconCache::key(const CaEntry &entry, const QSizeF &size)
{
    QString key;
    if (!entry.iconDescription().skinId().isEmpty()) {
        key.append(entry.iconDescription().skinId());
        key.append(separator);
        key.append(entry.entryTypeName());
    } else if (!entry.iconDescription().filename().isEmpty()) {
        key.append(entry.iconDescription().filename());
        key.append(separator);
        key.append(entry.entryTypeName());
    } else {
        key.append(separator);
        key.append(entry.id());
    }
    key.append(separator);
    if (size.isValid()) {
        key += QString::number(size.height());
        key.append(separator);
        key += QString::number(size.width());
    }
    return key;
}
