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
* Description: This implements the bookmarks API's.
*
*/

#include<QString>
#include<QFile>
#include<QFileInfo>
#include<QDebug>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QWidget>
#include<QtGui>

#include "bookmarksapi.h"
#include "BookmarksManager.h"
#include "BookmarkFav.h"
#include "BookmarkResults.h"
#include "TagResults.h"
#include "xbelreader.h"
#include "xbelwriter.h"
#include "defaultBookmarks.xml.h"

BookmarksManager::BookmarksManager(QWidget *parent) :
    QObject(parent)
{
    setObjectName("bookmarksManager");

    m_db = QSqlDatabase::database(BOOKMARKS_DB_NAME);
    if (!m_db.isValid()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE", BOOKMARKS_DB_NAME);
        m_db.setDatabaseName(BOOKMARKS_DB_FILE);
    }
    if (m_db.open()) {
        // TODO: Do we need a flag so that this gets called only once OR when
        // creating a database tables "IF NOT EXISTS" is good enough
        createBookmarksSchema();
    }
}

BookmarksManager::~BookmarksManager() {
    m_db.close();
    QSqlDatabase::removeDatabase(BOOKMARKS_DB_NAME);
}

/*
 * +-------------+       +----------+
 * |Bookmarks(ID)|* <-> *|Tags(BMID)|
 * +-------------+       +----------+
 */
void BookmarksManager::createBookmarksSchema() {
    // Bookmarks
    if (!doQuery("CREATE TABLE IF NOT EXISTS bookmarks("
                    "id INTEGER PRIMARY KEY,"
                    "title text, "
                    "url text,"
                    "sortIndex int DEFAULT 0)")) {
        // TODO: do some error handling here!
        return;
    }
    // Make sorting faster
    if (!doQuery("CREATE INDEX IF NOT EXISTS bm_sort_idx ON bookmarks(sortIndex ASC)")) {
        // TODO: do some error handling here!
        return;
    }
    // We do a lot of lookups by id
    if (!doQuery("CREATE INDEX IF NOT EXISTS bm_id_idx ON bookmarks(id ASC)")) {
        // TODO: do some error handling here!
        return;
    }

    // Tags
    // Note: foreign key constraints are not enforced in the current version of sqlite
    // that we are using.
    if (!doQuery("CREATE TABLE IF NOT EXISTS tags("
                    "bmid INTEGER,"
                    "tag text,"
                    "FOREIGN KEY(bmid) REFERENCES bookmarks(id))")) {
        // TODO: do some error handling here!
        return;
    }
    // We do a lot of lookups, both by bookmark id and by tag
    if (!doQuery("CREATE INDEX IF NOT EXISTS tags_bmid_idx ON tags(bmid ASC)")) {
        // TODO: do some error handling here!
        return;
    }
    if (!doQuery("CREATE INDEX IF NOT EXISTS tags_tag_idx ON tags(tag ASC)")) {
        // TODO: do some error handling here!
        return;
    }
}

// TODO refactor this - nothing except the schema creation can use it as is
bool BookmarksManager::doQuery(QString query) {
#ifdef ENABLE_PERF_TRACE
    PERF_DEBUG() << __PRETTY_FUNCTION__ << query << "\n";
    unsigned int st = WrtPerfTracer::tracer()->startTimer();
#endif
    QSqlQuery db_query(m_db);
    bool ok = db_query.exec(query);
    if (!ok) {
        qDebug() << "BookmarksManager::doQuery" << QString("ERR: %1 %2").arg(db_query.lastError().type()).arg(db_query.lastError().text()) << " Query: " << db_query.lastQuery();
    }
#ifdef ENABLE_PERF_TRACE
    PERF_DEBUG() << __PRETTY_FUNCTION__ << WrtPerfTracer::tracer()->elapsedTime(st) << "\n";
#endif
    return ok;   
}


/**==============================================================
 * Description: Normalize a given url, if needed.
 * It adds http:// in front, if the url is relative.
 ================================================================*/
QString BookmarksManager::normalizeUrl(const QString& url)
{
    // If the URL is relative, add http in front
    QString updatedUrl = url;
    
    if (!url.contains("://")) {
        updatedUrl.prepend("http://");
    }
    return updatedUrl;
}

/**==============================================================
 * Description: Adds the bookmark to the database, given title and
 * url. 
 * Returns: Returns bookmarkID ( >0)else Failure (-1 or -2)
 ================================================================*/
