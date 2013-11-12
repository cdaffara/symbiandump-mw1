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


#ifndef CONTEXTSOURCEDEF_V2_H
#define CONTEXTSOURCEDEF_V2_H

#include <cfdefinitions.h>

/** Source */
_LIT( KContextSource_V2_Source, "ContextSource_V2" );

/** Type */
_LIT( KContextSource_V2_Type,   "ContextSource_V2.Installed" );

/** Values */
_LIT_CONTEXT_VALUES( KContextSource_V2_Type_Values ) =
    {
    _S( "Yes" ),
    _S( "No" )
    };

/** Values enum */
enum TContextSource_V2_Type_Values
    {
    EContextSource_V2_ValueYes,
    EContextSource_V2_ValueNo,
    EContextSource_V2_ValueLast
    };

#endif /*CONTEXTSOURCEDEF_V2_H*/
