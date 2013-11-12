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
***************************************************************************/

#include <hbaction.h>
#include <hbdialog.h>
#include <hbinputdialog.h>
#include<hblineedit.h>
#include <hbiconanimationmanager.h>
#include "hbdeviceinputdialogwidget_p.h"
#include "hbdeviceinputdialogpluginerrors_p.h"

//For Localization
QString LOCALE_PATH("/resource/qt/translations/"); 
QString TS_FILE("filemanager");

// Constructor
HbDeviceInputDialogWidget::HbDeviceInputDialogWidget(const QVariantMap &parameters):HbInputDialog()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::Constructor:Begin") ) );
    mLastError = NoError;
    mShowEventReceived = false;
    mPrimaryAction = 0;
    mSecondaryAction = 0;
    mDeviceDialogError =0;
    mHack = 1;
    //resetProperties();
    if (preConstructDialog())
       {
        //Translator loaded succesfully then olny construct widgets
        constructDialog(parameters);
        this->actions()[0]->setEnabled(true);

        if (!mPrimaryAction) {
            // If default button provided by HbInputDialog is used, connect into its triggered signal.
            QAction *action = /*primaryAction()*/this->actions().first();
            if (action) {
                connect(action, SIGNAL(triggered()), SLOT(primaryActionTriggered()));
                }
        }
        if (!mSecondaryAction) {
            // If default button provided by HbInputDialog is used, connect into its triggered signal.
            QAction *action = /*secondaryAction()*/ this->actions().at(1);
            if (action) {
                connect(action, SIGNAL(triggered()), SLOT(secondaryActionTriggered()));
            }
        }  
        connect(this, SIGNAL(visibilityChanged()), SLOT(closeDialog()));
       }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::Constructor:End") ) );
}

// Destructor
HbDeviceInputDialogWidget::~HbDeviceInputDialogWidget()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::Destrcuctor:Begin") ) );
    delete mPrimaryAction;
    delete mSecondaryAction;
    delete mHbTranslator;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::Destructor:End") ) );
}

// Set parameters
bool HbDeviceInputDialogWidget::setDeviceDialogParameters(const QVariantMap &parameters)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setDeviceDialogParameters:Begin") ) );
    mLastError = NoError;
    bool ret(false);
    if (checkProperties(parameters)) {
        setProperties(parameters);
        ret = true;
    }
    else {
        mLastError = ParameterError;
        ret = false;
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setDeviceDialogParameters:End") ) );
    return ret;
}

/**
 * Before creating widgets Translator must be loaded
 */
bool HbDeviceInputDialogWidget::preConstructDialog()
    {
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::preConstructDialog:Begin") ) );
    //Translator
    mHbTranslator = new HbTranslator(LOCALE_PATH,TS_FILE);
    if(!mHbTranslator)
        {
        //Translator is failed to create
        mDeviceDialogError = UnknownDeviceDialogError;
        TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::preConstructDialog:MemoryAllocation Failed") ) );
        return false;
        }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::preConstructDialog:End") ) );
    return true;
    }

// Get error
int HbDeviceInputDialogWidget::deviceDialogError() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::deviceDialogError:Begin") ) );
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::deviceDialogError:End") ) );
    return mLastError;
}

// Close device dialog
void HbDeviceInputDialogWidget::closeDeviceDialog(bool byClient)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::closeDeviceDialog:Begin") ) );
    Q_UNUSED(byClient);
    // Closed by client or internally by server -> no action to be transmitted.
    mSendAction = false;
    close();
    // If show event has been received, close is signalled from hide event. If not,
    // hide event does not come and close is signalled from here.
    if (!mShowEventReceived) {
        emit deviceDialogClosed();
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::closeDeviceDialog:End") ) );
}

// Return display widget
HbDialog *HbDeviceInputDialogWidget::deviceDialogWidget() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::deviceDialogWidget:Begin") ) );
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::deviceDialogWidget:End") ) );
    return const_cast<HbDeviceInputDialogWidget*>(this);
}


// Construct dialog
bool HbDeviceInputDialogWidget::constructDialog(const QVariantMap &parameters)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::constructDialog:Begin") ) );
    bool ret(false);
    // Check that parameters are valid
    
    if (!checkProperties(parameters)) {
        mLastError = ParameterError;
        ret = false;
    }
    else {
        setProperties(parameters);
        resetProperties();
        ret = true;
    }
   
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::constructDialog:End") ) );
    return ret;
}

// Check that device dialog parameters are valid
bool HbDeviceInputDialogWidget::checkProperties(const QVariantMap &parameters)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::checkProperties:Begin") ) );
    QVariantMap::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd()) {
        QByteArray key = i.key().toAscii();
        if (!property(key.constData()).isValid()) {
        TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::checkProperties:End :: returning false") ) );
            return false;
        }
        ++i;
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::checkProperties:End :: returning true") ) );
    return true;
}

// Set properties
void HbDeviceInputDialogWidget::setProperties(const QVariantMap &parameters)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setProperties:Begin") ) );
    QVariantMap::const_iterator i = parameters.constBegin();
    while (i != parameters.constEnd()) {
        QByteArray key = i.key().toAscii();
        if (property(key.constData()).isValid()) {
            setProperty(key.constData(), i.value());
        }
        ++i;
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setProperties:End") ) );
    return;
}

