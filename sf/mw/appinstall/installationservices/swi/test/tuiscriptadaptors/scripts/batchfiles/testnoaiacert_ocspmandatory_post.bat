@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of the License "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
REM This batch file will replace the old ocspppolicy.ini and swipolicy.ini files into 
REM z:\system\data\ after running the test for installation of testnoaiacert.script

copy /y \epoc32\release\winscw\udeb\z\testnoaiacert\ocsppolicy.ini \epoc32\release\winscw\udeb\z\system\data\ocsppolicy.ini 

copy /y \epoc32\release\winscw\udeb\z\testnoaiacert\swipolicy.ini \epoc32\release\winscw\udeb\z\system\data\swipolicy.ini

del /s /Q \epoc32\release\winscw\udeb\z\testnoaiacert