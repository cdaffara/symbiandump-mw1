/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MSysApLocationPrivacyIndicator class definition. A virtual class.
*
*/


#ifndef MSYSAPLOCATIONPRIVACY_H
#define MSYSAPLOCATIONPRIVACY_H

// CLASS DECLARATION
/**
*  MSysApLocationPrivacyIndicator
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApAppUi;

class MSysApLocationPrivacyIndicator
{
public:

    /**
    * Destructor.
    */
    virtual ~MSysApLocationPrivacyIndicator() {};

    /**
    * Initializes Location Privacy indicator.
    * @param None
    * @return void
    */	
    virtual void InitL() = 0;

    TInt iIndicatorIdAcceptAll;
    TInt iIndicatorIdRejectAll;
    TInt iIndicatorIdAlwaysAsk;
    TInt iIndicatorIdIndividualPrivacy;
};

MSysApLocationPrivacyIndicator* CreateSysApLocationPrivacyIndicatorL( CSysApAppUi& aSysApAppUi );

#endif // MSYSAPLOCATIONPRIVACY_H

// End of File
