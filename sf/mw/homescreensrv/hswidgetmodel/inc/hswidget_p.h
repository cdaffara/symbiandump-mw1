/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The private implementation of the HsWidget.
*
*/


#ifndef HSWIDGET_P_H
#define HSWIDGET_P_H

class HsWidget;

class HsWidgetPrivate
{

public:

    enum WidgetState{
        WidgetStateConstructed,
        WidgetStateRunning,
        WidgetStateSuspended,
        WidgetStateFinishing,
        WidgetStateFaulting,
        WidgetStateFinished
    };

    HsWidgetPrivate(HsWidget *widget);
    ~HsWidgetPrivate();


public:
    // automata control
    void start();
    void stop();
    void suspend();
    void resume();

    void setFinished();
    void setFaulted();
    void setFaulting();

    HsWidget* mPublic;
    WidgetState mState;

private:

    Q_DISABLE_COPY(HsWidgetPrivate)

};




#endif
