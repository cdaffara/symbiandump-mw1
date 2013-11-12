/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Context Framework common definitions
*
*/


#ifndef CFDEFINITIONS_H
#define CFDEFINITIONS_H

/**
* Defines context values array.
* Usage:
* @code
*  _LIT_CONTEXT_VALUES( KContextValues ) =
*  {
*  _S( "Value1" ),
*  _S( "Value2" ),
*  _S( "Value3" )
*  };
* @endcode
*/
#define _LIT_CONTEXT_VALUES( array ) static const TText* const array[]

#endif