int BookmarksManager::addBookmark(QString title, QString URL)
{
    int bookmarkId = 0;
    
    if(URL.isEmpty()) {
       bookmarkId = FAILURE;
    }
    
    if(bookmarkId != FAILURE) {
        // do some checking on parameters
        QString updatedTitle = title;
        QString updatedUrl = normalizeUrl(URL);
        int soIndex = 1;
        if (title.isEmpty()) {
            updatedTitle = "Untitled";
        }
        if (m_db.isOpen()) {
            QSqlQuery query(m_db);
            m_db.transaction();
            if (!query.exec("SELECT count(*) from bookmarks")) {
                lastErrMsg(query);
                m_db.rollback();
                return DATABASEERROR;
            }
            if(query.next()) {
                query.prepare("UPDATE bookmarks SET sortIndex=sortIndex+1 WHERE sortIndex >= :sIndex");
                query.bindValue(":sIndex", soIndex);
                if (!query.exec()) {
                    lastErrMsg(query);
                    m_db.rollback();
                    return DATABASEERROR;
                 }
           } 
           query.prepare("INSERT INTO bookmarks (title, url, sortIndex) "
                               "VALUES (:title, :url, :sIndex)");
            query.bindValue(":title", QVariant(updatedTitle));
            query.bindValue(":url",    QVariant(updatedUrl));
            query.bindValue(":sIndex",  QVariant(soIndex));
            if (!query.exec()) {
                lastErrMsg(query);
                m_db.rollback();
                return DATABASEERROR;
           }
           // Note: lastInsertId() is not thread-safe
            bookmarkId = query.lastInsertId().toInt();
            if (!m_db.commit()) {
                qDebug() << m_db.lastError().text();
                m_db.rollback();
               return DATABASEERROR;
            }
        } else {
            bookmarkId = FAILURE;
        }
      }
    return bookmarkId;
}

/**==============================================================
 * Import bookmarks from an XBEL file. If no filename is 
 * passed in a set of default bookmarks will be imported.
 * The default bookmarks used can be changed by updating
 * the file "defaultBookmarks.xml.cpp".
 * @param xbelFilePath - String containing the path to the
 * file to import.
 * @return SUCCESS or FAILURE depending on whether the 
 * import process was successful.
 ==============================================================*/
int BookmarksManager::importBookmarks(QString xbelFilePath)
{
    XbelReader *reader = new XbelReader(this);
    bool retVal = false;
    
    if(xbelFilePath.isEmpty() || !QFile::exists(xbelFilePath)) {
        xbelFilePath = "c:\\data\\temp.xml";
        QFile file(xbelFilePath);
        if(file.exists())
            file.remove();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return false;
        QTextStream out(&file); 
        out << defaultBookmarksList;
        out.flush();
        file.close();
    } 
    QFile file(xbelFilePath);
    if(file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        retVal = reader->read(&file);
        file.close();
    }
    if(reader)
        delete reader;
    return retVal ? SUCCESS : FAILURE;
}

/**==============================================================
 * Exports all bookmarks in the database to an XBEL file. This function
 * expects a valid file path to be passed in. If the path is not valid
 * a default file path will be used. If a file with that name already 
 * exists at that location,
 * @param xbelFilePath String containing the path of the file to 
 * export to.
 * @return SUCCESS or FAILURE depending on whether the 
 * export process was successful.
 ==============================================================*/
int BookmarksManager::exportBookmarks(QString xbelFilePath)
{
    XbelWriter *writer = new XbelWriter(this);
    bool retVal = false;

    if(xbelFilePath.isEmpty()) {
        xbelFilePath = "c:\\data\\myBookmarks.xml";
    }  

    QFile file(xbelFilePath);
    if(file.exists()) {
        file.remove(xbelFilePath);
    }
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    retVal = writer->writeFile(&file);
    file.flush();
    file.close();
    
    if(writer)
        delete writer;
    return retVal ? SUCCESS : FAILURE;
    
}

/**==============================================================
 * Description: Modify the existing bookmark. Given id of the bookmark
 * and new title or url, it modifies the existing bookmark.
 * Returns : Success(0) or Failure(-1 or -2).
 ================================================================*/