// Reset properties to default values
void HbDeviceInputDialogWidget::resetProperties()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::resetProperties:Begin") ) );
//    mIconName.clear();
    setModal(true);
    setPromptText(hbTrId("txt_common_dialog_password"), 0); 
    setInputMode(TextInput,0);
    setEchoMode(HbLineEdit::Password,0);
    setTimeout(HbPopup::NoTimeout);
    setDismissPolicy(HbPopup::NoDismiss);
    
    mPrimaryActionNull = false;
    mSecondaryActionNull = false;
    mSendAction = true;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::resetProperties:End") ) );
    return;
}

QString HbDeviceInputDialogWidget::primaryActionText() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::primaryActionText:Begin") ) );
    QAction *action = /*primaryAction()*/this->actions().first();
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::primaryActionText:End") ) );
    return action ? action->text() : QString();
}

void HbDeviceInputDialogWidget::setPrimaryActionText(QString &actionText)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setPrimaryActionText:Begin") ) );
    QAction *action = /*primaryAction()*/this->actions().first();
    if (action) {
        action->setText(actionText);
    } else {
        if (!mPrimaryAction) {
            mPrimaryAction = new HbAction(actionText);
            connect(mPrimaryAction, SIGNAL(triggered()), this, SLOT(primaryActionTriggered()));
        } else {
            mPrimaryAction->setText(actionText);
        }
        setPrimaryAction(mPrimaryAction);
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setPrimaryActionText:End") ) );
}

QString HbDeviceInputDialogWidget::secondaryActionText() const
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::secondaryActionText:Begin") ) );
    QAction *action = /*secondaryAction()*/ this->actions().at(1);
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::secondaryActionText:End") ) );
    return action ? action->text() : QString();
    
}

void HbDeviceInputDialogWidget::setSecondaryActionText(QString &actionText)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setSecondaryActionText:Begin") ) );
    QAction *action = /*secondaryAction()*/ this->actions().at(1);
    if (action) {
        action->setText(actionText);
    } else {
        if (!mSecondaryAction) {
            mSecondaryAction = new HbAction(actionText);
            connect(mSecondaryAction, SIGNAL(triggered()), this, SLOT(secondaryActionTriggered()));
        } else {
            mSecondaryAction->setText(actionText);
        }
        setSecondaryAction(mSecondaryAction);
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setSecondaryActionText:End") ) );
}

bool HbDeviceInputDialogWidget::primaryActionNull() const
{
    return this->actions().first() == 0;
}

void HbDeviceInputDialogWidget::setPrimaryActionNull(bool isNull)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setPrimaryActionNull:Begin") ) );
    if (isNull) {
        // If there is a message box's default action, disconnect from it.
        QAction *action = /*primaryAction()*/this->actions().first();
        if (action && mPrimaryAction == 0) {
            action->disconnect(SIGNAL(triggered()), this, SLOT(primaryActionTriggered()));
        }
        setPrimaryAction(0);
    } else {
        QString text = mPrimaryAction ? mPrimaryAction->text() : QString();
        setPrimaryActionText(text);
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setPrimaryActionNull:End") ) );
}

bool HbDeviceInputDialogWidget::secondaryActionNull() const
{
    return this->actions().at(1) == 0;
}

void HbDeviceInputDialogWidget::setSecondaryActionNull(bool isNull)
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setSecondaryActionNull:Begin") ) );
    if (isNull) {
        // If there is a message box's default action, disconnect from it.
        QAction *action = /*secondaryAction()*/ this->actions().at(1);
        if (action && mSecondaryAction == 0) {
            action->disconnect(SIGNAL(triggered()), this, SLOT(secondaryActionTriggered()));
        }
        setSecondaryAction(0);
    } else {
        QString text = mSecondaryAction ? mSecondaryAction->text() : QString();
        setSecondaryActionText(text);
    }
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::setSecondaryActionNull:End") ) );
}

// Widget is about to hide. Closing effect has ended.
void HbDeviceInputDialogWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::hideEvent:Begin") ) );
    HbInputDialog::hideEvent(event);
    emit deviceDialogClosed();
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::hideEvent:End") ) );
}

// Widget is about to show
void HbDeviceInputDialogWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::showEvent:Begin") ) );
    HbInputDialog::showEvent(event);
    mShowEventReceived = true;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::showEvent:End") ) );
}

void HbDeviceInputDialogWidget::setAnimationDefinition(QString &animationDefinition)
{
    mAnimationDefinition = animationDefinition;
}

QString HbDeviceInputDialogWidget::animationDefinition() const
{
    return mAnimationDefinition;
}

// Primary action triggered
void HbDeviceInputDialogWidget::primaryActionTriggered()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::primaryActionTriggered:Begin") ) );
    //Get the string enter by user
    QString myString = this->value().toString();
    data.insert("ok",myString);
    emit deviceDialogData(data);
    mSendAction = true;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::primaryActionTriggered:End") ) );
}

// Secondary action triggered
void HbDeviceInputDialogWidget::secondaryActionTriggered()
{
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::secondaryActionTriggered:Begin") ) );
    data.insert("cancel","cancel");
    emit deviceDialogData(data);
    mSendAction = false;
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::secondaryActionTriggered:End") ) );
}


// Close Dialog when visibility changed
void HbDeviceInputDialogWidget::closeDialog()
    {
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::closeDialog:Begin") ) );
    if(mHack == 1)
        {
        emit deviceDialogData(data); // first time plugin didnt called deviceDialogData and hence this hack added.
        }
    emit deviceDialogClosed();
    TRACES( RDebug::Print( _L("HbDeviceInputDialogWidget::closeDialog:End") ) );
    mHack = 0;
    }
