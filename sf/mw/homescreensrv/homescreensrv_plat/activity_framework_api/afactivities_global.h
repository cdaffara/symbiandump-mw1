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

#ifndef AFACTIVITIES_GLOBAL_H
#define AFACTIVITIES_GLOBAL_H

#include <qglobal.h>
#include <qmetatype.h>

#ifdef AFACTIVITIES_LIB
    #define AFACTIVITIES_EXPORT Q_DECL_EXPORT
#else
    #ifdef AFACTIVITIES_TEST
        #define AFACTIVITIES_EXPORT
    #else
        #define AFACTIVITIES_EXPORT Q_DECL_IMPORT
    #endif
#endif

namespace Af {
    enum ActivationReason {
        ActivationReasonActivity = 0,
        ActivationReasonService,
        ActivationReasonNormal
    };    
    
    const char KActivityScheme[] = "appto";

    const char KActivityUriNameKey[] = "activityname";
    const char KActivityUriBackgroundKey[] = "activityinbackground";
}

Q_DECLARE_METATYPE( Af::ActivationReason )

#endif // AFACTIVITIES_GLOBAL_H
