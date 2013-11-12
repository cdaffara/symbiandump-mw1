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
#ifndef	UT_CPPLUGIN_H
#define	UT_CPPLUGIN_H

#include <QObject>
#include <QVector>
#include <QStringList>

class CpPluginPlatInterface;
class CpPluginLoader;
class HbMainWindow;

class TestCpPlugin : public QObject
{
	Q_OBJECT
private slots:
    void initTestCase();                // initialize test data
    //void test_loadPluginsID();          //loading plugins
    //void test_loadPluginsID_N();	//loading plugins(with wrong input)
    void test_loadPluginsName();        //loading plugins(by dll name)
    void test_loadPluginsName_N();      //loading plugins(by dll name)(with wrong input)
    //void test_uid();                    //
    void test_createSettingFormItemData();
    //void test_launcher_by_ID();     //testing launcher by id
    //void test_launcher_by_ID_N();   //testing launcher by id (with wrong input)
    void test_launcher_by_Name();   //testing launcher by name
    void test_launcher_by_Name_N(); //testing launcher by name (with wrong input)
    void cleanupTestCase();	//Finalize test data
private:
    QVector<CpPluginPlatInterface *> plist;	//
    CpPluginLoader* pLoader;						//
    QStringList  pluginDlls;
    HbMainWindow* mainWindow;
};


#endif	//UT_CPPLUGIN_H
