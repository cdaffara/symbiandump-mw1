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
* Description:  HsWidget factory.
*
*/


#ifndef HSWIDGETFACTORY_H
#define HSWIDGETFACTORY_H

#include <QObject>
#include <hswidgetmodel_global.h>
#include <hsiwidgetprovider.h>


class QPluginLoader;
class HsWidgetFactoryPrivate;

class HSWIDGETMODEL_EXPORT HsPluginUnloader: public QObject
{
    Q_OBJECT

public:

    HsPluginUnloader(QPluginLoader *pluginLoader, QObject *parent = 0);
    ~HsPluginUnloader();

private:
    QPluginLoader *mPluginLoader;

};



class HSWIDGETMODEL_EXPORT HsWidgetFactory : public QObject
{
    Q_OBJECT

public:

    HsWidgetFactory(QObject *parent = 0);
    virtual ~HsWidgetFactory();

    HsWidget* createWidget(const HsWidgetToken &token);

private:

    Q_DISABLE_COPY(HsWidgetFactory)

    HsWidgetFactoryPrivate *mD;
    friend class HsWidgetFactoryPrivate;

};




#endif
