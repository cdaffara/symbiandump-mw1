/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/
#ifndef TSTASKCONTENT_H
#define TSTASKCONTENT_H

class TsTaskContent
{
    
public:
    bool mClosable;
    bool mActive;
    QPixmap mScreenshot;
    QString mName;
    QByteArray mKey;
    
};

#endif //TSTASKCONTENT_H
