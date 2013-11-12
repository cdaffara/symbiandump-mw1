@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
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
REM Create a temporary directory to store the original localization resource files

mkdir \epoc32\release\winscw\udeb\z\resource\temp


REM Move the required localization files to another directory just to simulate the "missing localization file" condition

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r01 \epoc32\release\winscw\udeb\z\resource\temp\timezones.r01

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.rsc \epoc32\release\winscw\udeb\z\resource\temp\timezones.rsc

move \epoc32\release\winscw\udeb\z\resource\timezonelocalization\timezones.r02 \epoc32\release\winscw\udeb\z\resource\temp\timezones.r02