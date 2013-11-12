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

#include<QString>
#include<bookmarkscommonengine.h>
#include<bookmarkclient.h>
#include <serviceipc.h>
#include<serviceipcclient.h>
#include<QDebug>

class BookmarksClientSidePrivate
    {
BOOKMARKSCLIENT_PUBLIC(BookmarksClientSide)
  
public:
    ~BookmarksClientSidePrivate();
    QString m_clientName;
    int m_clientuid;
    bool m_isConnected;
    int m_error;
    WRT::ServiceFwIPC* m_IpcSession;
    int dispatchServerMessagestoClient(int msgType, QString title);
    };

/**==============================================================
 * Description: Constructor of BookmarksClientSidePrivate
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookmarksClientSidePrivate::~BookmarksClientSidePrivate()
    {
    if (m_IpcSession)
        {
        m_IpcSession->disconnect();
        delete m_IpcSession;
        m_IpcSession = NULL;
        }

    }

/**==============================================================
 * Description: Constructor of BookmarksClientSide
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookmarksClientSide::BookmarksClientSide()
    {
    BOOKMARKSCLIENT_INITIALIZE(BookmarksClientSide);

    priv->m_IpcSession = new WRT::ServiceFwIPC(this);

    }

/**==============================================================
 * Description: Connects to the bookmark server
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookmarksClientSide::connectToServer()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    bool connected = false;
    if (priv->m_IpcSession)
        {
        int retry(5);
        for (;;)
            {
            connected = priv->m_IpcSession->connect(BOOKMARKSENGINESERVER);
            if (connected)
                {
                break;
                }
            else
                {
                if (!priv->m_IpcSession->startServer(BOOKMARKSENGINESERVER,
                BOOKMARKSENGINESERVEREXE))
                    { // start server failed
                        qDebug() << "Failed to Start the BOOKMARK Server.";
                        break;
                        }

                    }
                if (0 == --retry)
                    {
                    break;
                    }
                }
            }
        if (connected)
            {

            connect(priv->m_IpcSession, SIGNAL(readyRead()), this,
                    SLOT(handleServerResponse()));
            return ErrNone;
            }
        else
            {
            return ErrGeneral;
            }
        }

/**==============================================================
* Description: Disconnects the client
* Author: Chandrashekar.V
* EmpId:  10289207
================================================================*/
void BookmarksClientSide::closeServer()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    if (priv->m_IpcSession)
        {
        priv->m_IpcSession->disconnect();
        }
    }