int BookmarksManager::modifyBookmark(int origBookmarkId, QString newTitle, QString newURL)
{
    int retVal = SUCCESS;
    
    // Client has to at least pass title or url
    if(newTitle.isEmpty() && newURL.isEmpty())
       retVal = FAILURE;
     
    if(retVal == SUCCESS) {
    
        if (m_db.isOpen()) { 
              QSqlQuery query(m_db);
            
            if(newTitle.isEmpty()) {        
                query.prepare("UPDATE bookmarks SET url=:newurl WHERE id=:bmId");
                query.bindValue(":newurl", normalizeUrl(newURL));
                query.bindValue(":bmId", origBookmarkId);
            } else if(newURL.isEmpty()) { 
                query.prepare("UPDATE bookmarks SET title=:newTitle WHERE id=:bmId");
                query.bindValue(":newTitle", newTitle);
                query.bindValue(":bmId", origBookmarkId);
            } else {
                query.prepare("UPDATE bookmarks SET url=:newurl, title=:newTitle WHERE id=:bmId");
                query.bindValue(":newurl", normalizeUrl(newURL));
                query.bindValue(":newTitle", newTitle);
                query.bindValue(":bmId", origBookmarkId);
            }
            if (!query.exec()) {
                lastErrMsg(query);
                return DATABASEERROR;
            } 
            if (query.numRowsAffected() == 0) {
                // No update happened - must be an invalid id
                // TODO: shall we return some other status
                retVal = FAILURE;
            }
        } else 
            retVal = FAILURE;
    }
    return retVal;
}

/**===================================================================================
 * Description: Delete the bookmark item, also delete all the tags associated with it.
 * Returns: SUCCESS(0) or Failure(-1 or -2)
 ======================================================================================*/
int BookmarksManager::deleteBookmark(int bookmarkId)
{
    int retVal = SUCCESS;
   
    if (m_db.isOpen()) {      
    
        QSqlQuery query(m_db);
         
        // TODO: Need to think about whether we need to get sortIndex and update all the 
        // rows after the deletion or not

        // TODO: check if transaction() has been supported 
        // by calling hasfeature() function first
        m_db.transaction();
        
        query.prepare("DELETE FROM bookmarks WHERE id=:bmId");
        query.bindValue(":bmId", bookmarkId);
        if (!query.exec()) {
            lastErrMsg(query);
            m_db.rollback();
            return DATABASEERROR;
        }
        
        query.prepare("DELETE FROM tags WHERE bmid=:bmId");
        query.bindValue(":bmId", bookmarkId);
        if (!query.exec()) {
            lastErrMsg(query);
            m_db.rollback();
            return DATABASEERROR;
        } 
        if (!m_db.commit()) {
            qDebug() << m_db.lastError().text();
            m_db.rollback();
            return DATABASEERROR;
        }         
    } else 
        retVal = FAILURE;
    
    return retVal;     
}

/**===================================================================================
 * Description: Delete all records from the bookmarks table as well as all the tags.
 * Returns: SUCCESS(0) or FAILURE(-1 or -2)
 ======================================================================================*/
int BookmarksManager::clearAll()
{
    int retVal = SUCCESS;
    
    if (m_db.isOpen()) {        
       QSqlQuery query(m_db);
           
       // TODO: check if transaction() has been supported 
       // by calling hasfeature() function first
       m_db.transaction();
       
       if(!query.exec("DELETE FROM bookmarks")) {  
            lastErrMsg(query);
            m_db.rollback();
            retVal = DATABASEERROR;
       } 
       if (retVal == SUCCESS && !query.exec("DELETE FROM tags")) {
            lastErrMsg(query);
            m_db.rollback();
            retVal = DATABASEERROR;
       } 
       if (retVal == SUCCESS && !m_db.commit()) {
           qDebug() << m_db.lastError().text();
           m_db.rollback();
           retVal = DATABASEERROR;
       }
    } else 
        retVal = FAILURE;
    
    return retVal;     
}

/**==============================================================
 * Description: Deletes a single tag associated with the bookmark
 * Returns: SUCCESS(0) or FAILURE (-1 or -2)
 ===============================================================*/
int BookmarksManager::deleteTag(int bookmarkId, QString tagToDelete)
{
    int retVal = SUCCESS;
 
    if(tagToDelete.isEmpty()|| bookmarkId < 0)
          retVal = FAILURE;
    
    if (retVal == SUCCESS) {
        if (m_db.isOpen()) {
            QSqlQuery query(m_db);
         
            query.prepare("DELETE FROM tags WHERE bmid=:bmId AND tag=:tag");
            query.bindValue(":bmId", bookmarkId);
            query.bindValue(":tag", tagToDelete);
            if (!query.exec()) {
                lastErrMsg(query);
                retVal = DATABASEERROR;
            }
        } else 
            retVal = FAILURE;
    }
    
    return retVal;     
}

/**================================================================
 * Description: Adds a single tag associated given the bookmark id
 * Returns: SUCCESS(0) or FAILURE (-1 or -2)
 ==================================================================*/
