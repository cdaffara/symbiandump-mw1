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
* Description:
*
*/
#ifndef BOOKMARKCLIENT_H
#define BOOKMARKCLIENT_H

#include<QObject>
#include<QString>
#include<bookmarkclientdefs.h>
#include<QtGui>
//using namespace WRT;

class BookmarksClientSidePrivate;

class BOOKMARKSENGINECLIENT_EXPORT BookmarkLeaf
{
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    QString getTag(){ return m_tag;}
    int getIndex(){ return m_index;}

    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setTag(QString atag){m_tag=atag;}
    void setIndex(int index){m_index=index;}
private:    
    QString url;
    QString title;

    /*The last date on which URL is visited*/
    QDate date;
    /*The last time on which URL is visited*/
    QTime lastVisited;

    QString desc;
    QIcon favicon;
    bool expanded;
    QString m_tag;
    int m_index;
    

};

class BOOKMARKSENGINECLIENT_EXPORT BookmarksClientSide : public QObject
    {
Q_OBJECT
BOOKMARKSCLIENT_PRIVATE(BookmarksClientSide)
public:

    BookmarksClientSide();
    ~BookmarksClientSide();
    int connectToServer();
    void closeServer();
    int AddBookmark(BookmarkLeaf* BookmarkContent);
    BookmarkLeaf* FetchBookmark(QString title);
    int DeleteBookmark(QString title);
    int getSessionId(int& BookmarkId);
    QList<BookmarkLeaf*> FetchAllBookmarks();
signals:
    void BookmarkAdded(QString title);
    void BookmarkDeleted(QString title);
private slots:
    void handleServerResponse();
    };
#endif //BOOKMARKCLIENT_H
