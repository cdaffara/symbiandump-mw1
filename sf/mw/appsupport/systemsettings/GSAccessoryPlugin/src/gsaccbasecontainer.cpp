/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CGSAccBaseContainer class
*
*/


#include <aknlists.h>
#include <gsaccessoryplugin.rsg>
#include <gslistbox.h>

#include "gsaccbasecontainer.h"
#include "gsaccessorypluginmodel.h"
#include "trace.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccBaseContainer::CGSAccBaseContainer()
// ---------------------------------------------------------------------------
//
CGSAccBaseContainer::CGSAccBaseContainer( CGSAccessoryPluginModel& aModel )
  : iModel ( aModel )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::~CGSAccBaseContainer()
// ---------------------------------------------------------------------------
//
CGSAccBaseContainer::~CGSAccBaseContainer()
    {
    FUNC_LOG;

    delete iListboxItemArray;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::CurrentFeatureId()
// ---------------------------------------------------------------------------
//
TInt CGSAccBaseContainer::CurrentFeatureId() const
    {
    FUNC_LOG;

    return iListboxItemArray->CurrentFeature();
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::CreateListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseContainer::CreateListBoxL()
    {
    FUNC_LOG;

    iListBox = new( ELeave ) CAknSettingStyleListBox;
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::SetItemTextL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseContainer::SetItemTextL( TInt aItemId, const TDesC& aText )
    {
    FUNC_LOG;

    iListboxItemArray->SetDynamicTextL( aItemId, aText );
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::MakeItemVisibleL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseContainer::MakeItemVisibleL( TInt aItemId )
    {
    FUNC_LOG;
    INFO_1( "Making listbox item %d visible", aItemId );

    iListboxItemArray->SetItemVisibilityL(
        aItemId, CGSListBoxItemTextArray::EVisible );
    }


// ---------------------------------------------------------------------------
// CGSAccBaseContainer::ConstructListBoxL()
// ---------------------------------------------------------------------------
//
void CGSAccBaseContainer::ConstructListBoxL( TInt aResLbxId )
    {
    FUNC_LOG;

    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL(
        aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    }
