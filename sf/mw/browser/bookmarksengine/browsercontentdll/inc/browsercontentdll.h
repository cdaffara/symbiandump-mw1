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
#ifndef BROWSERCONTENT_H
#define BROWSERCONTENT_H

#include<QObject>
#include<QString>
#include<browsercontentdllclientdefs.h>
#include<QtGui>


static const char KDOUBLEQUOTE[]       = "&#34";
static const char KBACKSLASH[]         = "&#92";
class BrowserContentPrivate;

class BOOKMARKSCONTENTDLL_EXPORT BookmarkLeaf {
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    QString getTag(){ return m_tag;}
    int getIndex() {return m_index;}
    
    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setTag(QString atag){m_tag=atag;}
    void setIndex(int aIndex){m_index=aIndex;}
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

class BOOKMARKSCONTENTDLL_EXPORT HistoryLeaf {
public:
    QString getUrl(){return url;}
    QString getTitle(){return title;}
    QDate getDate(){return date;}
    QTime getLastVisited(){return lastVisited;}
    QString getDesc(){return desc;}
    QIcon getFavIcon(){return favicon;}
    int getIndex() {return m_index;}
    
    void setUrl(QString aurl){url=aurl;}
    void setTitle(QString atitle){title=atitle;}
    void setDate(QDate adate){date=adate;}
    void setLastVisited(QTime atime){lastVisited=atime;}
    void setDesc(QString adesc){desc=adesc;}
    void setFavIcon(QIcon afavicon){favicon=afavicon;}
    void setIndex(int aIndex){m_index=aIndex;}
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
    int m_index;
   
};

class BOOKMARKSCONTENTDLL_EXPORT SuggestData : public QObject
{
        Q_OBJECT
        
        public:
            SuggestData(QString pgTitle, QString pgUrl) : m_title(pgTitle), m_url(pgUrl){};
            Q_PROPERTY(QString title READ pageTitle)
            Q_PROPERTY(QString url READ pageUrl)
            
        private:
            QString m_title;
            QString m_url;
            QString pageTitle() {return m_title;}
            QString pageUrl() {return m_url;}
};
class BOOKMARKSCONTENTDLL_EXPORT BrowserContent:public QObject {
Q_OBJECT
BOOKMARKSCLIENT_PRIVATE(BrowserContent)
public:

    BrowserContent(QString aClientName);
    ~BrowserContent();
    int addBookmark(BookmarkLeaf* BookmarkContent);
    int deleteBookmark(QString title);
    QList<BookmarkLeaf*> fetchAllBookmarks();
    QList<BookmarkLeaf*> suggestBookMarks(QString atitle);
    int reorderBokmarks(QString title,int new_index);
	int modifyBookmark(QString aOrgTitle, QString aNewTitle, QString aNewUrl);
	QObjectList suggestContent(QString atitle);

    int addHistory(HistoryLeaf* HistoryContent);
    QList<HistoryLeaf*> fetchHistory();
    QList<HistoryLeaf*> suggestHistory(QString atitle);
    int clearHistory();
    int clearBookmarks();
    QString fetchSerializedBookmarks();
    void fetchSerializedHistory(QVector<QString> &folderVector,QMap<QString,QString> &mymap);
    void fetchAllBookmarkTitles(QVector<QString> &title);
    QMap<QString, QString> findSimilarHistoryItems(QString atitle);

private:
  int createDatabase();
  QString filterUrl(QString atitle);
  QString findFolderForDate( QDate& nodeDate);
  bool dateInThisMonth(QDate &date);

private:
	QObjectList suggestedList;
};
#endif //BROWSERCONTENT_H
