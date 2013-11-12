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

#include <QString>
#include <QXmlStreamWriter>
#include <QIODevice>
#include <QDebug>

#include "xbelwriter.h"
#include "BookmarksManager.h"
#include "BookmarkFav.h"
#include "BookmarkResults.h"
#include "TagResults.h"


XbelWriter::XbelWriter(BookmarksManager *bmgr)
    : m_bmgr(bmgr), m_xml(new QXmlStreamWriter())
{
}

XbelWriter::~XbelWriter() {
    if(m_xml) delete m_xml;
}

bool XbelWriter::writeFile(QIODevice *device)
{
    bool retVal = false;
    if(device && m_bmgr) {
        m_xml->setDevice(device);
        m_xml->setAutoFormatting(true);
    
        m_xml->writeStartDocument();
        m_xml->writeDTD("<!DOCTYPE xbel>");
        m_xml->writeStartElement("xbel");
        m_xml->writeAttribute("version", "1.0");
    
        BookmarkResults *bookmarks = m_bmgr->findAllBookmarks();
        if(bookmarks) {
            while (BookmarkFav *bm = bookmarks->nextBookmark()) {
                if(!bm->url().isEmpty())
                    writeBookmark(bm);
                delete bm;
                bm = NULL;
            }
            delete bookmarks;
        }
        m_xml->writeEndElement(); // xbel
        m_xml->writeEndDocument();
        retVal = true;
    }
    return retVal;
}

void XbelWriter::writeBookmark(BookmarkFav *bm)  
{
    m_xml->writeStartElement("bookmark");
    m_xml->writeAttribute("href", bm->url());
    m_xml->writeTextElement("title", bm->title());
    writeTags(bm->id());
    m_xml->writeEndElement(); // bookmark
}

void XbelWriter::writeTags(int bmId) 
{
    if(bmId > 0) {
        
        TagResults * tr = m_bmgr->findTagsByBookmark(bmId);
        
        if(tr) {
            m_xml->writeStartElement("info");
            m_xml->writeStartElement("metadata");
            m_xml->writeAttribute("owner", "http://www.nokia.com");
            m_xml->writeStartElement("tags");
            try {
                while(QString * tag = tr->nextTag()) {
                    m_xml->writeTextElement("tag", *tag);
                    delete tag;
                    tag = NULL;
                }
            } catch(...) {
                qDebug() << "Exception Thrown while writing tags.";
            }
    
            m_xml->writeEndElement(); // tags
            m_xml->writeEndElement(); // metadata
            m_xml->writeEndElement(); // info
            delete tr;
        }
    }
}

