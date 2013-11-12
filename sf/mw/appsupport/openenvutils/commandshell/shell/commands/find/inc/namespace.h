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
#ifndef __NAMESPACE_H__
#define __NAMESPACE_H__
/* ______________________________________________________________________
   namespace.h
   $Id: namespace.h,v 1.2 2003/07/02 21:59:38 jriehl Exp $
   ______________________________________________________________________ */

void pushNamespace (void);
void popNamespace (void);
void addName (char * name, void * data);
int inNamespace (char * name);

/* ______________________________________________________________________
   End of namespace.h
   ______________________________________________________________________ */
#endif
