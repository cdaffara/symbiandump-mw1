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
#ifndef XBELWRITER_H
#define XBELWRITER_H

class BookmarksManager;
class BookmarkFav;
class QXmlStreamWriter;
class QIODevice;

/*
 * This will export all bookmarks
 * Each tag will be treated as a folder, so the bookmarks will be output by tag 
 * (there will be duplicates if a bookmark has more than one tag)
 * Then any bookmarks that don't have tags will be written out into the top level
 */
class XbelWriter
{
public: // ctor/dtor
    XbelWriter(BookmarksManager *bmgr);
    virtual ~XbelWriter();
    
public: // members
    bool writeFile(QIODevice *device);

private:
    void writeBookmark(BookmarkFav * bm);
    void writeTags(int bmId);
    
    QXmlStreamWriter *m_xml;
    BookmarksManager *m_bmgr;
};

#endif
