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
#ifndef BOOKMARKSMANAGER_H
#define BOOKMARKSMANAGER_H

#include <QString>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMap>

#include "bookmarksapi.h"
class QWidget;
class BookmarkFav;
class BookmarkResults;
class TagResults;

#define BOOKMARKS_DB_NAME   "Bookmarks"
#define BOOKMARKS_DB_FILE   "bookmarks.db"

class BOOKMARKSAPI_EXPORT BookmarksManager : public QObject {
  
  Q_OBJECT

public:
    enum status {
            SUCCESS = 0,
            DATABASEERROR = -1,
            FAILURE = -2
    };
    
    BookmarksManager(QWidget *parent = 0);
    ~BookmarksManager();

public slots:
    int addBookmark(QString title, QString URL);
    int modifyBookmark(int origBookmarkId, QString newTitle, QString newURl);
    int deleteBookmark(int bookmarkId);
    int clearAll();
    int deleteTag(int bookmarkId, QString tag);
    int addTag(int bookmarkId, QString tag);
    BookmarkResults *findAllBookmarks();
    TagResults *findAllTags();
    BookmarkResults *findBookmarksByTag(QString tag);
    int importBookmarks(QString xbelFilePath);
    int exportBookmarks(QString xbelFilePath);
    BookmarkFav* findBookmark(int bookmarkId);
    BookmarkResults *findUntaggedBookmarks();
    int reorderBookmark(int bookmarkID, int newIndex);
    TagResults* findTagsByBookmark(int bookmarkID);
		QMap<QString, QString> findBookmarks(QString atitle);   
 
 private:
    bool doQuery(QString query);
    void createBookmarksSchema();
    QString normalizeUrl(const QString& url);
    void lastErrMsg(QSqlQuery& query);

    QSqlDatabase  m_db;
    // Note: One instance of a query was locking the database even after using finish() and clear()
    //QSqlQuery*    m_query;
};
#endif //BOOKMARKSMANAGER_H
