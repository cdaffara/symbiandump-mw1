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
* Description:  HsWidget provider manifest.
*
*/

#include <QDomDocument>
#include <QFile>

#include "hswidgetprovidermanifest.h"
#include "hswidgetprovidermanifest_p.h"
#include "hstest_global.h"


/*DEPRECATED
    \class HsWidgetProviderManifest
    \ingroup group_hswidgetmodel
    \brief Loads home screen widget tokens from an XML manifest file.

    Used by the HsWidgetFactory for loading widget tokens from an XML
    manifest file.
 
*/


HsWidgetProviderManifestPrivate::HsWidgetProviderManifestPrivate(
    HsWidgetProviderManifest *handle):
    mPublic(handle),
	mLoadOnQuery(false)
{
}


HsWidgetProviderManifestPrivate::~HsWidgetProviderManifestPrivate()
{

}

QList<HsWidgetToken> HsWidgetProviderManifestPrivate::widgets() const
{
    return mWidgetTokens;
}

bool HsWidgetProviderManifestPrivate::loadFromXml(const QString &fileName)
{
    mWidgetTokens.clear();

    QFile file(fileName);
    if (!file.exists()) {
       return false;
    }

    QDomDocument document;
    if (!document.setContent(&file)) {
        return false;
    }

    QDomElement element = document.documentElement();
    if (element.tagName() != "widgetprovider") {
        return false;
    }
	
    mLoadOnQuery = false;
    QDomAttr attribute = element.attributeNode("loadonquery");
	if (attribute.value().toLower() == "true") {
        mLoadOnQuery = true;
	}

    QList<HsWidgetToken> tokens;
    HsWidgetToken token;

    QDomNodeList widgets = element.elementsByTagName("widget");
	
	bool required(true);

    if (mLoadOnQuery) {
		required = false;
	}

    for(int i = 0; i < widgets.count(); ++i) {
        element = widgets.at(i).toElement();
        token.mLibrary = parseAttribute(element, "library");
        token.mUri = parseAttribute(element, "uri");
        token.mTitle = parseAttribute(element, "title", required);
        token.mIconUri = parseAttribute(element, "iconuri", false);
        token.mDescription = parseAttribute(element, "description");
        tokens << token;
    }

    mWidgetTokens = tokens;
    
    return true;
}

QString HsWidgetProviderManifestPrivate::parseAttribute(
	QDomElement &element, 
	const QString &attributeName, 
	bool isRequired) const
{
    QDomAttr attribute = element.attributeNode(attributeName);
    if (attribute.isNull() || attribute.value().isEmpty()) {
        if (isRequired) {
            HSDEBUG("Required attribute missing")
			return QString();
        }
        else {
            return QString();
        }
    }
    return attribute.value();
}

/*!
    \fn HsWidgetProviderManifest::HsWidgetProviderManifest(QObject* aParent)

    Constructor
*/
HsWidgetProviderManifest::HsWidgetProviderManifest(QObject *parent)
    : QObject(parent),
    mD(new HsWidgetProviderManifestPrivate(this))
{
}

/*!
    \fn HsWidgetProviderManifest::~HsWidgetProviderManifest()

    Destructor
*/
HsWidgetProviderManifest::~HsWidgetProviderManifest()
{
    delete mD;
}

/*!
    \fn QList<HsWidgetToken> HsWidgetProviderManifest::widgets() const

    Returns widget tokens found from manifest
*/
QList<HsWidgetToken> HsWidgetProviderManifest::widgets() const
{
    return mD->widgets();
}

/*!
    \fn bool HsWidgetProviderManifest::loadOnQuery() const

    Returns whether widget tokens should be quered dynamically
*/
bool HsWidgetProviderManifest::loadOnQuery() const
{
	return mD->mLoadOnQuery;
}

/*!
    \fn bool HsWidgetProviderManifest::loadFromXml(const QString& aFileName)

    Returns true if given \a fileName manifest parsing succeeded
*/
bool HsWidgetProviderManifest::loadFromXml(const QString &fileName)
{
    return mD->loadFromXml(fileName);
}

