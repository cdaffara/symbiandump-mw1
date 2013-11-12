/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CA_STORAGE_GLOBAL_H
#define CA_STORAGE_GLOBAL_H

#ifndef CONTENT_ARSENAL_STORAGE_UT
#define CA_STORAGE_TEST_CLASS(aClassName)
#else
#define CA_STORAGE_TEST_CLASS(aClassName) class aClassName;
#endif

#ifndef CONTENT_ARSENAL_STORAGE_UT
#define CA_STORAGE_TEST_FRIEND_CLASS(aClassName)
#else
#define CA_STORAGE_TEST_FRIEND_CLASS(aClassName) friend class aClassName;
#endif

#endif // CA_STORAGE_GLOBAL_H
