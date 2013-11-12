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
#ifndef XBELREADER_H
#define XBELREADER_H

#include <QString>

class BookmarksManager;
class QXmlStreamReader;
class QIODevice;

/*
 * Reads an XBEL file and imports bookmarks
 * Folders are treated as "tags"
 * i.e. bookmarks inside folders are imported one at a time and the folder name is added as a tag for that bookmark 
 */
class XbelReader
{
public: // ctor/dtor
    XbelReader(BookmarksManager *bmgr);
    virtual ~XbelReader();
    
public: // members
    bool read(QIODevice *device);
    QString errorString() const;

private:
    bool isValidXbel(); 
    void readXBEL();
    QString readFolder(QList<QString>& parentFolders);
    void readBookmark(QList<QString>& parentFolders);
    void readInfo(QList<QString>& tags);
    void readMetadata(QList<QString>& tags, QString &owner);
    void readTags(QList<QString>& tags);

    BookmarksManager *m_bmgr;
    QXmlStreamReader *m_xml;
};
#endif
