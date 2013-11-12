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
 * Description: caentry_p.h
 *
 */

#ifndef CAENTRY_PRIVATE_H
#define CAENTRY_PRIVATE_H

#include <QString>
#include <QPixmap>
#include <QFlags>
#include <QMap>
#include <QSharedData>

#include "cadefs.h"
#include "caicondescription.h"

class CaEntry;
class HbIcon;

class CaEntryPrivate: public QSharedData
{

public:

    explicit CaEntryPrivate(CaEntry *entryPublic);
    CaEntryPrivate &operator=(const CaEntryPrivate &entry);
    ~CaEntryPrivate();

    int id() const;
    void setId(int id);

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
    
    HbIcon makeIcon(const QSizeF &size) const;

    EntryRole role() const;
    void setRole(const EntryRole &role);
    
    bool isLocalized(LocalizationType localized) const;

private:

    CaEntry *const m_q;

    int mId;

    QString mText;

    QString mDescription;

    CaIconDescription mIconDescription;

    EntryFlags mFlags;

    QString mEntryTypeName;

    QMap<QString, QString> mAttributes;

    EntryRole mEntryRole;
    
    bool mTextLocalized;
    
    bool mDescriptionLocalized;
};

#endif // CAENTRY_PRIVATE_H
