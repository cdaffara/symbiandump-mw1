/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
*/
#include <QXmlStreamReader>
#include <QIODevice>
#include <QList>

#include "xbelreader.h"
#include "BookmarksManager.h"

XbelReader::XbelReader(BookmarksManager *bmgr) : 
    m_bmgr(bmgr), m_xml(new QXmlStreamReader)
{
}

XbelReader::~XbelReader() {
    if(m_xml) delete m_xml;
}

bool XbelReader::read(QIODevice *device)
{
    if(!device) {
        m_xml->raiseError(QObject::tr("Invalid Device passed into XBEL Reader."));
    }
    if(!m_bmgr) { 
        m_xml->raiseError(QObject::tr("Invalid BookmarkManager passed into XBEL Reader."));
    } else {
        m_xml->setDevice(device);
        if(m_xml->readNextStartElement()) {
           if(m_xml->name() == "xbel" && 
              m_xml->attributes().value("version") == "1.0")
               readXBEL();
           else
               m_xml->raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
        } else 
            m_xml->raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
    }
        
    return !m_xml->error();
}

/**
 * This function should be called before reading any xml elements
 * It attempts to validate that the file being read is actually an 
 * xbel file.
 * @return whether the file is an XBEL file.
 */
bool XbelReader::isValidXbel() 
{
    // The xbel element must exist and must be version 1.0
    return true;
}

QString XbelReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_xml->errorString())
            .arg(m_xml->lineNumber())
            .arg(m_xml->columnNumber());
}

void XbelReader::readXBEL()
{
    QString folder = "";
    QList<QString> folders;
    while (m_xml->readNextStartElement()) {
        if (m_xml->name() == "folder")
            readFolder(folders);
        else if (m_xml->name() == "bookmark")
            readBookmark(folders);
        else
            m_xml->skipCurrentElement();
    }
}

QString XbelReader::readFolder(QList<QString>& parentFolders)
{
    QString folderName = "";

    while (m_xml->readNextStartElement()) {
        if (m_xml->name() == "title")
            folderName = m_xml->readElementText();
        else if (m_xml->name() == "folder") {
            QList<QString> folders;
            folders = folders + parentFolders;
            if(!folderName.isEmpty())
                folders.append(folderName);
            readFolder(folders);
        } else if (m_xml->name() == "bookmark") {
            QList<QString> folders;
            folders = folders + parentFolders;
            if(!folderName.isEmpty())
                folders.append(folderName);
            readBookmark(folders);
        } else
            m_xml->skipCurrentElement();
    }
    return folderName;
}

void XbelReader::readBookmark(QList<QString>& parentFolders)
{
    QXmlStreamAttributes attrs = m_xml->attributes();
    
    // if there is no URL attribute skip this bookmark.
    if(!attrs.hasAttribute("href")) {
        m_xml->skipCurrentElement();
        return;
    }
    
    QString url = attrs.value("href").toString();
    QString title = "Untitled";
    QString desc = "";
    QList<QString> tags;
    tags = tags + parentFolders;
    
    while (m_xml->readNextStartElement()) {
        if (m_xml->name() == "title")
            title = m_xml->readElementText();
        else if(m_xml->name() == "desc")
            desc = m_xml->readElementText();
        else if(m_xml->name() == "info")
            readInfo(tags);
        else
            m_xml->skipCurrentElement();
    }

    int bmID = m_bmgr->addBookmark(title, url);
    if (bmID > 0) {
        QList<QString>::iterator iter;
        for (iter = tags.begin(); iter != tags.end(); ++iter)
            m_bmgr->addTag(bmID, *iter);
    }
}

/**
 * Reads the info element.
 * @param tags - A reference to a list of string tags
 */
void XbelReader::readInfo(QList<QString>& tags) 
{
    QString owner = "http://www.nokia.com";
    while(m_xml->readNextStartElement()) {
        if(m_xml->name() == "metadata")
            readMetadata(tags, owner);
        else
            m_xml->skipCurrentElement();
    }
}

/**
 * This function reads the metadata XBEL tag.
 * @param tags - A reference to a list of strings. This is passed into
 * readTags.
 * @param owner - A reference to the owner string. This is to allow for
 * future support for this attribute.
 */
void XbelReader::readMetadata(QList<QString>& tags, QString &owner) 
{
    if(m_xml->attributes().hasAttribute("owner"))
    owner = m_xml->attributes().value("owner").toString();
    while(m_xml->readNextStartElement()) {
        if(m_xml->name() == "tags")
            readTags(tags);
        else
            m_xml->skipCurrentElement();
    }
    
}

/**
 * Reads the tags and tag attributes, adding all tags found to 
 * the list of string tags passed into the function.
 * @param tags - Reference to a list of string tags.
 */
void XbelReader::readTags(QList<QString>& tags) 
{
    while(m_xml->readNextStartElement()) {
        if(m_xml->name() == "tag") 
            tags.append(m_xml->readElementText());
        else
            m_xml->skipCurrentElement();
    }

}

