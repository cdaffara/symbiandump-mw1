/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  View item implementation for list entry item.
*
*/

#include "cpdataformlistentryviewitem.h"

#ifdef HB_EFFECTS
    #include "hbeffect.h"
#endif

#ifdef HB_GESTURE_FW
    #include <hbtapgesture.h>
    #include <hbframeitem.h>
#endif

#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbabstractitemview.h>
#include <cpsettingformentryitemdata.h>

#define CP_DATAFORMLISTENTRYVIEWITEM QLatin1String("cpdataformlistentryviewitem")

/**
 * Constructor
 */
CpDataFormListEntryViewItem::CpDataFormListEntryViewItem(QGraphicsItem *parent) : 
    HbDataFormViewItem(parent)
{
    // Add effeect just like item in HbListView
#ifdef HB_EFFECTS       
    HbEffect::add(CP_DATAFORMLISTENTRYVIEWITEM + "-focus", "listviewitem_press", "pressed");
    HbEffect::add(CP_DATAFORMLISTENTRYVIEWITEM + "-focus", "listviewitem_release", "released");
#endif
}

/**
 * Destructor
 */
CpDataFormListEntryViewItem::~CpDataFormListEntryViewItem()
{
}

/**
 * Reimplement by CpDataFormListEntryViewItem, this function return a instance copy.
 */
HbAbstractViewItem* CpDataFormListEntryViewItem::createItem()
{
    return new CpDataFormListEntryViewItem(*this);
}

/**
 * Reimplement by CpDataFormListEntryViewItem, return true when the model item type is 
 * ListEntryItem
 */
bool CpDataFormListEntryViewItem::canSetModelIndex(const QModelIndex &index) const
{
    HbDataFormModelItem::DataItemType itemType = 
        static_cast<HbDataFormModelItem::DataItemType>( 
                index.data(HbDataFormModelItem::ItemTypeRole).toInt());

    return ( itemType == CpSettingFormEntryItemData::ListEntryItem );
}

/**
 * Reimplement by CpDataFormListEntryViewItem
 * No content widget in custom list item. So return a null pointer directly
 */
HbWidget* CpDataFormListEntryViewItem::createCustomWidget()
{
    // Don't need to create any widget
    return 0;
}
/**
 * Reimplement by CpDataFormListEntryViewItem. Change the description property for 
 * custom list item.
 */
void CpDataFormListEntryViewItem::restore()
{
    HbDataFormModelItem::DataItemType itemType =
        static_cast<HbDataFormModelItem::DataItemType> (modelIndex().data(
            HbDataFormModelItem::ItemTypeRole).toInt());

    if (itemType == CpSettingFormEntryItemData::ListEntryItem) {
        QModelIndex itemIndex = modelIndex();
        HbDataFormModel *model = qobject_cast<HbDataFormModel*> (itemView()->model());
        if (model) {
            HbDataFormModelItem *modelItem = model->itemFromIndex(itemIndex);
            if (modelItem->data(HbDataFormModelItem::DescriptionRole).toString().isEmpty()) {
                this->setProperty("hasDescription", false);
            }
            else {
                this->setProperty("hasDescription", true);
            }    
        }        
    }
    HbDataFormViewItem::restore();
}

/**
 * Reimplement by CpDataFormListEntryViewItem, provide the effect when pressing list item
 */
void CpDataFormListEntryViewItem::pressStateChanged(bool pressed, bool animate)
{
#ifdef HB_EFFECTS
    QGraphicsItem *focusItem =  primitive("dataItem_Background");
    bool doAnimate = animate;

    if (pressed) {
        if (doAnimate) {
            HbEffect::cancel(this, "released");
            HbEffect::cancel(focusItem, "released");

            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "pressed");
            HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "pressed");
        }
    } else {
        if (doAnimate) {
            HbEffect::cancel(this, "pressed");
            HbEffect::cancel(focusItem, "pressed");

            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "released");
            HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "released");
        } else {
            HbEffect::cancel(this, "pressed");
            HbEffect::start(this, CP_DATAFORMLISTENTRYVIEWITEM, "released");
            if (focusItem) {
                HbEffect::cancel(focusItem, "pressed");
                HbEffect::start(focusItem, CP_DATAFORMLISTENTRYVIEWITEM + QString("-focus"), "released", this);
            }
        }
    }
#endif
}

#ifdef HB_GESTURE_FW

/**
 * Reimplement by CpDataFormListEntryViewItem, set different graphics for list item's gesture
 */
void CpDataFormListEntryViewItem::gestureEvent(QGestureEvent *event)
{
    HbFrameItem *backgroundItem = static_cast<HbFrameItem*> ( primitive("dataItem_Background") );
    HbTapGesture *tap = qobject_cast<HbTapGesture *>(event->gesture(Qt::TapGesture));
    
    if(backgroundItem && tap) {
        switch(tap->state()) {
        case Qt::GestureStarted: {
                // Set the pressed state
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_pressed"));
            }
            break;
        case Qt::GestureUpdated: {
            
            }
            break;
        case Qt::GestureCanceled: {
                // Set the normal state
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_normal"));
            }
            break;
        case Qt::GestureFinished: {
                // Set the normal state
                backgroundItem->frameDrawer().setFrameGraphicsName(QLatin1String("qtg_fr_list_normal"));
            }
            break;
        default:
            break;
        }
    }

    HbDataFormViewItem::gestureEvent( event );
}
#endif


//End of File
