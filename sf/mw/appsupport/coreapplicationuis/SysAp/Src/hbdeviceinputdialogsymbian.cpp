/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbWidgets module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#include "hbdeviceinputdialogsymbian.h"

#include <hbsymbianvariant.h>
#include <hbdevicedialogsymbian.h>
#include <coreapplicationuisdomainpskeys.h>

#define ARG_UNUSED(x) (void)x;

//_LIT(KDialogText, "text");

enum TInputDialogPanic {
    ELineEditPanic,
    EPropertyPanic,
    EButtonIdPanic
};

NONSHARABLE_CLASS(CHbDeviceInputDialogPrivate)  : public CBase, public MHbDeviceDialogObserver
{
public:

    void ConstructL(MHbDeviceInputDialogObserver *aObserver);
    ~CHbDeviceInputDialogPrivate();
    
    void SetButtonNull(CHbDeviceInputDialogSymbian::TButtonId aButtonId, bool aValue);
    void SetButtonTextL(CHbDeviceInputDialogSymbian::TButtonId aButtonId, const TDesC &aValue);
    void SendToServerL(bool aShow = false);
    void Close();
    bool WaitForClosed();
    void AddVariantL(const TDesC& aKey, const TAny* aData,CHbSymbianVariant::TType aDataType, CHbSymbianVariantMap* map);

public: // MHbDeviceDialogObserver
    void DataReceived(CHbSymbianVariantMap& aData);
    void DeviceDialogClosed(TInt aCompletionCode);

public: // data

    CHbDeviceInputDialogSymbian *q;
    CHbDeviceDialogSymbian *mDeviceDialog;
    bool mShowCalled;
    CHbDeviceInputDialogSymbian::TButtonId mReceivedButton;
    MHbDeviceInputDialogObserver *mObserver;
    CHbSymbianVariantMap* iVariantMap;
    
    bool *mDeleted;
    CActiveSchedulerWait mActiveSchedulerWait;
};


void CHbDeviceInputDialogPrivate::ConstructL(MHbDeviceInputDialogObserver *aObserver)
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::ConstructL::Begin") ) );
    mDeviceDialog = CHbDeviceDialogSymbian::NewL();
    iVariantMap = CHbSymbianVariantMap::NewL();
    
    

    mReceivedButton = CHbDeviceInputDialogSymbian::ERejectButton;
    mObserver = aObserver;
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::ConstructL::End") ) );
}

// Destructor
CHbDeviceInputDialogPrivate::~CHbDeviceInputDialogPrivate()
{
    // If no observer, remove observer from device dialog. This leaves the
    // dialog widget executing at server when client closes connection.
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::Destructor::Begin") ) );
    if (!mObserver) {
        mDeviceDialog->SetObserver(0);
    }
    
    delete mDeviceDialog;
    delete iVariantMap;

    // Set object deleted flag
    if (mDeleted) {
        // Mark the object as deleted.
        *mDeleted = true;
        mDeleted = 0;
    }
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::Destructor::End") ) );
}

void CHbDeviceInputDialogPrivate::AddVariantL(const TDesC& aKey,const TAny* aData,CHbSymbianVariant::TType aDataType,CHbSymbianVariantMap* map )
    {
    CHbSymbianVariant *variant = CHbSymbianVariant::NewL(aData, aDataType);
    CleanupStack::PushL(variant);
    User::LeaveIfError(map->Add(aKey, variant));
    CleanupStack::Pop(variant);
    }

// Send properties to server. Show or update.
void CHbDeviceInputDialogPrivate::SendToServerL(bool /* aShow */)
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::SendToServerL::Begin") ) );
    _LIT(KDeviceDialogType, "com.nokia.hb.deviceinputdialog/1.0");
    TInt error;
    mReceivedButton = CHbDeviceInputDialogSymbian::ERejectButton;
    error = mDeviceDialog->Show(KDeviceDialogType, *iVariantMap, this);
    User::LeaveIfError(error);
    mShowCalled = true;
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::SendToServerL::End") ) );
}


// Close input dialog
void CHbDeviceInputDialogPrivate::Close()
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::Close::Begin") ) );
    mDeviceDialog->Cancel();
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::Close::End") ) );
}

