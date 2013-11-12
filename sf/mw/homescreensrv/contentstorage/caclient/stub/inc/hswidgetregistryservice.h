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
* Description:  Manages installed widget's information
*
*/

#ifndef HSWIDGETREGISTRYSERVICE_H
#define HSWIDGETREGISTRYSERVICE_H

#include <QObject>
#include <QSharedPointer>

#include "hswidgetcomponentdescriptor.h"

class HsWidgetRegistryServicePrivate;


class HsWidgetRegistryService : public QObject
{
    Q_OBJECT

public:

    HsWidgetRegistryService(const QString &installationPath,
                            QObject *parent = 0);

    virtual ~HsWidgetRegistryService();

    QList<HsWidgetComponentDescriptor> widgets();

signals:

    void widgetAddedToRegistry(const QList<HsWidgetComponentDescriptor> &widgetTokenList);

    void widgetRemovedFromRegistry(int uid);

private:

    Q_DISABLE_COPY(HsWidgetRegistryService)

private:

    void emitWidgetAddedToRegistry(const QList<HsWidgetComponentDescriptor> &widgetsAdded);

    void emitWidgetRemovedFromRegistry(int uid);

private:

    QSharedPointer<HsWidgetRegistryServicePrivate> mPrivate;

    friend class HsWidgetRegistryServicePrivate;
};

Q_DECLARE_METATYPE(HsWidgetRegistryService *)


#endif
