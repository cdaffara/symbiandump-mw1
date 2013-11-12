/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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




#ifndef DUMMYPLUGIN_PROPETIES_H_
#define DUMMYPLUGIN_PROPETIES_H_

const TUid KUidOomPropertyCategory = {0x10286A3E};

//The key is the UID of the implementation instance, i.e. 10286A34 To 10286A3D
const TInt KUidOOMDummyPluginFirstImplementation(0x10286A34);
const TInt KUidOOMDummyPluginLastImplementation(0x10286A3D);

const TUint KOOMDummyPluginImplementationCount = 10;

const TUint KOOMDummyPluginLowMemoryCount = 0;
const TUint KOOMDummyPluginGoodMemoryCount = 1 * KOOMDummyPluginImplementationCount;
const TUint KOOMDummyPluginCurrentAllocationBytes = 2 * KOOMDummyPluginImplementationCount;
const TUint KOOMDummyPluginBytesRequested = 3 * KOOMDummyPluginImplementationCount;

#endif /* DUMMYPLUGIN_PROPETIES_H_ */
