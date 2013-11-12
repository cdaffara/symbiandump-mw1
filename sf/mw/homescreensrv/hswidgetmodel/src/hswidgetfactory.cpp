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


#include <QStringList>
#include <QDir>
#include <QPluginLoader>

#include "hswidgetfactory.h"
#include "hswidgetfactory_p.h"
#include "hswidget.h"
#include "hswidgetprovidermanifest.h"
#include "hstest_global.h"



/*DEPRECATED
    \class HsPluginUnloader
    \ingroup group_hswidgetmodel
    \brief Unloads plugin and deletes plugin loader.

    Holds plugin loader instance and unloads and destroys plugin
    on it's destructor. 
 
*/


/*!
    \fn HsPluginUnloader::HsPluginUnloader(QPluginLoader *pluginLoader,QObject *parent)

    Constructs plugin unloader object. When this object is destroyed it 
    unloads it's pointed \a pluginLoader instance.
*/
HsPluginUnloader::HsPluginUnloader(QPluginLoader *pluginLoader,QObject *parent)
    :QObject(parent),mPluginLoader(pluginLoader)
{
}

/*!
    \fn HsPluginUnloader::~HsPluginUnloader()

    Unloads it's plugin loader instance.
*/
HsPluginUnloader::~HsPluginUnloader()
{
    if(mPluginLoader) {
        mPluginLoader->unload();
        delete mPluginLoader; 
    }
}

/*DEPRECATED
    \class HsWidgetFactory 
    \ingroup group_hswidgetmodel
    \brief Finds and creates home screen widgets.

    HsWidget factory creates an instance of
    a widget base on a widget token that is given to it.
 
*/


HsWidgetFactoryPrivate::HsWidgetFactoryPrivate(HsWidgetFactory *factory):
    mPublic(factory)
{

}


HsWidgetFactoryPrivate::~HsWidgetFactoryPrivate()
{

}



HsWidget* HsWidgetFactoryPrivate::createWidget(const HsWidgetToken &token)
{
   QPluginLoader* loader = new QPluginLoader(token.mLibrary);
   QObject* plugin = loader->instance();
   IHsWidgetProvider* provider = qobject_cast<IHsWidgetProvider*>(plugin);
   HsWidget* widget(0);
   
   if (provider) {
       widget = provider->createWidget(token);
       
       if (!widget) {
          HSDEBUG("Widget creation failed.")
          loader->unload();
          delete loader;
       }
       else {
           HsPluginUnloader* p = new HsPluginUnloader(loader);
           p->connect(widget,SIGNAL(destroyed()),SLOT(deleteLater()));
       }
    }

   else {
        HSDEBUG("Widget creation failed - No provider.")
        loader->unload();
        delete loader;
   }

   
    return widget;
}

/*!
    Constructor
*/
HsWidgetFactory::HsWidgetFactory(QObject *parent)
    : QObject(parent),mD(new HsWidgetFactoryPrivate(this))
{
}
/*!
    Destructor
*/
HsWidgetFactory::~HsWidgetFactory()
{
    delete mD;
}


/*!
    Creates and returns a widget based on given \a token
*/
HsWidget* HsWidgetFactory::createWidget(const HsWidgetToken &token)
{
    return mD->createWidget(token);
}