int BookmarksManager::addTag(int bookmarkId, QString tagToAdd)
{
    int retVal = SUCCESS;
    
    if(tagToAdd.isEmpty()|| bookmarkId < 0)
       retVal = FAILURE;
    
    if(retVal == SUCCESS) {
        if (m_db.isOpen()) {     
             QSqlQuery query(m_db);
             
            query.prepare("INSERT INTO tags (bmid, tag) "
                     "VALUES (:id, :tag)");                      
            query.bindValue(":id",   QVariant(bookmarkId));
            query.bindValue(":tag",  QVariant(tagToAdd));
            
            if (!query.exec()) {
                lastErrMsg(query);
                retVal = DATABASEERROR;
            } 
        } else 
            retVal = FAILURE;
    }
    
    return retVal;
}

/**==============================================================
 * Description: Finds all the bookmarks weather they have tags 
 * or not.
 * Returns: A pointer to BookmarkResults object or NULL.
 ===============================================================*/
BookmarkResults *BookmarksManager::findAllBookmarks()
{
    BookmarkResults * results = NULL; 
    
    QString queryStr = QString("SELECT "
                               " id, title, url, sortIndex " 
                               " FROM bookmarks ORDER BY sortIndex");
    if (m_db.isOpen()) {
        QSqlQuery *query = new QSqlQuery(m_db);
        if (query->exec(queryStr)) {
             results = new BookmarkResults(query);
        } else {
            qDebug() << query->lastError().text() << " Query: " << query->lastQuery();
            results = NULL;
        }
    }
    return results;
}

/**==============================================================
 * Description: Finds all the distinct tags.
 * Returns: A pointer to TagResults object or NULL.
 ===============================================================*/
TagResults *BookmarksManager::findAllTags()
{
    TagResults * results = NULL; 
    
    if (m_db.isOpen()) {
        QSqlQuery *query = new QSqlQuery(m_db);
        if (query->exec("SELECT DISTINCT tag FROM tags")) {
            // TODO: do we need javascript hack here like in findAllBookmarks API.
            results = new TagResults(query); 
        } else {
            qDebug() << query->lastError().text() << " Query: " << query->lastQuery();
            results = NULL;
        }
    }
    return results;
}


/**==============================================================
 * Description: Finds all the bookmarks associated with a given
 * tag. 
 * Returns: A pointer to BookmarkResults object or NULL.
 ===============================================================*/
BookmarkResults *BookmarksManager::findBookmarksByTag(QString tag)
{
    BookmarkResults * results = NULL; 
    
    QString queryStr = QString("SELECT "
                               " id, title, url, sortIndex " 
                               " FROM bookmarks b JOIN"
                               " tags t ON b.id=t.bmid WHERE" 
                               " t.tag=:tag");
     if (m_db.isOpen()) {
         QSqlQuery *query = new QSqlQuery(m_db);
         query->prepare(queryStr);
         query->bindValue(":tag", tag);
         if (query->exec()) {
             // TODO: do we need javascript hack here like in findAllBookmarks API.
             results = new BookmarkResults(query);
         } else {
            qDebug() << query->lastError().text() << " Query: " << query->lastQuery();
            results = NULL;
         }
     }
     return results;
}


/**==============================================================
 * Description: Finds all the Tags associated with a given
 * bookmarkID.
 * Returns: A pointer to TagResults object or NULL.
 ===============================================================*/
TagResults *BookmarksManager::findTagsByBookmark(int bookmarkID)
{
     TagResults * results = NULL; 
    
     QString queryStr = QString("SELECT DISTINCT tag "
                                 " FROM tags t JOIN"
                                 " bookmarks b ON t.bmid=b.id WHERE" 
                                 " t.bmid=:id");
     if (m_db.isOpen()) {
        QSqlQuery *query = new QSqlQuery(m_db);
        query->prepare(queryStr);
        query->bindValue(":id", bookmarkID);
        if (query->exec()) {
            // TODO: do we need javascript hack here like in findAllBookmarks API.
            results =  new TagResults(query); 
        } else {
            qDebug() << query->lastError().text() << " Query: " << query->lastQuery();
            results = NULL;
        }
     }
     return results;
}

/**==============================================================
 * Description: Finds all the Bookmarks that doesn't have any
 * tags associated with it. It is needed by export API.
 * Returns: A pointer to BookmarkResults object or NULL.
 ===============================================================*/
BookmarkResults *BookmarksManager::findUntaggedBookmarks()
{   
    BookmarkResults * results = NULL; 
       
    QString queryStr = QString("SELECT "
                               " id, title, url, sortIndex " 
                               " FROM bookmarks b LEFT OUTER JOIN"
                               " tags t ON b.id=t.bmid WHERE" 
                               " t.bmid IS NULL ORDER BY sortIndex");
    
    if (m_db.isOpen()) { 
       QSqlQuery *query = new QSqlQuery(m_db);
       if (query->exec(queryStr)) {
           // TODO: do we need javascript hack here like in findAllBookmarks API.
           results = new BookmarkResults(query);
       } else {
           qDebug() << query->lastError().text() << " Query: " << query->lastQuery();
           results = NULL;
       }
    }
    return results;
}


