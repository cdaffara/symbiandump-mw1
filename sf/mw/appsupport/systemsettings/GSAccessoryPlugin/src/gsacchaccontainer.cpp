/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CGSAccHACContainer class
*
*/


#include <e32cmn.h> // For gsfwviewuids.h
#include <gsaccessoryplugin.rsg>
#include <gsfwviewuids.h> // for KUidGS
#include <gslistbox.h>
#include <csxhelp/cp.hlp.hrh>

#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginmodel.h"
#include "gsacchaccontainer.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccHACContainer::CGSAccHACContainer()
// ---------------------------------------------------------------------------
//
CGSAccHACContainer::CGSAccHACContainer(
    CGSAccessoryPluginModel& aModel )
  : CGSAccBaseContainer( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::~CGSAccHACContainer()
// ---------------------------------------------------------------------------
//
CGSAccHACContainer::~CGSAccHACContainer()
    {
    FUNC_LOG;

    delete iHacItems;
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::UpdateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccHACContainer::UpdateListBoxL( TInt aFeatureId )
    {
    FUNC_LOG;

    if ( aFeatureId == KGSSettIdHACMode )
        {
        RefreshHacModeL();
        }

    iListBox->HandleItemAdditionL();
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccHACContainer::ConstructL( const TRect& aRect )
    {
    FUNC_LOG;

    CreateListBoxL();
    BaseConstructL( aRect, R_ACC_HAC_TITLE, R_ACC_HAC_LBX ); // Needs the listbox
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccHACContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    CGSAccBaseContainer::ConstructListBoxL( aResLbxId );

    iHacItems =
        iCoeEnv->ReadDesC16ArrayResourceL( R_ACC_HAC_MODE_SETTING_PAGE_LBX );

    RefreshHacModeL();
    MakeItemVisibleL( KGSSettIdHACMode );
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::GetHelpContext() const
// ---------------------------------------------------------------------------
//
void CGSAccHACContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    FUNC_LOG;

    aContext.iMajor = KUidGS;
    aContext.iContext = KSET_HLP_ACCESSORIES_LIST;
    }


// ---------------------------------------------------------------------------
// CGSAccHACContainer::RefreshHacModeL()
// ---------------------------------------------------------------------------
//
void CGSAccHACContainer::RefreshHacModeL()
    {
    FUNC_LOG;

    TInt mode = iModel.HacMode();
    if ( iHacItems && mode >= 0 && mode < iHacItems->Count() )
        {
        SetItemTextL( KGSSettIdHACMode, ( *iHacItems )[ mode ] );
        }
    }

