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
* Declaration of RfsCustCmdFactory class.
*
*/

#ifndef RFSCUSTCMDFACTORY_H
#define RFSCUSTCMDFACTORY_H

#include <e32def.h>

class MSsmCustomCommand;

/**
 *  Creates custom command objects.
 *
 *  Custom commands are loaded by system state manager based on DLL name and
 *  function ordinal.
 *  Each DLL containing custom commands must offer factory methods for each
 *  custom command.
 *  The function ordinals must be exactly the same in emulator and HW builds
 *  or loading the custom command may have unpredictable results.
 *
 *  RfsCustCmdFactory is a static class containing factory method of one
 *  custom command. It can be easily extended to contain factory methods of
 *  multiple custom commands by adding more member functions.
 *
 *  It is better to implement own factory method for each separate command
 *  than to try to use command paramters to distinguis between them.
 *  Note that similar commands can be implemented in the same command class -
 *  just the factory methods need to be different.
 */
class RfsCustCmdFactory
    {

public:

    /**
     * Creates and returns a custom command of type CRfsCustCmd for first boot rfs.
     * This method has function ordinal 1 in rfscustcmd.dll.
     *
     * @return A custom command object.
     */
	IMPORT_C static MSsmCustomCommand* RfsCustCmdFirstBootNewL();

    /**
     * Creates and returns a custom command of type CRfsCustCmd for deep rfs.
     * This method has function ordinal 2 in rfscustcmd.dll.
     *
     * @return A custom command object.
     */
	IMPORT_C static MSsmCustomCommand* RfsCustCmdDeepNewL();

    /**
     * Creates and returns a custom command of type CRfsCustCmd for pre ui rfs.
     * This method has function ordinal 3 in rfscustcmd.dll.
     *
     * @return A custom command object.
     */
	IMPORT_C static MSsmCustomCommand* RfsCustCmdPreUiNewL();

    /**
     * Creates and returns a custom command of type CRfsCustCmd for post ui rfs.
     * This method has function ordinal 4 in rfscustcmd.dll.
     *
     * @return A custom command object.
     */
	IMPORT_C static MSsmCustomCommand* RfsCustCmdPostUiNewL();

    };

#endif // RFSCUSTCMDFACTORY
