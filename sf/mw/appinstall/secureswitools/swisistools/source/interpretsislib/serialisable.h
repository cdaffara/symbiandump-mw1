/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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


#ifndef	SERIALISABLE_H
#define	SERIALISABLE_H

#include <iostream>

class Serialiser;
class Deserialiser;
/** 
* @file SERIALISABLE.H
*
* @internalComponent
* @released
*/
class Serialisable
	{
public:
	virtual void Externalize(Serialiser& os) = 0;
	virtual void Internalize(Deserialiser& os) = 0;

private:

	};
#endif	/* SERIALISABLE_H */
