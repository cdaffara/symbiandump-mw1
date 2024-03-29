/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbWidgets module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#ifndef HBZOOMSLIDER_H
#define HBZOOMSLIDER_H

#include <hbslider.h>
#include <hbstyleoptionslider_p.h>
#include "hbnamespace_p.h"

class HbZoomSliderPrivate;

class HB_AUTOTEST_EXPORT HbZoomSlider : public HbSlider
{
    Q_OBJECT

public:
    explicit HbZoomSlider(QGraphicsItem *parent = 0);
    virtual ~HbZoomSlider();

    enum { Type = HbPrivate::ItemType_ZoomSlider };
    int type() const { return Type; }

    int defaultZoomLevel()const;
    void setDefaultZoomLevel(int value);

    virtual void setToolTipVisible(bool);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusInEvent(QFocusEvent *event);
    void initStyleOption(HbStyleOptionSlider *option) const;

private:
    Q_DECLARE_PRIVATE_D(d_ptr, HbZoomSlider)
    Q_DISABLE_COPY(HbZoomSlider)

    Q_PRIVATE_SLOT(d_func(), void _q_resetToDefault())
};

#endif // HBZOOMSLIDER_H
