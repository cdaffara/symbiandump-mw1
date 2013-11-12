/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *  Description : Content Arsenal client session
 *
 */

#ifndef CAOBSERVER_H_
#define CAOBSERVER_H_

#include <QObject>

#include "cadef.h"
#include "cadefs.h"


class CCaInnerEntry;
class CaEntry;

/**
 *  IDataObserver
 *
 */
class IDataObserver
{
public:
    /**
     * Method invoked when entry is changed.
     * @param entryId entry id.
     * @param changeType change type.
     */
    virtual void entryChanged(TInt entryId,
                              TChangeType changeType) const = 0;

    /**
     * Method invoked when entry is changed.
     * @param entry entry.
     * @param changeType change type.
     */
    virtual void entryChanged(const CCaInnerEntry &entry,
                              TChangeType changeType) const = 0;

    /**
     * Method invoked when entry was changed.
     * @param entryId entry id.
     */
    virtual void entryTouched(TInt id) const = 0;

    /**
     * Method invoked when entry was changed.
     * @param groupId group id.
     */
    virtual void groupContentChanged(TInt groupId) const = 0;
};



class CaObserver: public QObject, public IDataObserver
{
    Q_OBJECT
public:
    /**
     * Constructor.
     * @param parent pointer to parent. Defaults to NULL.
     */
    CaObserver(QObject *parent = 0);
    
    /**
     * Method invoked when entry is changed.
     * @param entryId entry id.
     * @param changeType change type.
     */
    void entryChanged(TInt entryId,
                              TChangeType changeType) const;

    /**
     * Method invoked when entry is changed.
     * @param entry entry.
     * @param changeType change type.
     */
    void entryChanged(const CCaInnerEntry &entry,
                              TChangeType changeType) const;

    /**
     * Method invoked when entry was changed.
     * @param entryId entry id.
     */
    void entryTouched(TInt id) const ;

    /**
     * Method invoked when entry was changed.
     * @param groupId group id.
     */
    void groupContentChanged(TInt groupId) const;

signals:

    void signalEntryChanged(int entryId, ChangeType changeType) const;
    void signalEntryChanged(const CaEntry &entry,
                            ChangeType changeType) const;
    void signalEntryTouched(int id) const;
    void signalGroupContentChanged(int groupId) const;
};


#endif /* CAOBSERVER_H_ */
