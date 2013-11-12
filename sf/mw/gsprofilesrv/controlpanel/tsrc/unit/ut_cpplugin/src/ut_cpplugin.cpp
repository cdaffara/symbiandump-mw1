/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/
#include "ut_cpplugin.h"
#include <hbmainwindow.h>
#include <QGraphicsWidget>
#include <qDebug>
#include <QtTest/QtTest>
#include <cpsettingformitemdata.h>

#include <cppluginplatinterface.h>
#include <cppluginloader.h>
#include <cppluginlauncher.h>
#include <cpitemdatahelper.h>

//define plugin uids, need replaced by header file
const int PluginUIDs[] =
{
    //0X20025FE2,	 //viewplugin
    //0X20025FE4,  //tonesplugin		Y
    0X20025FDF,  //communicationplugin.pro  N
    0X20025FDC,  //applicationsettingsplugin.pro    N
    //0xEEF7E3EC,  //3rdpartyappsettingsplugin.pro    Y
    0X20025FE5,  //personalizationplugin.pro    N
    0X20025FE8,  //placeholderplugin    Y
	0X20025FDD,  //placeholder no icon
	0X20025FDE,  //key touch
	0X20025FE0,  // look and feel
	0X20025FDB, // theme
	0X20025FE1, // privatcy


};
//define whether the plugin has exported view or not
const int PluginHasView[] =
{
    0, // communication
    0, // application
    0, //personalization
    1, //placeholder
    1, //placeholdernoicon
    0, //accountsplugin
    0, //privacy
	0, //device
	1, //theme
	1, //keytouch
    0  // look and feel
};

void TestCpPlugin::initTestCase()
{
    pLoader = new CpPluginLoader();
    mainWindow = new HbMainWindow;
    mainWindow->show();
	pluginDlls  << "cpcommunicationplugin"
                << "cpapplicationsettingsplugin"
                << "cppersonalizationplugin"
                << "cpplaceholderplugin"
				<< "cpplaceholdernoiconplugin"
				<< "cpaccountsplugin"
				<< "cpprivacyplugin"
				<< "cpdeviceplugin"
				<< "cpthemeplugin"
				<< "cpkeytouchfdbkplugin"
				<< "cplookfeelplugin";
}

void TestCpPlugin::test_loadPluginsName()
{
    


    int count = pluginDlls.size();
    for( int i=0; i<pluginDlls.size(); ++i )
    {
        CpPluginPlatInterface* plugin = pLoader->loadCpPlugin( pluginDlls[i] );
        qDebug()<< "<DEBUGINFO>Loading plugin ["
                << pluginDlls[i]<< "],result is:"
                << ( (plugin != 0)? "OK":"Fail" );
        QVERIFY( plugin!= 0 );
    }

}
void TestCpPlugin::test_loadPluginsName_N()
{
    CpPluginPlatInterface* plugin = pLoader->loadCpPlugin( "" );
    qDebug()<< "<DEBUGINFO>Loading plugin ["
            << "(empty name)"<< "],result is:"
            << ( (plugin == 0)? "OK":"Fail" );
    QVERIFY( plugin == 0 );
}

void TestCpPlugin::test_createSettingFormItemData()
{
    CpItemDataHelper *pHelper = new CpItemDataHelper();

    for( int i = 0; i< plist.count(); ++i)
    {
        CpPluginPlatInterface* plugin = plist[i];
        if(plugin)
        {
            CpSettingFormItemData *itemData = plugin->createSettingFormItemData(*pHelper);
            bool bToF = (itemData != 0);
            qDebug( "<DEBUGINFO>Plugin [0X%08X], result is %s", PluginUIDs[i], bToF?"OK":"Fail" );
           // delete itemData;
            QVERIFY( bToF );
        }
    }
    delete pHelper;
}

void TestCpPlugin::test_launcher_by_Name()
{
    //clear all existing view firstly
    int vcount = mainWindow->viewCount();
    int i = 0;
    for ( i=vcount-1; i>=0; --i )
    {
        QGraphicsWidget * pwgt = mainWindow->removeView( i );
        delete pwgt;
    }
    //launch plugins list by name
    int count = pluginDlls.size();
    for( i=0; i<pluginDlls.size(); ++i )
    {
        bool bret = CpPluginLauncher::launchCpPluginView( pluginDlls[i] );
        if(!PluginHasView[i])   bret = !bret;
        qDebug()<< "<DEBUGINFO>Loading plugin ["
                << pluginDlls[i]<< "],result is:"
                << ( bret? "OK":"Fail" );
        QVERIFY( bret );
    }
}
void TestCpPlugin::test_launcher_by_Name_N()
{
    bool bret = CpPluginLauncher::launchCpPluginView( "<not_existing>" );
    qDebug()<< "<DEBUGINFO>Loading plugin ["
            << "<not_existing>"<< "],result is:"
            << ( (!bret )? "OK":"Fail" );
        QVERIFY( !bret );
}

void TestCpPlugin::cleanupTestCase()
{
    for (int i = 0; i < plist.count(); ++i)
    {
        CpPluginPlatInterface* plugin = plist[i];
        if (plugin)
        {
            delete plugin;
            plugin = 0;
        }
    }
    if (pLoader)
    {
        delete pLoader;
        pLoader = 0;
    }
    delete mainWindow;
    mainWindow = 0;
}

QTEST_MAIN(TestCpPlugin)
