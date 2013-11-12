/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 *
 */

#include "hswidgetcomponentparser.h"
#include <QFile>

const char WIDGETPROVIDER[] = "widgetprovider";
const char WIDGET[] = "widget";
const char WIDGETMANIFEST[] = "hswidgetmanifest";

const char URI[] = "uri";
const char TITLE[] = "title";
const char ICONURI[] = "iconuri";
const char ICON[] = "icon";
const char DESCRIPTION[] = "description";
const char HIDDEN[] = "hidden";
const char SERVICEXML[] = "servicexml";
const char PREVIEWIAMGE[] = "previewimage";
const char VERSION[] = "version";
const char VERSION_1_0[] = "1.0";
const char TRANSLATIONFILENAME[] = "translationfile";



HsWidgetComponentParser::HsWidgetComponentParser(const QString &aComponentManifest, QObject *parent)
    : QObject(parent),mError(false)
{
    parseFile(aComponentManifest);
}

HsWidgetComponentParser::~HsWidgetComponentParser()
{

}

HsWidgetComponentDescriptor HsWidgetComponentParser::widgetComponentDescriptor()
{
    return mComponentDescriptor;
}

bool HsWidgetComponentParser::error()
{
    return mError;
}

void HsWidgetComponentParser::parseFile(const QString &aComponentManifest)
{
    QFile file(aComponentManifest);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        mError = false;
        return;
    }
    mParser.setDevice(&file);
    if (mParser.readNextStartElement()) {
        if (mParser.name() == WIDGETMANIFEST){
             parseWidgetManifestTag();
        } else if(mParser.name() == WIDGETPROVIDER){
            parseWidgetProviderTag();
        } else {
            mParser.raiseError(QObject::tr("Malformed manifest"));
        }
    } else {
        mParser.raiseError(QObject::tr("Malformed manifest"));
    }
    mError = mParser.error();

}

void HsWidgetComponentParser::parseWidgetManifestTag()
{
    Q_ASSERT(mParser.isStartElement() && mParser.name() == WIDGETMANIFEST);
    
    /* Detect version */
    if (mParser.attributes().hasAttribute(VERSION)) {
        QStringRef version = mParser.attributes().value(VERSION);
        mComponentDescriptor.version = version.toString();
        if (version == VERSION_1_0) { 
            parseVersion1_0();
        }
    } else {
        // newest is default
        parseVersion1_0();
        mComponentDescriptor.version = VERSION_1_0;
    }
}

void HsWidgetComponentParser::parseWidgetProviderTag()
{
    Q_ASSERT(mParser.isStartElement() && mParser.name() == WIDGETPROVIDER);
    if (!mParser.readNextStartElement() || mParser.name() != WIDGET) {
        mParser.raiseError(QObject::tr("Malformed manifest"));
        return;
    }
   
    // everything is in attributes
    QXmlStreamAttributes attrs = mParser.attributes();
    foreach (QXmlStreamAttribute attr, attrs) {
        QStringRef name = attr.name();
        if (name == URI) {
            mComponentDescriptor.uri = attr.value().toString();
        } else if (name == ICONURI) {
            mComponentDescriptor.iconUri = attr.value().toString();   
        } else if (name == TITLE) {
            mComponentDescriptor.title = attr.value().toString();    
        } else if (name == DESCRIPTION) {
            mComponentDescriptor.description = attr.value().toString();
        } else if (name == HIDDEN) {
            mComponentDescriptor.hidden = attr.value().toString();
        } 
    }

}
void HsWidgetComponentParser::parseVersion1_0()
{
    Q_ASSERT(mParser.isStartElement() && mParser.name() == WIDGETMANIFEST);
    /* We'll parse the XML until we reach </hswidgetmanifest> tag, end of cml or have an error. */
	while( !(mParser.tokenType() == QXmlStreamReader::EndElement && mParser.name() == WIDGETMANIFEST) && 
        !mParser.hasError() && !mParser.atEnd()) {
        
        mParser.readNext();
        if (mParser.tokenType() != QXmlStreamReader::StartElement) {
            continue;
        }
		/* Read next start element.*/
	    QStringRef tag = mParser.name();
		if(tag == URI) {
			parseUri();
        } else if (tag == ICON) {
            parseIcon();
        } else if (tag == TITLE) {
            parseTitle();
        } else if (tag == DESCRIPTION) {
            parseDescription();
        } else if (tag == HIDDEN) {
            parseHidden();
        } else if (tag == SERVICEXML) {
            parseServiceXml();
        } else if (tag == PREVIEWIAMGE) {
            parsePreviewImage();
        } else if (tag == TRANSLATIONFILENAME) {
            parseTranslationFileName();
		}
    }
}


void HsWidgetComponentParser::parseUri()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.uri = mParser.text().toString();
}

void HsWidgetComponentParser::parseIcon()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.iconUri = mParser.text().toString();
}

void HsWidgetComponentParser::parseTitle()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.title = mParser.text().toString();
}

void HsWidgetComponentParser::parseDescription()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.description = mParser.text().toString();
}

void HsWidgetComponentParser::parseHidden()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.hidden = mParser.text().toString();
}

void HsWidgetComponentParser::parseServiceXml()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.serviceXml = mParser.text().toString();
}

void HsWidgetComponentParser::parsePreviewImage()
{
    mParser.readNext();
    if(mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
	}
    mComponentDescriptor.previewImage = mParser.text().toString();
}

void HsWidgetComponentParser::parseTranslationFileName()
{
    mParser.readNext();
    if (mParser.tokenType() != QXmlStreamReader::Characters) {
		return;
        }
    mComponentDescriptor.translationFilename = mParser.text().toString();
}

bool HsWidgetComponentParser::isWidgetTagValid() 
{
   return true;
}
