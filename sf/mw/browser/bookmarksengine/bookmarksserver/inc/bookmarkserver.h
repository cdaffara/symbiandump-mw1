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
#ifndef BOOKMARKSERVER_H
#define BOOKMARKSERVER_H

#include <QHash>
#include <serviceipcobserver.h>
#include <serviceipcserver.h>
#define BOOKMARKDATAMODELSERVER "BookMarkServer"

const QString dbLocation="BookMarkDatabase.db";



//using namespace WRT;
class BookMarkServer: public QObject, public WRT::MServiceIPCObserver
    {
    Q_OBJECT
    public:
        BookMarkServer();
        virtual ~BookMarkServer();
        enum BookmarkRequestTypes
            {
            EDeleteBookMark,
            EAddBookMark,
            EFetchBookmark,
            EFetchAllBookmarks,
            EUnknownrequest
            };
        enum BookmarkResponseTypes
            {
            EDeleteBookMarkDone,
            EAddBookMarkDone,
            EFetchBookmarkDone,
            EFetchAllBookmarksDone,
            EUnknownresponse
            };


    private:
        bool handleRequest(WRT::ServiceIPCRequest *aRequest);
        void handleCancelRequest(WRT::ServiceIPCRequest *aRequest);
        void handleClientConnect(WRT::ClientInfo *aNewClient);
        void handleClientDisconnect(WRT::ClientInfo *aClient);
    private:
        void fetchBookMarkData(WRT::ServiceIPCRequest *request);
        int createDatabase();
        void addBookMark(WRT::ServiceIPCRequest *request);
        void deleteBookMark(WRT::ServiceIPCRequest *request);
    private slots:
    void closeServer();
    private:
        WRT::ServiceFwIPCServer* m_server;
        QString m_connectionName;
        QHash<int,QString>m_clients;
        int m_sessionCount;
    };
#endif //BOOKMARKSERVER_H
