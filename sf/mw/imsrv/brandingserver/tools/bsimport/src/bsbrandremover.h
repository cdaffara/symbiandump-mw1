/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Helper class for removing brands
*
*/


#ifndef BSBRANDREMOVER_H
#define BSBRANDREMOVER_H


/**
 *  Brand remover helper
 *
 *  @lib bsinstall.exe
 *  @since S60 v3.2
 */
class BSBrandRemover 
    {
public:

    /**
     * Removes brand from branding server
     * @aBrandId brand id of removed brand
     * @aApplicationId application id of removed brand
     */
    static void RemoveBrandL( const TDesC& aBrandId,
                              const TDesC& aApplicationId );

    };

#endif //CBSBRANDREMOVER_H
