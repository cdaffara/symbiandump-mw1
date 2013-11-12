/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camcorder MMF API Test DLL
*
*/


#include <e32base.h>
#include <f32file.h>
#include "camcTest.h"
//////////////////
// Utility function
//////////////////


void AddDriveLetterToPath(const TDesC &aFileName,TDes &aFileNameWithPath ) 
    {
    TFileName aDrive;

    Dll::FileName(aDrive);
    TParsePtr parse(aDrive);
    aDrive = parse.Drive();
    if ((aDrive[0] == 'Z') || (aDrive[0] == 'z'))
        {
        aDrive = _L("E:");
        }
    aDrive.Append(_L("\\"));
    aDrive.Append(aFileName);
    aFileNameWithPath = aDrive;
    RDebug::Print(_L("Recording Full Path:")); 
    RDebug::Print(aDrive);
    }