// Wait for input dialog to close
bool CHbDeviceInputDialogPrivate::WaitForClosed()
{
    // Returns true if object was not deleted during wait
    if (!mShowCalled) {
        return true;
    }
    return true;
}

// Observer, data received from device input dialog
void CHbDeviceInputDialogPrivate::DataReceived(CHbSymbianVariantMap& aData)
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::DataReceived::Begin") ) );
    mReceivedButton = CHbDeviceInputDialogSymbian::ERejectButton;
    _LIT(KKeyOk, "ok");
    _LIT(KKeyCancel, "cancel");
    const CHbSymbianVariant* variantOk = aData.Get(KKeyOk);
    const CHbSymbianVariant* variantCancel = aData.Get(KKeyCancel);
    if (variantOk) 
    {
        const TDesC *value = variantOk->Value<const TDesC>();
        q->iPassword = *value;
        mReceivedButton = CHbDeviceInputDialogSymbian::EAcceptButton;
        RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsMemoryCardCustomDialogOk );
    }
    else if(variantCancel)
    {
      const TDesC *value = variantCancel->Value<const TDesC>();
    q->iPassword = *value;
    mReceivedButton = CHbDeviceInputDialogSymbian::EAcceptButton;
    RProperty::Set( KPSUidCoreApplicationUIs, KCoreAppUIsPowerMenuCustomDialogStatus, ECoreAppUIsMemoryCardCustomDialogCancel);
    }
    	    	
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::DataReceived::End") ) );
}

// Observer, device input dialog closed
void CHbDeviceInputDialogPrivate::DeviceDialogClosed(TInt aCompletionCode)
{
    ARG_UNUSED(aCompletionCode)
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::DeviceDialogClosed::Begin") ) );

    mShowCalled = false;
    
        
    if (mObserver) {
        mObserver->InputDialogClosed(q, mReceivedButton);
            return; // observer callback deleted this object. Do not touch it anymore.
    }
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogPrivate::DeviceDialogClosed::End") ) );
}

// Constructor
CHbDeviceInputDialogSymbian::CHbDeviceInputDialogSymbian()
{
}

// Destructor
CHbDeviceInputDialogSymbian::~CHbDeviceInputDialogSymbian()
{
    delete d;
}

CHbDeviceInputDialogSymbian* CHbDeviceInputDialogSymbian::NewL(MHbDeviceInputDialogObserver *aObserver)
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::NewL::Begin") ) );
    ARG_UNUSED(aObserver)
    CHbDeviceInputDialogSymbian *inputDialog = new (ELeave) CHbDeviceInputDialogSymbian;
    CleanupStack::PushL(inputDialog);
    CHbDeviceInputDialogPrivate *idialogPrivate = new (ELeave) CHbDeviceInputDialogPrivate;
    idialogPrivate->q = inputDialog;
    CleanupStack::PushL(idialogPrivate);
    idialogPrivate->ConstructL( aObserver);
    inputDialog->d = idialogPrivate;
    CleanupStack::Pop(2); // inputDialog, idialogPrivate
    inputDialog->SetObserver(aObserver);
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::NewL::End") ) );
    return inputDialog;
}


void CHbDeviceInputDialogSymbian::ShowL()
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::ShowL::Begin") ) );
    d->SendToServerL(true);
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::ShowL::End") ) );
}


TPtrC CHbDeviceInputDialogSymbian::getTextL()
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::getTextL::Begin") ) );
    TRACES( RDebug::Print(_L( "CHbDeviceInputDialogSymbian::getTextL::Begin %S" ),&iPassword));
    HBufC* sample1 = HBufC::NewLC(iPassword.Length());
    TPtrC aStringPointer = sample1->Des();
    aStringPointer.Set(iPassword);
    CleanupStack::PopAndDestroy();       
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::getTextL::End") ) );
    return aStringPointer;
}

void CHbDeviceInputDialogSymbian::Close()
{
    TRACES( RDebug::Print( _L("CHbDeviceInputDialogSymbian::Close::Begin") ) );
    d->Close();
}

void CHbDeviceInputDialogSymbian::SetObserver(MHbDeviceInputDialogObserver *aObserver)
{
    d->mObserver = aObserver;
}
