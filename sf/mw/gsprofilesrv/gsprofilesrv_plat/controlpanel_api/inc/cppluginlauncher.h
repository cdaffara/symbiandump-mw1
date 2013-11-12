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
* Description: Launch controlpanel plugin setting view in client process.
*
*/

#ifndef CPPLUGINLAUNCHER_H
#define CPPLUGINLAUNCHER_H

#include "cpglobal.h"
#include <QVariant>

class QString;
class CpBaseSettingView;

class CP_EXPORT CpPluginLauncher
{
public:    
	static CpBaseSettingView* launchSettingView(const QString &pluginFile,const QVariant &hint = QVariant());
};

#endif /* CPPLUGINLAUNCHER_H */
