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

#include <QCoreApplication>
#include<BookMarkServerTestui.h>
#include<QtGui>

int main( int argc, char **argv )
{
    QApplication app(argc, argv);
    BookMarkServerTestui obj;
    //QWidget* win=new QWidget();
    QWidget win;
    win.setWindowTitle("BOOKMARKS SERVER TESTUI");
    QPushButton* butadd=new QPushButton("Add Bookmark Google" );
    QPushButton* butadd1=new QPushButton("Add Bookmark yahoo" );
    QPushButton* butadd2=new QPushButton("Add Bookmark Nokia" );
    
    QPushButton* butdel=new QPushButton("del Bookmark Google" );
    QPushButton* butdel1=new QPushButton("del Bookmark Yahoo" );
    QPushButton* butdel2=new QPushButton("del Bookmark Nokia" );
    
    QPushButton* butfetch=new QPushButton("Fetch Bookmark" );
    QPushButton* butexit=new QPushButton("Exit Server" );
    QPushButton* Exitapp=new QPushButton("Exit UI" );
    //Add 
    QObject::connect(butadd,SIGNAL(clicked()),&obj,SLOT(buttonclicked()));
    QObject::connect(butadd1,SIGNAL(clicked()),&obj,SLOT(yahooclicked()));
    QObject::connect(butadd2,SIGNAL(clicked()),&obj,SLOT(nokiaclicked()));
    
    QObject::connect(butdel,SIGNAL(clicked()),&obj,SLOT(delbutton()));
    QObject::connect(butdel1,SIGNAL(clicked()),&obj,SLOT(delyahoo()));
    QObject::connect(butdel2,SIGNAL(clicked()),&obj,SLOT(delnokia()));
    
    
    QObject::connect(butfetch,SIGNAL(clicked()),&obj,SLOT(fetchbutton()));
    //QObject::connect(butexit,SIGNAL(clicked()),&obj,SLOT(closeAll()));
    QObject::connect(Exitapp,SIGNAL(clicked()),&app,SLOT(quit()));
    
    
    QVBoxLayout* lay=new QVBoxLayout();
    lay->addWidget(butadd);
    lay->addWidget(butadd1);
    lay->addWidget(butadd2);
    lay->addWidget(butdel);
    lay->addWidget(butdel1);
    lay->addWidget(butdel2);
    lay->addWidget(butfetch);
    //lay->addWidget(butexit);
    lay->addWidget(Exitapp);

    win.setLayout(lay);
    //win->show();
    win.showFullScreen();
    
/*    butadd->show();    
    butdel->show();
    butfetch->show();*/
    return app.exec();
}

