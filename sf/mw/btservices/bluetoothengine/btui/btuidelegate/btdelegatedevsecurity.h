/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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

#ifndef BTDELEGATEDEVSECURITY_H
#define BTDELEGATEDEVSECURITY_H

#include <e32base.h>
#include <btengdevman.h>
#include "btabstractdelegate.h"

class BtuiModel;

/*!
    \class BtDelegateDevSecurity
    \brief the base class for Unpairing Bluetooth Connection.
 */
class BtDelegateDevSecurity : public BtAbstractDelegate,
        public MBTEngDevManObserver
{
    Q_OBJECT

public:
    explicit BtDelegateDevSecurity( 
            BtSettingModel* settingModel, 
            BtDeviceModel* deviceModel, 
            QObject *parent = 0 );
    
    virtual ~BtDelegateDevSecurity();

    int supportedEditorTypes() const;
    
    virtual void exec( const QVariant &params );
    
    virtual void cancel();
    
public slots:
    void disconnectDelegateCompleted(int err);

protected:
    //From MBTEngDevManObserver
    virtual void HandleDevManComplete( TInt aErr );
    virtual void HandleGetDevicesComplete( TInt aErr, CBTDeviceArray* aDeviceArray );

    void emitCommandComplete(int error);
 
private:
    void unpair();
    void authorizeOrBlock();
    
private:

    CBTEngDevMan *mBtEngDevMan;
    BtAbstractDelegate* mDisconnectDelegate;
    TBTDevAddr mBtEngAddr;  // Symbian BT address for target device
    QString mStrBtAddr;     // QT BT address for target device
    TBTRegistrySearch mSearchPattern; 
    CBTDeviceArray *mRegDevArray;
    int mOperation;
    CBTDevice *mDevice;
    CBTDevice *mNewDev;
    int mActiveHandling;
    QModelIndex mIndex;
    bool mAddingBlockedDev;
    Q_DISABLE_COPY(BtDelegateDevSecurity)

};


#endif /* BTDELEGATEDEVSECURITY_H */
