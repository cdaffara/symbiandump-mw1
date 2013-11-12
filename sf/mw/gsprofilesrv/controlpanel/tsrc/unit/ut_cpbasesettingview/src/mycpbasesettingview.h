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
* Description:  
*       test application for qt control panel public apis.
*/

#ifndef MYCPBASESETTINGVIEW_H_
#define MYCPBASESETTINGVIEW_H_

#include <cpbasesettingview.h>
#include <QObject>

class MyCpBaseSettingView : public CpBaseSettingView
    {
    Q_OBJECT
    
public:
    /*!
     * provide a public interface for unit test code to call close()
     */
    virtual void close();
    
    void testprivate();
    };

#endif /* MYCPBASESETTINGVIEW_H_ */
