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
 * Description:  ?Description
 *
 */

#ifndef CAUNINSTALLEROBSERVER_H_
#define CAUNINSTALLEROBSERVER_H_

class CaUninstallerObserver
{
public:
    virtual void uninstallError(int error) = 0;
};

#endif /* CAUNINSTALLEROBSERVER_H_ */
