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
 * Description: caicondescription_p.h
 *
 */

#ifndef CAICONDESCRIPTION_PRIVATE_H
#define CAICONDESCRIPTION_PRIVATE_H

#include <qstring.h>
#include <QSharedData>

class CaIconDescription;

class CaIconDescriptionPrivate: public QSharedData
{
public:

    explicit CaIconDescriptionPrivate(
        CaIconDescription *iconDescriptionPublic);
    ~CaIconDescriptionPrivate();

    int id() const;
    void setId(int id);

    QString filename() const;
    void setFileName(const QString &fileName);

    QString skinId() const;
    void setSkinId(const QString &skinId);

    QString applicationId() const;
    void setApplicationId(const QString &applicationId);

private:

    CaIconDescription *const m_q;

    int mId;

    QString mFilename;

    QString mSkinId;

    QString mApplicationId;

};

#endif //CAICONDESCRIPTION_PRIVATE_H
