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
*		test application for qt control panel public apis.
*/
#ifndef	UT_CPAPI_H
#define	UT_CPAPI_H

#include <QObject>

class HbMainWindow;

class TestCpAPI : public QObject
{
	Q_OBJECT
private slots:
    void initTestCase();                // initialize test data
    
    void testCpBaseSettingView();				// test basesettingview functions
    void testCpSettingFormEntryItemData();
    void testCpSettingFormEntryItemDataImpl();  //test CpSettingFormEntryItemDataImpl functions    
    void testCpSettingFormItemData_data(); // provide data for "testCpSettingFormItemData()"
    void testCpSettingFormItemData();           //test CpSettingFormItemData    
    void testItemDataHelper();    //testing itemdatahelper class CpItemDataHelper
    void testCpPluginUtility();
    void testCpPluginLoader();    //test cppluginloader functions
    void testCpPluginInterface(); // test cpplugininterface functions
    void testCpLancherInterface(); // test the cpLancherInterface class
    void testCpLogger();          //test cplogger functions
    void cleanupTestCase();       // Finalize test data
private:
    HbMainWindow* mainWindow;
};

#endif	//UT_CPAPI_H
