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
 * Description: caentry.h
 *
 */

#ifndef CAENTRY_H
#define CAENTRY_H

#include <QMap>
#include <QSharedDataPointer>
#include <QSize>

#include "caclient_global.h"
#include "cadefs.h"
#include "caicondescription.h"

CA_CLIENT_TEST_CLASS( TestCaClient )

class CaObjectAdapter;
class CaEntryPrivate;
class HbIcon;

class CACLIENT_EXPORT CaEntry
{
    /*!
     * This class is not intended to be derived.
     */

public:

    explicit CaEntry(EntryRole entryRole = ItemEntryRole);

    virtual ~CaEntry();

    int id() const;

    QString text() const;
    void setText(const QString &text, bool localized = false);

    QString description() const;
    void setDescription(const QString &text, bool localized = false);
    
    CaIconDescription iconDescription() const;
    void setIconDescription(const CaIconDescription &iconDescription);

    EntryFlags flags() const;
    void setFlags(EntryFlags flags);

    QString entryTypeName() const;
    void setEntryTypeName(const QString &entryTypeName);

    QMap<QString, QString> attributes() const;
    QString attribute(const QString &name) const;
    void setAttribute(const QString &name, const QString &value);
    void removeAttribute(const QString &name);

    HbIcon makeIcon(const QSizeF &size = QSizeF(70.0, 70.0)) const;

    EntryRole role() const;

    CaEntry(const CaEntry &entry);
    CaEntry &operator=(const CaEntry &entry);

private:

    void setId(int id);
    bool isLocalized(LocalizationType localized) const; 

private:
    /*!
     * A special kind of pointer, supporting sharing data, to a private
     * implementation.
     */
    QSharedDataPointer<CaEntryPrivate> m_d;

    friend class CaEntryPrivate;
    friend class ::CaObjectAdapter;

    CA_CLIENT_TEST_FRIEND_CLASS( TestCaClient )

};


#endif // CAENTRY_H
