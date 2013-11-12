/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  test eikspmod.h
 *
*/


#ifndef C_TESTSDKSTATUSPANEVISITOR_H
#define C_TESTSDKSTATUSPANEVISITOR_H

#include <eikspmod.h>

class TTestSDKStatusPaneVisitor : public MEikStatusPaneLayoutTreeVisitor
    {
public:
    
    /**
    * from MEikStatusPaneLayoutTreeVisitor
    */
    void VisitL( CEikStatusPaneLayoutTree* aNode );

    };


#endif /*C_TESTSDKSTATUSPANEVISITOR_H*/

