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
* Description:  test application for qt control panel public apis.
*
*/

#include "ut_cpapi.h"
#include "cptestpluginentryitem.h"
#include "cptestview.h"

//#include <hbmainwindow.h>
#include <QGraphicsWidget>
#include <qDebug>
#include <QtTest/QtTest>
#include <QtAlgorithms>
#include <HbView.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbdataformviewitem.h>
#include <hbabstractviewitem.h>
#include <hbpushbutton.h>
//testing following classes
#include <cpbasesettingview.h>
#include "cpsettingformentryitemdata.h"
#include "cpsettingformentryitemdataimpl.h"
#include <cpsettingformitemdata.h>
#include <cpitemdatahelper.h>
#include <cppluginutility.h>
#include <cppluginloader.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>
#include <cplogger.h>
/*!
     \class TestCpAPI 
     \brief class name: TestCpAPI \n
      class's description: \n
      type of test case: API class test \n
      test cases' number totally: 9 \n
 */
void TestCpAPI::initTestCase()
{    

}

void TestCpAPI::cleanupTestCase()
{
	QCoreApplication::processEvents();
}
/*!
    <b>Test Case Description:</b> \n&nbsp;&nbsp;
        Verify the class CpBaseSettingView can created corresponding views via different parameters.\n
     1. Fucntion Name: \n &nbsp;&nbsp;
		<1> CpBaseSettingView::CpBaseSettingView(QGraphicsWidget *widget,QGraphicsItem *parent)\n &nbsp;&nbsp;
		<2> virtual void close()\n
     2. Case Descrition: \n&nbsp;&nbsp;
		<1> test the constructor function\n &nbsp;&nbsp;
		<2> Test the close() function. \n
     3. Input Parameters: \n&nbsp;&nbsp;
		<1> widget = 0 , parent = 0 \n &nbsp;&nbsp;
          widget = 0, parent = new HbWidget () \n &nbsp;&nbsp;&nbsp;&nbsp;
          widget = new Hbpushbuton (), parent = 0 \n &nbsp;&nbsp;&nbsp;&nbsp;
          widget = new hbpushbutton(), parent  = new HbWidget() \n &nbsp;&nbsp;
	   <2> none \n
     4. Expected result:  \n &nbsp;&nbsp;
		<1> pview != 0, no crash \n &nbsp;&nbsp;
		<2> no crash \n
 */
