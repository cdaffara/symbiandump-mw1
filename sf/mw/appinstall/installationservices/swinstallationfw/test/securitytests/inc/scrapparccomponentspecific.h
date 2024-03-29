/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @test
*/

#ifndef SCRAPPARCCOMPONENTSPECIFIC_H_
#define SCRAPPARCCOMPONENTSPECIFIC_H_


#include <captestframework/aprcaptestutility.h>

class CScrApparcComponentSpecificSecTest : public CDefaultCapabilityTest
    {
public:
    static CScrApparcComponentSpecificSecTest* NewL();
    

private:
    CScrApparcComponentSpecificSecTest();
    
    void RunTestL();

    void ConstructL();
    };


#endif /* SCRAPPARCCOMPONENTSPECIFIC_H_ */
