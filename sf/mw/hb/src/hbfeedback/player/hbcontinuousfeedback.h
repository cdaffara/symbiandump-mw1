/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbFeedback module of the UI Extensions for Mobile.
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

#ifndef HBCONTINUOUSFEEDBACK_H
#define HBCONTINUOUSFEEDBACK_H

#include "hbabstractfeedback.h"

class HbContinuousFeedbackPrivate;

class HB_FEEDBACK_EXPORT HbContinuousFeedback : public HbAbstractFeedback
{

public:
    HbContinuousFeedback();
    HbContinuousFeedback(HbFeedback::ContinuousEffect effect, const QWidget* widget);
    ~HbContinuousFeedback();

    HbFeedback::Type type() const {
        return HbFeedback::TypeContinuous;
    }

    bool isValid() const;

    HbFeedback::ContinuousEffect continuousEffect() const;
    int intensity() const;
    int timeout() const;

    void setContinuousEffect(HbFeedback::ContinuousEffect effect);
    void setIntensity(int intensity);
    void setTimeout(int msecTimeout);

    void play();
    void stop();
    bool isPlaying();

    HbContinuousFeedback &operator=(const HbContinuousFeedback &feedback);
    bool operator==(const HbContinuousFeedback &feedback) const;
    bool operator!=(const HbContinuousFeedback &feedback) const;

private:
    HbContinuousFeedbackPrivate *d;
};

#endif // HBCONTINUOUSFEEDBACK_H
