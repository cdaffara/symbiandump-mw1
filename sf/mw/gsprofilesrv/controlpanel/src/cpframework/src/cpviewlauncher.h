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
* Description:  This class adds a setting view to main window, and restore previous view when back key clicked.
*
*/

#ifndef CPVIEWLAUNCHER_H
#define CPVIEWLAUNCHER_H

#include <QObject>

class HbView;
class CpViewLauncher : public QObject
{
    Q_OBJECT
public:
	static void launchView(HbView *view);
private slots:
    void viewDone();
private:
    Q_DISABLE_COPY(CpViewLauncher)
    CpViewLauncher();
    ~CpViewLauncher();
    void internalLaunchView(HbView *view);
private:
    HbView *mView;
    HbView *mPreView;
};

#endif /* CPVIEWLAUNCHER_H */