/**==============================================================
 * Description: Reorder bookmarks. Moves a given bookmark to a 
 * passed new index.
 * Returns: SUCCESS(0) or FAILURE (-1 or -2)
 ===============================================================*/
int BookmarksManager::reorderBookmark(int bookmarkID, int newIndex)
{   
    if (newIndex <= 0) 
        return FAILURE;

    if (!m_db.isOpen())
    	return DATABASEERROR;

	QSqlQuery query(m_db);

	// Make sure the bookmark exists
	BookmarkFav *bm = findBookmark(bookmarkID);
	if (!bm)
		return FAILURE;

	// Next, help stamp out and abolish redundancy.  If the bookmark is already at this sortIndex, do nothing.
	if (bm->sortIndex() == newIndex)
		return SUCCESS;

	/*
	 * Ok, the way the sortIndex works is that you can specify any sortIndex you want (it doesn't have to be consecutive).
	 * This means there can be holes in the list of sortIndexes.  Whenever someone wants to move a bookmark to a new sort
	 * position, we just bump everything that was in that position and further down the list by 1 and then set the sortIndex
	 * of the bookmark to what they want.  This should work whether there is more than one bookmark or not.
	 */

	// Bump all the bookmarks from this sortIndex down by 1 first
	m_db.transaction();
	query.prepare("UPDATE bookmarks SET sortIndex=sortIndex+1 WHERE sortIndex >= :newIndex");
	query.bindValue(":newIndex", newIndex);
	if (!query.exec()) {
		lastErrMsg(query);
		return DATABASEERROR;
	}
	// Then just set this bookmark's sortIndex to the new value
	query.prepare("UPDATE bookmarks SET sortIndex=:newIndex WHERE id=:id");
	query.bindValue(":id", bookmarkID);
	query.bindValue(":newIndex", newIndex);
	if (!query.exec()) {
		  lastErrMsg(query);
		  m_db.rollback();
		  return DATABASEERROR;
	}
	if (!m_db.commit()){
		qDebug() << m_db.lastError().text();
		m_db.rollback();
		return DATABASEERROR;
	}
    return SUCCESS;
}

/**==============================================================
 * Description: Finds a bookmark based on a given bookmarkID.
 * Returns: A pointer to BookmarkFav object or NULL.
 ===============================================================*/
BookmarkFav* BookmarksManager::findBookmark(int bookmarkId)
{
    BookmarkFav * results = NULL;
    
  
    if (m_db.isOpen()) { 
        QSqlQuery query(m_db);  
        query.prepare("SELECT title, url, sortIndex FROM bookmarks WHERE id=:id");
        query.bindValue(":id", bookmarkId);
        if (query.exec()) {
            if (query.next()) 
                results = new BookmarkFav(bookmarkId, query.value(0).toString(),
                         query.value(1).toString(), query.value(2).toInt()); 
        } else {
            lastErrMsg(query);
        }
    }
    return results;
}

/**==============================================================
 * Description: Finds a bookmark based on a given bookmarkID.
 * Returns: A pointer to BookmarkFav object or NULL.
 ===============================================================*/
QMap<QString, QString> BookmarksManager::findBookmarks(QString atitle)
{      
    QMap<QString, QString> map;
    
    if (m_db.isOpen()) { 
        QSqlQuery query(m_db);    
        QString queryStatement = "SELECT url, title FROM bookmarks WHERE title LIKE '%"+atitle+"%' OR url LIKE '%" + atitle + "%'";          
        query.prepare(queryStatement);
        if(query.exec()) {    
           while (query.next()){
              QString bookmarkUrl = query.value(0).toString();
              QString bookmarkTitle =   query.value(1).toString();
              map.insert( bookmarkUrl, bookmarkTitle );
           }
        } else {
            lastErrMsg(query); 
        }
    }
    return map;
}

/**==============================================================
 * Description: Prints a last error message from the query.
 * Returns: Nothing.
 ===============================================================*/
void BookmarksManager::lastErrMsg(QSqlQuery& query) 
{
    qDebug() << "BookmarksManager::lastErrMsg" << QString("ERR: %1 %2").arg(query.lastError().type()).arg(query.lastError().text()) << " Query: " << query.lastQuery();
}

