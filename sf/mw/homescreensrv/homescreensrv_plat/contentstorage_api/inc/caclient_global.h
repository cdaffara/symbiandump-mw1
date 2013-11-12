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
 * Description:
 *
 */

#ifndef CACLIENT_GLOBAL_H
#define CACLIENT_GLOBAL_H


#ifdef CACLIENT_LIB
#define CACLIENT_EXPORT Q_DECL_EXPORT
#else
#ifdef CACLIENT_TEST
#define CACLIENT_EXPORT
#else
#define CACLIENT_EXPORT Q_DECL_IMPORT
#endif
#endif

#ifndef CONTENTARSENAL_UNITTEST
#define CA_CLIENT_TEST_CLASS(aClassName)
#else
#define CA_CLIENT_TEST_CLASS(aClassName) class aClassName;
#endif

#ifndef CONTENTARSENAL_UNITTEST
#define CA_CLIENT_TEST_FRIEND_CLASS(aClassName)
#else
#define CA_CLIENT_TEST_FRIEND_CLASS(aClassName) friend class aClassName;
#endif

#endif // CACLIENT_GLOBAL_H
