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
* Description:  Utility class for cpframework.
*
*/
#ifndef CP_UTILITY_H
#define CP_UTILITY_H

#include <QStringList>

class HbDataFormModelItem;
class CpItemDataHelper;

class CpUtility
{
public:
	/*
		get all physical drives of the devices
	*/
	static QStringList drives();

	/*
		get all controlpanel plugin directories of the device
	*/
	static QStringList pluginDirectories();

	/*
		get all config directories of the device
	*/
	static QStringList configFileDirectories();

};

#endif