void TestCpAPI::testCpBaseSettingView()
{
	//set the parameters as 0
    CpBaseSettingView * pview = new CpBaseSettingView(0);
	QVERIFY( pview != 0 );	
    HbDataForm *pForm = new HbDataForm(0);
    pview->setWidget(pForm);
    QVERIFY( qobject_cast<HbDataForm *>(pview->widget()) == pForm );
	delete pview;
	pview = 0;
	
	HbPushButton *widget = new HbPushButton();
	QVERIFY(widget != 0);
	// set the first parameter as widget
	CpBaseSettingView *pView1 = new CpBaseSettingView(widget);
	QVERIFY(pView1!=0);
	HbPushButton *button = qobject_cast<HbPushButton *>(pView1->widget());
	QVERIFY(button != 0);	
	delete pView1;	
	pView1 = 0;
	
	// test close() function in CpBaseSettingView class.
	CpTestView *pview3 = new CpTestView();
	pview3->testClose();
	delete pview3;
	pview3 = 0;
}
/*!
     <b>Test Case Description:</b> \n
     .	Verify this class could create the corresponding control panel defined HbDataFormModelItem. \n\n
     <b>Function 1:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
      CpSettingFormItemData(HbDataFormModelItem::DataItemType type, const QString &label, const HbDataFormModelItem *parent = 0) \n
     2. Case Descrition: \n &nbsp;&nbsp;
		Test the constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
      <1> type = HbDataFormModelItem::DataItemType,  label = QString, parent = new HbDataFormModelItem(), \n&nbsp;&nbsp;
      <2> type =  (HbDataFormModelItem::DataItemType) invaildValue, label = 0, parent = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> pdata != 0, pdata->type() == type \n &nbsp;&nbsp;
            <2> no crash  \n
     <b>Function 2:</b> \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            CpSettingFormItemData(const HbDataFormModelItem *parent = 0) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            none \n
     4. Expected result: \n &nbsp;&nbsp;
            pdata != 0 & no crash \n
	 <b>Function 3:</b> \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            QString text() const; \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it returns the right text string. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            none \n
     4. Expected result: \n &nbsp;&nbsp;
            return the correct text string \n
	 <b>Function 4:</b> \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            void setText(const QString &text); \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify that the text can be set via this function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            text = QString(xx)
     4. Expected result: \n &nbsp;&nbsp;
            pItemData->text() ==  text \n
     <b>Function 5:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            QString description() const; \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify that it returns the correct description name. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            none \n
     4. Expected result: \n &nbsp;&nbsp;
            return the correct text string \n
     <b>Function 6:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void setDescription(const QString &description); \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it can set description successfully. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            description = QString(xx)
     4. Expected result: \n &nbsp;&nbsp;
            pItemData->description() ==  description \n
     <b>Function 7:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            QString iconName() const;\n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it can return the right icon name. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            none \n
     4. Expected result: \n &nbsp;&nbsp;
            return the correct icon name \n
     <b>Function 8:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void setIconName(const QString &icon); \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it can set the given icon name successfully.\n
     3. Input Parameters: \n &nbsp;&nbsp;
            iconName = QString(xx)
     4. Expected result: \n &nbsp;&nbsp;
            pItemData->iconNmae() ==  iconName \n
     <b>Function 9:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void setEntryItemIcon(const HbIcon &icon); \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it can set the item icon successfully. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            icon = HbIcon()
     4. Expected result: \n &nbsp;&nbsp;
            entryItemIcon() == icon \n            
*/
void TestCpAPI::testCpSettingFormEntryItemData()
{
    // create CpTestPluginEntryItemData class firstly
    CpItemDataHelper *pHelper = new CpItemDataHelper();   
    HbIcon pIcon = HbIcon("icon");
    
    // test the first constructor
    CpTestPluginEntryItemData *pItemData = new CpTestPluginEntryItemData(0);
    QVERIFY (pItemData!=0);
    delete pItemData;
    pItemData = 0;    
    
    // The second constructor
    pItemData = new CpTestPluginEntryItemData(*pHelper, "test","test", pIcon, 0);
    QVERIFY (pItemData!=0);
    
    // test settext() and text() function
    QString aText = "strText";
    pItemData->setText(aText);
    QString aTextTemp = pItemData->text();
    QVERIFY( aTextTemp == aText);
    
    // test setDescription() and description() function    
    QString aDescription = "strDescription";
    pItemData->setDescription(aDescription);
    QString aDesTemp = pItemData->description();
    QVERIFY( aDesTemp == aDescription);
    
    // test setIconName() and iconName() function    
    QString aIconName = "strIconName";
    pItemData->setIconName(aIconName);
    QString aIconTemp = pItemData->iconName();
    QVERIFY( aIconTemp == aIconName);
    
    // test setEntryItemIcon() and entryItemIcon() function 
    pItemData->setEntryItemIcon(pIcon);
    HbIcon pIconTemp = pItemData->entryItemIcon();
    QVERIFY( pIconTemp == pIcon);
    
    // test onLaunchView() function
    pItemData->onLaunchView();
    
    delete pItemData;
    pItemData = 0;
    delete pHelper;
    pHelper = 0;
}

