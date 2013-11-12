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
* Description:  HsWidget provider interface.
*
*/


#ifndef HSIWIDGETPROVIDER_H
#define HSIWIDGETPROVIDER_H

#include <QtPlugin>
#include <QList>
#include <QString>
#include <QVariant>
#include <QMetaType>


/*DEPRECATED
    \class HsWidgetToken
    \ingroup group_hswidgetmodel_api
    \brief Represents a home screen widget that can be created. 

    Home screen widgets are located in widget providers. Each provider
    exposes its contained widgets as widget tokens. A token has all
    the needed information for widget selection and creation.
*/
class HsWidgetToken
{

public:

/*!
    The name of the library/plugin that contains the widget.
*/
    QString mLibrary;

/*!
    Uniquely identifies the widget.
*/
    QString mUri;

/*!
    Human readable title for GUI purposes.
*/
    QString mTitle;

/*!
    Identifies widget's icon if it has one.
*/
    QString mIconUri;

/*!
    The description of the widget.
*/
    QString mDescription;

/*!
    User data  
*/
    QVariant mUserData;
    
/*!
    Uid of the widget
*/
    int mUid;
/*!
    HsWidget preferences.
*/
	QMap<QString,QString> mPreferences;
};

class HsWidget;

/*DEPRECATED
    \class IHsWidgetProvider
    \ingroup group_hswidgetmodel group_widget_api_candidates
    \brief Defines a mechanism for retrieving a home screen widget.
     
    Interface that defines a mechanism for retrieving a home screen
    widget. Each home screen widget provider implements this interface. 
*/
class IHsWidgetProvider
{

public:

/*!
    Destructor.
*/
    virtual ~IHsWidgetProvider() {}

public:

/*!
    Returns contained widgets as a list of widget tokens.
*/
    virtual QList<HsWidgetToken> widgets() = 0;

/*!
    Creates and returns a widget based on the given \a token.
*/
    virtual HsWidget *createWidget(const HsWidgetToken &token) = 0;

};

Q_DECLARE_METATYPE(HsWidgetToken)
Q_DECLARE_INTERFACE(IHsWidgetProvider, "com.nokia.homescreen.iwidgetprovider/1.0")

#endif
