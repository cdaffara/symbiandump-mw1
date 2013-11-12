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
#ifndef	CPPSMVIEW_H
#define	CPPSMVIEW_H

#include <cpbasesettingview.h>
#include <psmclient.h>
#include <psmsettings.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <psmclientobserver.h> // CPsmClient
#include <hbtranslator.h>

class HbDataFormModel;
class HbDataFormModelItem;
class CpSettingFormItemData;
class CPsmClient;


class CpPsmView : public CpBaseSettingView,public MPsmClientObserver
{
    Q_OBJECT
public:
    explicit CpPsmView(QGraphicsItem *parent = 0);
    virtual ~CpPsmView();
    
    void PowerSaveModeChanged( const TPsmsrvMode aMode );
        
        /**
         * @see MPsmClientObserver
         */
    void PowerSaveModeChangeError( const TInt aError );
           
private slots:
    //need handling your member's value change
    void checkBoxStateChanged(int state);
    void toggleChange();
private:
    HbDataForm *iForm;
    HbDataFormModel *iModel ;
    HbDataFormModelItem  *iCheckBoxItem;
    CPsmClient* iPsmClient;
    HbDataFormModelItem *iPowerSaveMode;
    HbTranslator *iTrans;
   
};
#endif//	CPPSMVIEW_H