/*!
     Test Case Description: \n
         Verify this class could create the corresponding CpSettingFormEntryItemData. \n
	 <b>Function 1:</b> \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            explicit CpSettingFormEntryItemDataImpl(CpItemDataHelper &itemDataHelper,const QString &text = QString(), \n
                                                    const QString &description = QString(),const HbIcon &icon = HbIcon(), \n
                                                    const HbDataFormModelItem *parent = 0) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the first constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> itemDataHelper = CpItemDataHelper, text = Qstring(XX),description = Qstring(XX),
            	  icon = HbIcon(), parent = new HbDataFormModelItem() \n &nbsp;&nbsp;
            <2> itemDataHelper = CpItemDataHelper , text = Qstring(),
            	  description = Qstring(), icon = HbIcon(), parent = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1>  pdataimp != 0 \n &nbsp;&nbsp;
            <2> no crash  \n
    <b>Function 2:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
        explicit CpSettingFormEntryItemDataImpl(HbDataForm *dataForm,const QString &text = QString(), \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
        																				const QString &description = QString(),const HbIcon &icon = HbIcon(), \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
        																				   const HbDataFormModelItem *parent = 0) \n 
     2.  Case Descrition: \n &nbsp;&nbsp;
         Test the second constructor function. \n
     3.  Input Parameters: \n &nbsp;&nbsp;
            <1> dataform = new HbDataForm(), text = Qstring(XX), description Qstring(XX), 
            	  icon = HbIcon(XX), parent = new HbDataFormModelItem(), \n &nbsp;&nbsp;
						<2> dataForm = 0, text = Qstring(), description Qstring(), icon = HbIcon(), parent = 0 \n
     4.  Expected result: \n &nbsp;&nbsp;
            pdata != 0 \n      
     <b>Function 3:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
         explicit CpSettingFormEntryItemDataImpl(EntryItemType type,CpItemDataHelper &itemDataHelper, \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
         																				 const QString &text = QString(),const QString &description = QString(), \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
         																				 const QString &icon = QString(),const HbDataFormModelItem *parent = 0)  \n 
     2. Case Descrition: \n &nbsp;&nbsp;
         Test the third constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> type = HbDataFormModelItem::DataItemType,  itemDataHelper = CpItemDataHelper, \n &nbsp;&nbsp; &nbsp;&nbsp;
            	  text = QString(XX),  description = QString(XX),  icon = QString(XX), parent = new HbDataFormModelItem() \n &nbsp;&nbsp;
			<2> type = HbDataFormModelItem::DataItemType,  itemDataHelper = CpItemDataHelper default, \n &nbsp;&nbsp;
							  text = QString(),  description = QString(),  icon = QString(), parent = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> pdataimp != 0, pdataimp->icon() != icon,\n &nbsp;&nbsp;
			<2> pdataimp != 0, pdataimp->icon() == 0, \n            
     <b>Function 4:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
         explicit CpSettingFormEntryItemDataImpl(EntryItemType type,HbDataForm *dataForm,const QString &text = QString(),\n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
            																		 const QString &description = QString(),const QString &icon = QString(),\n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
            																		 const HbDataFormModelItem *parent = 0) \n
     2. Case Descrition: \n &nbsp;&nbsp;
         Test the forth constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> type = HbDataFormModelItem::DataItemType, dataform = new HbDataForm(),   \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
            	  text = QString(XX),  description = QString(XX),  icon = QString(XX), parent = new HbDataFormModelItem() \n &nbsp;&nbsp;
            <2> type = HbDataFormModelItem::DataItemType,  dataform = 0, text = QString(),   \n &nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp;
            		description = QString(),  icon = QString(), parent = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> pdataimp != 0, pdataimp->icon() != icon,\n &nbsp;&nbsp;
			<2> pdataimp != 0, pdataimp->icon() == 0, \n   						    
*/
void TestCpAPI::testCpSettingFormEntryItemDataImpl()
{
    //Define the parameters for the constructors.
    QString strText = "testText";
    QString strDes = "testDescription";
    QString strIcon = "iconString";
    CpItemDataHelper *pHelper = new CpItemDataHelper();
    HbDataFormModelItem *aParent = new HbDataFormModelItem();
    HbIcon icon = HbIcon("icon");  
    HbDataForm *pDataform = new HbDataForm();
    CpSettingFormEntryItemData::EntryItemType EType1 = CpSettingFormEntryItemData::ListEntryItem;
    CpSettingFormEntryItemData::EntryItemType EType2 = CpSettingFormEntryItemData::ButtonEntryItem;
    // test the first constructor function.
    CpSettingFormEntryItemDataImpl<CpBaseSettingView> *pdataimp =
            new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper, strText, strDes, icon, aParent ); // first constructor
    QVERIFY (pdataimp != 0);
    QVERIFY( pdataimp->text() == strText );
    QVERIFY( pdataimp->description()==strDes );
    QVERIFY( pdataimp->icon() == icon.iconName() );
    delete pdataimp;
    pdataimp = 0; 
    pdataimp = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( *pHelper, QString(), QString(), icon, 0);
    QVERIFY (pdataimp != 0);
    delete pdataimp;
    pdataimp = 0; 
        
    pdataimp = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(0); // set parameter as "0"
    QVERIFY (pdataimp != 0);
    QVERIFY( pdataimp->text() == 0 );
    QVERIFY( pdataimp->description()==0 );
    delete pdataimp;
    pdataimp = 0;
    // test the second constructor function.
    pdataimp = new CpSettingFormEntryItemDataImpl <CpBaseSettingView> ( pDataform,strText, strDes, icon, aParent ); //second contructor
    QVERIFY( pdataimp->text() == strText );
    QVERIFY( pdataimp->description()==strDes );
    QVERIFY( pdataimp->icon() == icon.iconName() );
    delete pdataimp;
    pdataimp = 0;
    pdataimp = new CpSettingFormEntryItemDataImpl <CpBaseSettingView> ( 0, QString(), QString(), icon, 0 );
    QVERIFY (pdataimp != 0);
    delete pdataimp;
    pdataimp = 0; 
    // test the third constructor function.
    pdataimp = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>( EType1,*pHelper, strText, strDes, strIcon, aParent );// the third constructor
    HbDataFormModelItem::DataItemType EdataItmeTypeTemp1 = pdataimp->type();
    QVERIFY (EdataItmeTypeTemp1 == EType1);
    QVERIFY( pdataimp->text() == strText );
    QVERIFY( pdataimp->description()==strDes );
    QVERIFY( pdataimp->icon() == strIcon );
    delete pdataimp;
    pdataimp = 0;    
    // test the forth constructor function.
    pdataimp = new CpSettingFormEntryItemDataImpl<CpBaseSettingView>(EType2,pDataform, strText, strDes, strIcon, aParent );// the fourth constructor
    CpSettingFormEntryItemData::DataItemType EdataItmeTypeTemp2 = pdataimp->type();
    QVERIFY (EdataItmeTypeTemp2 == EType2);
    QVERIFY( pdataimp->text() == strText );
    QVERIFY( pdataimp->description()==strDes );
    QVERIFY( pdataimp->icon() == strIcon );
    delete pdataimp;
    pdataimp = 0;
  
    delete pHelper;
    pHelper=0;
    delete pDataform;
    pDataform = 0;
}
/*!
		provide test data for the testCpSettingFormItemData()
*/
void TestCpAPI::testCpSettingFormItemData_data()
{
    // add the test data as below
    QTest::addColumn<int>("aType");
    QTest::addColumn<QString>("aLabel");
    QTest::newRow("FormPageItem value") << (int)HbDataFormModelItem::FormPageItem << "FormPageItem Label";
    QTest::newRow("GroupItem value") << (int)HbDataFormModelItem::GroupItem << "GroupItem Label";
    QTest::newRow("GroupPageItem value") << (int)HbDataFormModelItem::GroupPageItem << "GroupPageItem Label";
    QTest::newRow("TextItem value") << (int)HbDataFormModelItem::TextItem << "TextItem Label";
    QTest::newRow("SliderItem value") << (int)HbDataFormModelItem::SliderItem << "SliderItem Label";
    QTest::newRow("VolumeSliderItem value") << (int)HbDataFormModelItem::VolumeSliderItem << "VolumeSliderItem Label";
    QTest::newRow("CheckBoxItem value") << (int)HbDataFormModelItem::CheckBoxItem << "CheckBoxItem Label";
    QTest::newRow("ToggleValueItem value") << (int)HbDataFormModelItem::ToggleValueItem << "ToggleValueItem Label";
    QTest::newRow("RadioButtonListItem value") << (int)HbDataFormModelItem::RadioButtonListItem << "RadioButtonListItem Label";
    QTest::newRow("MultiselectionItem value") << (int)HbDataFormModelItem::MultiselectionItem << "MultiselectionItem Label";
    QTest::newRow("ComboBoxItem value") << (int)HbDataFormModelItem::ComboBoxItem << "ComboBoxItem Label";    
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
         Verify this class could create the corresponding control panel defined HbDataFormModelItem. \n
     <b>Function 1:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            CpSettingFormItemData(HbDataFormModelItem::DataItemType type, const QString &label,const HbDataFormModelItem *parent = 0); \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the first constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> type = HbDataFormModelItem::DataItemType,  label = QString, parent = new HbDataFormModelItem() \n &nbsp;&nbsp;
			<2> type = (HbDataFormModelItem::DataItemType) invaildValue, label = 0, parent = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> pdata != 0, pdata->type() == type \n &nbsp;&nbsp;
			<2> no crash \n	
     <b>Function 2:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            CpSettingFormItemData(const HbDataFormModelItem *parent = 0 \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the second constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> parent = new HbDataFormModelItem() \n
     4. Expected result: \n &nbsp;&nbsp;
			<1> pdata != 0, no crash \n										    
*/
void TestCpAPI::testCpSettingFormItemData()
{
    // Fectch the test data from testCpSettingFormItemData_data()
    QFETCH(int, aType);    
    QFETCH(QString, aLabel);
    // test the constructor using the above data
    HbDataFormModelItem::DataItemType itemType = (HbDataFormModelItem::DataItemType)aType;
    CpSettingFormItemData *pdata=new CpSettingFormItemData(itemType,aLabel);
    QVERIFY( pdata !=0 );
    HbDataFormModelItem::DataItemType itemTemp=pdata->type();
    QVERIFY(itemType==itemTemp);
    delete pdata;
    pdata=0;
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
         To verify all the functions in CpItemdataHelper class. \n
     <b>Function 1:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            explicit CpItemDataHelper(HbDataForm *form = 0) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the first constructor function. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> form = 0 \n
						<2> form = new HbDataForm() \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> pHelper != 0 \n &nbsp;&nbsp;
            <2> pHelper != 0 \n 
     <b>Function 2:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void bindToForm(HbDataForm *form) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> form = 0 \n &nbsp;&nbsp;
			<2> form = new HbDataForm() \n
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n				
     <b>Function 3:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void addItemPrototype(HbAbstractViewItem *prototype) \n 
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> prototype = HbDataFormViewItem() \n &nbsp;&nbsp;
			<2> prototype = 0 \n
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n				
     <b>Function 4:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void addConnection(HbDataFormModelItem *item, const char *signal,  QObject *receiver,const char *method) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> item = new HbDataFormModelItem(), signal = char*, receiver = Qobject*, method = const char* \n &nbsp;&nbsp;
			<2> set all parameters as '0' \n
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n									
     <b>Function 5:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void removeConnection(HbDataFormModelItem *item, const char *signal, Qobject *receiver, const char *method) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> item = new HbDataFormModelItem(), signal = char*, receiver = Qobject*, method = const char*\n &nbsp;&nbsp;
			<2> set all parameters as '0'
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n	
     <b>Function 6:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            void connectToForm(const char *signal, QObject *receiver, const char *method) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> signal = char*, receiver = Qobject*, method = const char* \n &nbsp;&nbsp;
			<2> set all parameters as '0' \n
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n						
     <b>Function 7:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            HbWidget *widgetFromModelIndex(const QModelIndex &index) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1>  index = const QModelIndex \n						
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n			
     <b>Function 8:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            HbDataFormModelItem *modelItemFromModelIndex(const QModelIndex &index) \n
     2. Input Parameters: \n &nbsp;&nbsp;
            <1> index =  const QModelIndex() \n &nbsp;&nbsp;
			<2> set all parameters as '0' \n			
     3. Expected result: \n &nbsp;&nbsp;
			no crash \n																													    
*/
void TestCpAPI::testItemDataHelper()
{
    HbDataForm* form = new HbDataForm(); // create a dataform
    HbDataFormModel *model = new HbDataFormModel(); 
    HbDataFormModelItem *general = model->appendDataFormPage(QString("General")); // add a dataformpage
    CpSettingFormItemData *mItem = new CpSettingFormItemData(HbDataFormModelItem::SliderItem,
            QString("New Slider")); 
    general->appendChild(mItem); // add a slider to the dataformgroup, use it as a parameter when testing addConnection() function
    form->setModel(model);    
        
    // test constructor using default parameter
    CpItemDataHelper *phelperdefault = new CpItemDataHelper(0);
    QVERIFY (phelperdefault!=0);
    
    // test constructor using a HbDataForm parameter
    CpItemDataHelper *phelperForm = new CpItemDataHelper(form);
    QVERIFY (phelperForm!=0);
    
    // test bindtoForm()
    phelperForm->bindToForm(0);
    phelperForm->bindToForm(form);
    
    // test the addItemPrototype() function
    int oldProtoTypeListNr = form->itemPrototypes().count();
    HbDataFormViewItem *pitem = new HbDataFormViewItem();
    phelperForm->addItemPrototype(pitem); 
    int newProtoTypeListNr = form->itemPrototypes().count();
    //Verify that the prototype is added to the prototypelist
    QVERIFY( newProtoTypeListNr == oldProtoTypeListNr + 1 );    
    
    phelperForm->addItemPrototype(0); // pitem = 0
    
    // test addConnection() function     
    char aSignal[] = "asignal";
    char aMethod[] = "amethod";
    phelperForm->addConnection( general, aSignal,mItem,aMethod);    
    phelperForm->removeConnection(general,aSignal,mItem, aMethod);    
    phelperForm->addConnection( 0,0,0,0 );
    phelperForm->removeConnection(0,0,0,0);
    phelperdefault->addConnection(general, aSignal,mItem,aMethod);
    
    // test connectToForm() function
    phelperForm->connectToForm(aSignal,mItem, aMethod);
    phelperForm->disconnectFromForm(aSignal,mItem, aMethod);
    phelperForm->connectToForm(0,0,0);
    phelperForm->disconnectFromForm(0,0,0); 
    phelperdefault->connectToForm(aSignal,mItem, aMethod);
    
    // test widgetFromModelIndex() function
    QModelIndex aIndex = model->indexFromItem(mItem);
    HbWidget * widget = phelperForm->widgetFromModelIndex(aIndex);
    QVERIFY( widget->type() == Hb::ItemType_Slider );
    
    // test modelItemFromModelIndex() function
    HbDataFormModelItem *itemTest = phelperForm->modelItemFromModelIndex(aIndex);
    HbDataFormModelItem *itemGeneral = model->itemFromIndex(aIndex);
    QVERIFY(itemTest == itemGeneral);
  
    delete phelperdefault;
    phelperdefault = 0;
    delete phelperForm;
    phelperForm = 0;
    delete form;
    form = 0;
    delete model;
    
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
         Verify the class can add the control panel's proto type of setting items( only entry item currently) to the settingForm \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            static void addCpItemPrototype(HbDataForm *settingForm) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify it can add the control panel's proto type of setting items( only entry item currently) to the settingForm \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> settingForm = new HbDataForm () \n
						<2> settingForm = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> The proto type is added to the prototype list \n						    
*/
void TestCpAPI::testCpPluginUtility()
{
    HbDataForm* form = new HbDataForm();
    int oldListNr = form->itemPrototypes().count();
    CpPluginUtility::addCpItemPrototype(form); // set settingForm = new HbDataForm ()
    int newListNr = form->itemPrototypes().count();
    QVERIFY( newListNr = oldListNr + 1 );
    
    CpPluginUtility::addCpItemPrototype(0); // set settingForm = 0
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
         To verify the class could load the control panel plugin. \n
     <b>Function 1:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            static CpPluginInterface *loadCpPluginInterface(const QString &pluginFile) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify that it can load the cp interface successfully. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> pluginFile = const Qstring (a vaild plugin file) \n &nbsp;&nbsp;
			<2> pluginFile = const Qstring (an invaild plugin file) \n  &nbsp;&nbsp;
			<3> pluginFile = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            no crash	\n			
     <b>Function 2:</b> \n
	 1. Fucntion Name: \n &nbsp;&nbsp;
            static CpLauncherInterface   *loadCpLauncherInterface(const QString &pluginFile) \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Verify that it can load the cp cplaucher interface successfully. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> pluginFile = const Qstring (a vaild plugin file) \n &nbsp;&nbsp;
			<2> pluginFile = const Qstring (an invaild plugin file) \n  &nbsp;&nbsp;
			<3> pluginFile = 0 \n
     4. Expected result: \n &nbsp;&nbsp;
            no crash					            	    
*/
void TestCpAPI::testCpPluginLoader()
{
    // the plugins for test are created via the codes in tsrc/unit/pluginfortest.
    QString aInvaildFile = "non_existing_plugin.dll";
    QString aVaildFile1 = "firstpluginfortest";
    QString aVaildFile2 = "secondpluginfortest.dll";
    QString aVaildFile3 = "thirdpluginfortest.qtplugin";

    CpPluginInterface *pluginInterface = CpPluginLoader::loadCpPluginInterface(aVaildFile1);
    QVERIFY(pluginInterface != 0);

    pluginInterface = CpPluginLoader::loadCpPluginInterface(aVaildFile2);
    QVERIFY(pluginInterface != 0);

    pluginInterface = CpPluginLoader::loadCpPluginInterface(aVaildFile3);
    QVERIFY(pluginInterface != 0);

    //Test the 'loadCpLauncherInterface' function
    CpLauncherInterface *pluginLauncherInterface = CpPluginLoader::loadCpLauncherInterface(aInvaildFile);
    QVERIFY(pluginLauncherInterface == 0);  

    pluginLauncherInterface = CpPluginLoader::loadCpLauncherInterface(aVaildFile1);
    QVERIFY(pluginLauncherInterface != 0);

    pluginLauncherInterface = CpPluginLoader::loadCpLauncherInterface(aVaildFile2);
    QVERIFY(pluginLauncherInterface != 0);

    pluginLauncherInterface = CpPluginLoader::loadCpLauncherInterface(aVaildFile3);
    QVERIFY(pluginLauncherInterface != 0);
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
        Test the CP interface can work well. \n
     1. Fucntion Name: \n &nbsp;&nbsp;
            virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test the create settingformItemData() function, it's a pure vitual function. Define a simple class  and test its interface. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> itemDataHelper = CpItemDataHelper \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> no crash \n    					            	    
*/
void TestCpAPI::testCpPluginInterface()
{
    CpItemDataHelper *helper = new CpItemDataHelper();
    CpPluginInterface *plugin = CpPluginLoader::loadCpPluginInterface("firstpluginfortest");
    QVERIFY( plugin != 0 );
    QList<CpSettingFormItemData*> itemData = plugin->createSettingFormItemData(*helper);
    QVERIFY( itemData.size() == 1 );
    qDeleteAll(itemData.begin(),itemData.end());
    itemData.clear();
}
/*!
     <b>Test Case Description:</b> \n &nbsp;&nbsp;
        Test the CP launcher interface can work well.\n
     1. Fucntion Name: \n &nbsp;&nbsp;
            virtual CpBaseSettingView *createSettingView(const QVariant &hint) const  \n
     2. Case Descrition: \n &nbsp;&nbsp;
            Test it can create setting view after loading the plugin. \n
     3. Input Parameters: \n &nbsp;&nbsp;
            <1> hint = Qvariant() \n
     4. Expected result: \n &nbsp;&nbsp;
            <1> no crash \n     					            	    
*/
void TestCpAPI::testCpLancherInterface()
{
    QVariant hint = QVariant();
    CpLauncherInterface *plugin = CpPluginLoader::loadCpLauncherInterface("secondpluginfortest");
    QVERIFY( plugin != 0 );
    CpBaseSettingView *testView = plugin->createSettingView(hint);    
    QVERIFY( testView != 0 );
    delete testView;
    testView = 0; 
}

void TestCpAPI::testCpLogger()
{
}

QTEST_MAIN(TestCpAPI)
