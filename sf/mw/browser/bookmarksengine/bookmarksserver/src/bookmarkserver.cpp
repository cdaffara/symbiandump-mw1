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

#include<bookmarkserver.h>
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include<bookmarkscommonengine.h>
#include<serviceipcrequest.h>
#include<QSqlError>
#include<QDebug>

/**==============================================================
 * Description: Constructor of BookMarkServer
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookMarkServer::BookMarkServer() :
    m_sessionCount(0)
    {

    m_server = new WRT::ServiceFwIPCServer(this, this);
    createDatabase();
    bool ret = m_server->listen(BOOKMARKSENGINESERVER);
    if (!ret)
        {
        qDebug() << " Failed to LISTEN" << '\n';
        QTimer::singleShot(1, this, SLOT(closeServer()));
        return;
        }

    m_server->configIpcServerLifetime(true);

    }

/**==============================================================
 * Description:Destructor of BookMarkServer
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookMarkServer::~BookMarkServer()
    {

    if (m_server)
        {
        m_server->disconnect();
        delete m_server;
        m_server = NULL;
        }
    }

/**==============================================================
 * Description:closes the bookmark Server connection
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::closeServer()
    {
    // before shutting down, check if any active session exists

    if (m_sessionCount <= 0)
        QCoreApplication::quit();
    }

/**==============================================================
 * Description:Handles the client request
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
bool BookMarkServer::handleRequest(WRT::ServiceIPCRequest *aRequest)
    {

    WRT::ClientInfo* clientInfo =
            const_cast<WRT::ClientInfo*> (aRequest->clientInfo());
    int sessionID = clientInfo->sessionId();
    bool val(false);
    bool result = true;
    int command = aRequest->getOperation().toInt(&val);
    QString msg = aRequest->getData();
    QStringList list = msg.split(BOOKMARKSENGINESEPARATOR);
    switch (command)
        {
        case (EDeleteBookMark):
            deleteBookMark(aRequest);
            break;
        case (EAddBookMark):
            addBookMark(aRequest);
            break;
        case (EFetchBookmark):
            fetchBookMarkData(aRequest);
            break;
        case (EFetchAllBookmarks):
            fetchBookMarkData(aRequest);
            break;
        default:
            result=false;
            break;
        }
    return result;
    }

/**==============================================================
 * Description:Adds the bookmark to the database
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::addBookMark(WRT::ServiceIPCRequest *aRequest)
    {

    QString msg = aRequest->getData();
    QStringList list = msg.split(BOOKMARKSENGINESEPARATOR);

    QString title = list[0];
    QString url = list[1];
    QString adate = list[2];
    QString tags = list[3];

    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    if (db.isOpen())
        {

        QSqlQuery query(db);
        query.prepare("INSERT INTO BookMarkTable (title, url, adate, tags) "
            "VALUES (:title, :url, :adate, :tags)");

        query.bindValue(":title", QVariant(title));
        query.bindValue(":url", QVariant(url));
        query.bindValue(":adate", QVariant(adate));
        query.bindValue(":tags", QVariant(tags));
        query.exec();
        QSqlError error = query.lastError();

        }
    QString strResponse;
    WRT::ClientInfo* clientInfo =
            const_cast<WRT::ClientInfo*> (aRequest->clientInfo());
    strResponse.append(QString::number(EServerMsgBookmarkAdded));
    strResponse.append(BOOKMARKSENGINESEPARATOR);
    strResponse.append(title);
    strResponse.append(BOOKMARKSENGINESEPARATOR);
    aRequest->write(strResponse.toAscii());
    aRequest->completeRequest();
    m_server->broadcast(strResponse.toAscii());

    }

/**==============================================================
 * Description:Deletes the bookmark from the database
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::deleteBookMark(WRT::ServiceIPCRequest *aRequest)
    {

    QString msg = aRequest->getData();
    QStringList list = msg.split(";");
    QString title = list[0];
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    if (db.isOpen())
        {
        QSqlQuery query(db);
        query.prepare("DELETE FROM BookMarkTable WHERE title=:title");
        query.bindValue(":title", title);
        query.exec();
        QSqlError error = query.lastError();
        }
    QString strResponse;
    strResponse.append(QString::number(EServerMsgBookmarkDeleted));
    strResponse.append(BOOKMARKSENGINESEPARATOR);
    strResponse.append(title);
    strResponse.append(BOOKMARKSENGINESEPARATOR);
    aRequest->write(strResponse.toAscii());
    aRequest->completeRequest();
    }

/**==============================================================
 * Description:Fetches the bookmark data from the database
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::fetchBookMarkData(WRT::ServiceIPCRequest *request)
    {

    QSqlDatabase db = QSqlDatabase::database(m_connectionName);

    QString strResponse;

    bool dbopen = db.isOpen();

    if (dbopen)
        {

        QSqlQuery query(db);

        query.prepare("SELECT title,url,adate,tags FROM BookMarkTable");
        query.exec();
        QSqlError error = query.lastError();

        while (query.next())
            {

            QString title = query.value(0).toString();
            QString url = query.value(1).toString();
            QString date = query.value(2).toString();
            QString tag = query.value(3).toString();

            strResponse.append(title);
            strResponse.append(BOOKMARKSENGINESEPARATOR);
            strResponse.append(url);
            strResponse.append(BOOKMARKSENGINESEPARATOR);
            strResponse.append(tag);
            strResponse.append(BOOKMARKSENGINESEPARATOR);
            strResponse.append(date);
            strResponse.append(BOOKMARKSENGINESEPARATOR);
            }
        }

    WRT::ClientInfo* clientInfo =
            const_cast<WRT::ClientInfo*> (request->clientInfo());
    QByteArray res = strResponse.toAscii();

    request->write(strResponse.toAscii());
    request->completeRequest();

    }

/**==============================================================
 * Description:Handles the client cancel request
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::handleCancelRequest(WRT::ServiceIPCRequest *aRequest)
    {
    qDebug() << " Not supported Right now";
    }

/**==============================================================
 * Description:Handles the client connection requests
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::handleClientConnect(WRT::ClientInfo *aClient)
    {

    int sessionID = aClient->sessionId();
    m_clients.insert(sessionID, aClient->name());
    m_sessionCount++;

    }

/**==============================================================
 * Description:Handles the client disconnect request
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookMarkServer::handleClientDisconnect(WRT::ClientInfo *aClient)
    {
    int sessionID = aClient->sessionId();
    QMutableHashIterator<int, QString> itr(m_clients);
    while (itr.hasNext())
        {
        itr.next();
        if (itr.key() == sessionID)
            {
            itr.remove();
            break;
            }
        }

    if (--m_sessionCount <= 0)
        {
        closeServer();
        
        }

    }

/**==============================================================
 * Description:Handles the creation of bookmarks databse
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookMarkServer::createDatabase()
    {

    m_connectionName = "bookmark_";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    db.setHostName("Simulator");
    db.setDatabaseName(dbLocation);

    if (!db.open())
        return -1; 

    //Check if the table exists
    QStringList tablelist = db.tables(QSql::Tables);
    if (tablelist.count() == 0) {
        QSqlQuery query(db);
        query.exec(
                "CREATE TABLE BookMarkTable (title VARCHAR(40) NOT NULL,url VARCHAR(100),adate VARCHAR(40),tags VARCHAR(40),CONSTRAINT pk_BookMarkTable PRIMARY KEY(title))");

        }
    
  
    return 0;
    }

