/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include<BookMarkServerTestui.h>
#include <bookmarkclient.h>
//#include<bookmarks.h>
BookMarkServerTestui::BookMarkServerTestui()
    {
    int sessionId=0;
    obj=new BookmarksClientSide();
    obj->connectToServer();
    bool res=obj->getSessionId(sessionId);
    }
BookMarkServerTestui::~BookMarkServerTestui()
    {
    obj->closeServer();
    delete(obj);
    obj=NULL;
    }
void BookMarkServerTestui::buttonclicked()
{
 getop();

}
void BookMarkServerTestui::getop()
    {
    BookmarkLeaf* col=new BookmarkLeaf();
  /*  col->setTitle("TestT1");
    col->setUrl("http://www.google.com");
    QDate x(2010,01,19);
    col->setDate(x);
    col->setTag("GOOGLY");*/
    QDate x(2010,01,19);
    col->setUrl("http://www.google.com");
    col->setTitle("TestT1");
    col->setDate(x);
    col->setTag("GOOGLY");
    obj->AddBookmark(col);


    }
void BookMarkServerTestui::delbutton()
    {
    
    obj->DeleteBookmark("TestT1");
    }
void BookMarkServerTestui::fetchbutton()
    {
    
    //BookmarkLeaf* fin=obj->FetchBookmark("first");
    }
void BookMarkServerTestui::closeAll()
    {
     //obj->closeServer();
     
    }
void BookMarkServerTestui::yahooclicked()
    {
    /*BookmarkLeaf* BookmarkContent=new BookmarkLeaf();
    BookmarkContent->setBookmarkTitle("second");
    BookmarkContent->setBookmarkUrl("www.yahoo.com");
    BookmarkContent->setLastUpdatedTime("11:10");
    BookmarkContent->setBookmarkTag("HHHHHHO");*/
    BookmarkLeaf* col=new BookmarkLeaf();
    QDate x(2010,01,19);
    col->setUrl("http://www.yahoo.com");
    col->setTitle("TestT2");
    col->setDate(x);
    col->setTag("YAH");
    obj->AddBookmark(col);
    
    }
void BookMarkServerTestui::nokiaclicked()
    {
    BookmarkLeaf* col=new BookmarkLeaf();
       QDate x(2010,01,19);
       col->setUrl("http://www.NOKIA.com");
       col->setTitle("TestT3");
       col->setDate(x);
       col->setTag("NOK");
    obj->AddBookmark(col);
    
    }
void BookMarkServerTestui::delyahoo()
    {
    obj->DeleteBookmark("TestT2");
    }
void BookMarkServerTestui::delnokia()
    {
    obj->DeleteBookmark("TestT3");
    }
