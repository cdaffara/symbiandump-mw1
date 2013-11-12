/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cppsmview.h"
#include <hbdataform.h>
#include <qstringlist>
#include <QDebug>
#include <hbdataformmodel.h>
#include <cpsettingformitemdata.h>
#include <hbmessagebox.h>
#include <hblineedit.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <hwrmdomainpskeys.h>
#include <e32property.h>
#include <psmclient.h>
#include <psmsettings.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <hbcheckbox.h>
#include <e32debug.h>

CpPsmView::CpPsmView(QGraphicsItem *parent) :
    CpBaseSettingView(0,parent),iCheckBoxItem(0),iTrans(new  HbTranslator("powermanagement"))
{
    RDebug::Print( _L("CpPsmView::CpPsmView:Begin") );
    iForm = new HbDataForm();
    iModel = new HbDataFormModel();
    iForm->setHeading(hbTrId("txt_power_management_subhead_power_management"));
    //our view has 3 items first one being toggle button for enabling and disabling PSM
    iPowerSaveMode = iModel->appendDataFormItem(HbDataFormModelItem::ToggleValueItem,
                                                                        hbTrId("txt_power_list_power_save_mode"));
    iPsmClient = CPsmClient::NewL( *this );
    iPsmClient->RequestPowerSaveModeNotification(); 
    TInt mode;
    TInt err = iPsmClient->PsmSettings().GetCurrentMode( mode );
    //setting of toggle value to on or off depends on the current mode                             
    if(  mode == EPsmsrvModePowerSave )    
      {
      iPowerSaveMode->setContentWidgetData(QString("text"), hbTrId("txt_power_setlabel_val_off"));
      }
    else
      {
      iPowerSaveMode->setContentWidgetData(QString("text"), hbTrId("txt_power_setlabel_val_on"));
      }

    iForm->addConnection(iPowerSaveMode,SIGNAL(clicked()), this,SLOT(toggleChange()));
         
     RDebug::Print( _L("CpPsmView::toggle button added") );
     //this is the second item which is a text item which is readonly and describes what PSM does    
     HbDataFormModelItem *dataItem2 = iModel->appendDataFormItem(HbDataFormModelItem::TextItem);
     //setting the maxrows property of hblineedit
     dataItem2->setContentWidgetData("maxRows",11);
    //setting the text property of hblineedit
     dataItem2->setContentWidgetData("text",hbTrId("txt_power_list_activating_power_save_mode_will_con"));
     dataItem2->setContentWidgetData("readOnly" , true);
     RDebug::Print( _L("CpPsmView::text item added") );
    
     //now the third item which is checkbox item for enabling psm automatically when low power
     iCheckBoxItem = iModel->appendDataFormItem(
     HbDataFormModelItem::CheckBoxItem);
     //setting the text property of check box.
     iCheckBoxItem->setContentWidgetData("text",hbTrId("txt_power_list_activate_power_save_mode_automatica"));
     RDebug::Print( _L("CpPsmView::check box added") );
      
     CRepository* repository( NULL );
     TRAPD(err1,repository =CRepository::NewL(KCRUidDeviceManagementSettings));
     TInt value;
     err1 = repository->Get( KSettingsPowerSavingQuery, value );
                                    
     if ( err1 == KErrNone )
        {                                        
        //checkbox state is initialised based on cenrep key and default value of the key is 1 (meaning unchecked)
        if(value==0)
          {   
          RDebug::Print( _L("CpPsmView::checkbox state is checked ") );	                        
          iCheckBoxItem->setContentWidgetData("checkState",2);
          }
        }
     delete repository;      
     iForm->addConnection(iCheckBoxItem,SIGNAL(stateChanged(int)),this,SLOT(checkBoxStateChanged(int)));
      
            
      err = iPsmClient->PsmSettings().GetCurrentMode( mode );
                               
      if ( err == KErrNone && mode == EPsmsrvModePowerSave  )
         {
         //if device is in powersavemode then disable the checkbox
         RDebug::Print( _L("CpPsmView::checkbox disabled ") );
         iCheckBoxItem->setEnabled(0);
         }   
    
      iForm->setModel(iModel);
      setWidget(iForm);
      RDebug::Print( _L("CpPsmView::end "));
}
 
