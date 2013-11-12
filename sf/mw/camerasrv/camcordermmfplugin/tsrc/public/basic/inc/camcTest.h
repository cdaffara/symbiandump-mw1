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


/* Choose proper header file, depending on which test framework 
   the tests are built for */

#ifndef CAMCTEST_H
#define CAMCTEST_H

#ifdef USING_CPPUNIT_FRAMEWORK

 /* CppUnit headers */
#include <CppUnit/Test.h>
#include <CppUnit/TestCase.h>
#include <CppUnit/TestCaller.h>
#include <CppUnit/TestSuite.h>

#else

  /* STIF TFW headers */
#include "TestFramework/test.h"
#include "TestFramework/TestCase.h"
#include "TestFramework/TestCaller.h"
#include "TestFramework/TestSuite.h"

#endif


//////////////////
// Utility function
//////////////////

void AddDriveLetterToPath(const TDesC &aFileName,TDes &aFileNameWithPath ) ;

//////////////////
// Defines
//////////////////

#if ( defined ( __MPEG4_AAC_ENCODING) ) && ( defined (__MPEG4_VIDEO_ENCODING) )
#define MP4_FILE_FORMAT_SUPPORTED
#else
#undef MP4_FILE_FORMAT_SUPPORTED
#endif


#endif
