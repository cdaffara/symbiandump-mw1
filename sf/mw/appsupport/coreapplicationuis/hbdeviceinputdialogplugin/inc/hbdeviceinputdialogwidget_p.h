/****************************************************************************
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
****************************************************************************/

#ifndef HBDEVICEINPUTDIALOGWIDGET_P_H
#define HBDEVICEINPUTDIALOGWIDGET_P_H

#include <QObject>
#include <QVariantMap>

#include <hbdevicedialoginterface.h>
#include <hbdevicedialog.h>
#include <hbinputdialog.h>
#include <hbTranslator.h>

class HbDialog;

class HbDeviceInputDialogWidget : public HbInputDialog, public HbDeviceDialogInterface
{
    Q_OBJECT
    Q_PROPERTY(QString primaryActionText READ primaryActionText WRITE setPrimaryActionText)
    Q_PROPERTY(QString secondaryActionText READ secondaryActionText WRITE setSecondaryActionText)
    Q_PROPERTY(bool primaryActionNull READ primaryActionNull WRITE setPrimaryActionNull)
    Q_PROPERTY(bool secondaryActionNull READ secondaryActionNull WRITE setSecondaryActionNull)
    Q_PROPERTY(QString animationDefinition READ animationDefinition WRITE setAnimationDefinition)

public:
    HbDeviceInputDialogWidget(const QVariantMap &parameters);
    ~HbDeviceInputDialogWidget();

    bool setDeviceDialogParameters(const QVariantMap &parameters);
    int deviceDialogError() const;
    void closeDeviceDialog(bool byClient);
    HbDialog *deviceDialogWidget() const;

public slots:
    void primaryActionTriggered();
    void secondaryActionTriggered();
    void closeDialog();

private:
    bool preConstructDialog();
    bool constructDialog(const QVariantMap &parameters);
    bool checkProperties(const QVariantMap &parameters);
    void setProperties(const QVariantMap &parameters);
    void resetProperties();
    QString primaryActionText() const;
    void setPrimaryActionText(QString &actionText);
    QString secondaryActionText() const;
    void setSecondaryActionText(QString &actionText);
    bool primaryActionNull() const;
    void setPrimaryActionNull(bool null);
    bool secondaryActionNull() const;
    void setSecondaryActionNull(bool null);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    void setAnimationDefinition(QString &animationDefinition);
    QString animationDefinition() const;

signals:
    void deviceDialogClosed();
    void deviceDialogData(QVariantMap data);  //used to send the inputted text from server to client

private:
    Q_DISABLE_COPY(HbDeviceInputDialogWidget)

    int             mLastError;
    int             mDeviceDialogError;             //DeviceDialogError
    int 						mHack;

    QString         mPrimaryActionText;
    QString         mSecondaryActionText;
    bool            mPrimaryActionNull;
    bool            mSecondaryActionNull;
    int             mSendAction;
    bool            mShowEventReceived;
    QString         mAnimationDefinition;
    QVariantMap     data;
    HbAction*       mPrimaryAction;
    HbAction*       mSecondaryAction;
    HbTranslator*   mHbTranslator;     
};

#endif // HBDEVICEINPUTDIALOGWIDGET_P_H