/**==============================================================
 * Description: Destrcutor of the BookmarksClientSide
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookmarksClientSide::~BookmarksClientSide()
    {
    }

/**==============================================================
 * Description: Sends add bookmark request to the server
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookmarksClientSide::AddBookmark(
        BookmarkLeaf* BookmarkContent)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    QString data;
    bool result = false;
    data.append(BookmarkContent->getTitle());
    data.append(BOOKMARKSENGINESEPARATOR);
    data.append(BookmarkContent->getUrl());
    data.append(BOOKMARKSENGINESEPARATOR);
    data.append(BookmarkContent->getDate().toString("dd.MM.yyyy"));
    data.append(BOOKMARKSENGINESEPARATOR);
    data.append(BookmarkContent->getTag());
    QString requestType;
    requestType.append(QString::number(EAddBookmark));
    if (priv->m_IpcSession)
        {
        result = priv->m_IpcSession->sendSync(requestType, data.toAscii());
        }

    if (result == true)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }
    }

/**==============================================================
 * Description: Sends add fetchbookmark request to the server
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
BookmarkLeaf* BookmarksClientSide::FetchBookmark(
        QString title)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    BookmarkLeaf *node = NULL;
    QString data;
    data.append(title);
    QString requestType;
    requestType.append(QString::number(EFetchBookmark));
    if (priv->m_IpcSession)
        {
        priv->m_IpcSession->sendSync(requestType, data.toAscii());
        QByteArray msg = priv->m_IpcSession->readAll();
        QString serverMsg;
        serverMsg = msg.data();
        QStringList list = serverMsg.split(BOOKMARKSENGINESEPARATOR);

        QString title = list[0];
        QString url = list[1];
        QString strdate = list[2];
        QString tags = list[3];
        QDate adate = QDate::fromString(strdate, "dd.MM.yyyy");

        node = new BookmarkLeaf();
        node->setTitle(title);
        node->setUrl(url);
        node->setDate(adate);
        node->setTag(tags);
        }
    return node;
    }

/**==============================================================
 * Description: Sends add deletebookmark request to the server
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookmarksClientSide::DeleteBookmark(
        QString title)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    QString data;
    bool result = false;
    data.append(title);
    QString requestType;
    requestType.append(QString::number(EDeleteBookmark));
    if (priv->m_IpcSession)
        {
        result = priv->m_IpcSession->sendSync(requestType, data.toAscii());
        }
    if (result == true)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }

    }

/**==============================================================
 * Description: gets session Id of the client
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookmarksClientSide::getSessionId(
        int& BookmarkId)
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    bool result = false;
    if (priv->m_IpcSession)
        {
        result = priv->m_IpcSession->getSessionId(BookmarkId);
        }
    if (result == true)
        {
        return ErrNone;
        }
    else
        {
        return ErrGeneral;
        }
    }

/**==============================================================
 * Description: Sends add fetchAllbookmark request to the server
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
QList<BookmarkLeaf*> BookmarksClientSide::FetchAllBookmarks()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    QList<BookmarkLeaf*> nodeslist;
    QString data;
    data.append(NULL);
    QString requestType;
    requestType.append(QString::number(EFetchAllBookmark));

    if (priv->m_IpcSession)
        {
        priv->m_IpcSession->sendSync(requestType, data.toAscii());
        QByteArray msg = priv->m_IpcSession->readAll();
        QString serverMsg;
        serverMsg = msg.data();
        QStringList list = serverMsg.split(BOOKMARKSENGINESEPARATOR);
        int count = list.count() / 4;
        int i = 0;
        while (count--)
            {
            QString title = list[i++];
            QString url = list[i++];
            QString strdate = list[i++];
            QDate adate = QDate::fromString(strdate, "dd.MM.yyyy");
            QString tags = list[i++];
            BookmarkLeaf* node = new BookmarkLeaf();
            node->setTitle(title);
            node->setUrl(url);
            node->setDate(adate);
            node->setTag(tags);

            nodeslist.append(node);
            }
        }
    return nodeslist;

    }

/**==============================================================
 * Description: handles server responses
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
void BookmarksClientSide::handleServerResponse()
    {
    BOOKMARKSCLIENT_PRIVATEPTR(BookmarksClientSide);
    qDebug() << " handling server response" << '\n';
    QByteArray msg;
    msg = priv->m_IpcSession->readAll();
    QString serverMsg;
    serverMsg = msg.data();
    qDebug() << " Message is " << serverMsg << '\n';
    QStringList list1 = serverMsg.split(BOOKMARKSENGINESEPARATOR);
    int msgType = list1[0].toLocal8Bit().toInt();
    QString title = list1[1];
    int res=priv->dispatchServerMessagestoClient(msgType, title);

    }

/**==============================================================
 * Description: Sends server response notifications to the UI
 * Author: Chandrashekar.V
 * EmpId:  10289207
 ================================================================*/
int BookmarksClientSidePrivate::dispatchServerMessagestoClient(int msgType,
        QString title)
    {
    BOOKMARKSCLIENT_PUBLICPTR(BookmarksClientSide);
    switch (msgType)
        {
        case EServerMsgBookmarkDeleted:
            emit pub->BookmarkDeleted(title);
            break;
        case EServerMsgBookmarkAdded:
            emit pub->BookmarkAdded(title);
            break;
        default:
            break;
        }
    return ErrNone;
    }

