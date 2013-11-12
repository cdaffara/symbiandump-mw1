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

#ifndef HBDEVICEPOWERMENUPLUGINERRORS_P_H
#define HBDEVICEPOWERMENUPLUGINERRORS_P_H

#include <hbdevicedialog.h>
#include <e32debug.h>
#ifdef _DEBUG

    #define TRACES( x ) x

#else

    #define TRACES( x )

#endif


// No error
const int NoError = 0;
// Illegal parameter error
const int ParameterError = HbDeviceDialog::PluginErrors + 1;
// Unknown device dialog error
const int UnknownDeviceDialogError = HbDeviceDialog::PluginErrors + 2;

#endif // HBDEVICEPOWERMENUPLUGINERRORS_P_H