CpPsmView::~CpPsmView()
{
RDebug::Print( _L("CpPsmView::destructor begin ") );
delete iTrans;
delete iPsmClient;
RDebug::Print( _L("CpPsmView::destructor end ") );
}


void CpPsmView::checkBoxStateChanged(int state)
{
    // store your changes
    //state=1 means ->"checked" else it is  "un-checked"
    RDebug::Print( _L("CpPsmView::checkBoxStateChanged begin ") );
    CRepository* repository( NULL );
        
    TRAPD( err, repository = CRepository::NewL( KCRUidDeviceManagementSettings ) );
        
    if ( err == KErrNone )
       {
       TInt value( 0 );
       //default value of the key is 1 meaning unchecked,so if the checkbox is checked then state=1 so key is set to 0     
       if(state)
          {
          err = repository->Set( KSettingsPowerSavingQuery, value );
          RDebug::Print( _L("CpPsmView::checkBoxStateChanged checked ") );  
          }
       else
          {
          err = repository->Set( KSettingsPowerSavingQuery, 1 );
          RDebug::Print( _L("CpPsmView::checkBoxStateChanged unchecked ") );     
          } 
       }
        
    delete repository;   
    RDebug::Print( _L("CpPsmView::checkBoxStateChanged end ") );  
  
}

void CpPsmView::toggleChange()
    {
 //   iPsmClient = CPsmClient::NewL( *this );
    RDebug::Print( _L("CpPsmView::toggleChange begin ") );
    TInt mode;
    TInt err = iPsmClient->PsmSettings().GetCurrentMode( mode );
                        
    if ( err == KErrNone  )
       {
       if( mode == EPsmsrvModeNormal || mode == EPsmsrvPartialMode )
          {
          iPsmClient->CancelPowerSaveModeNotificationRequest();
          iPsmClient->ChangePowerSaveMode( EPsmsrvModePowerSave );
          //  powerSaveMode->setContentWidgetData(QString("additionalText"), QString("off"));
          RDebug::Print( _L("CpPsmView::toggleChange psm on ") );
          }
       else
          {
          iPsmClient->CancelPowerSaveModeNotificationRequest();
          iPsmClient->ChangePowerSaveMode( EPsmsrvModeNormal );
          //  powerSaveMode->setContentWidgetData(QString("text"), QString("on"));
          RDebug::Print( _L("CpPsmView::toggleChange psm off  ") );
          }
       }
     RDebug::Print( _L("CpPsmView::toggleChange end ")  );
     }


void CpPsmView::PowerSaveModeChanged( const TPsmsrvMode aMode )
    {
    RDebug::Print( _L("CpPsmView::PowerSaveModeChanged begin ") );
    iPsmClient->RequestPowerSaveModeNotification();
    if( aMode == EPsmsrvModePowerSave )
        {
         //device in psm mode so disable checkbox and set the value of toggle button to off 
         iCheckBoxItem->setEnabled(0);
         iPowerSaveMode->setContentWidgetData(QString("text"), hbTrId("txt_power_setlabel_val_off"));
         RDebug::Print( _L("CpPsmView::PowerSaveModeChanged powersavemode enabled ") );         
        }
    else
        {
        //device is in normal mode  so enable checkbox and set the value of toggle button to on 
        iCheckBoxItem->setEnabled(1);
        iPowerSaveMode->setContentWidgetData(QString("text"), hbTrId("txt_power_setlabel_val_on"));
        RDebug::Print( _L("CpPsmView::PowerSaveModeChanged powersavemode disabled ") );   
        }
    RDebug::Print( _L("CpPsmView::PowerSaveModeChanged end ") );
    }
        
       
void CpPsmView::PowerSaveModeChangeError( const TInt aError )
      {
    RDebug::Print( _L("CpPsmView::PowerSaveModeChangeError ") );
      }

