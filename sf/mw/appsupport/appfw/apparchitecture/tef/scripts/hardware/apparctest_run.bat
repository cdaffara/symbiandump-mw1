@rem
@rem Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

testexecute.exe z:\apparctest\apparctest_t_AppList.script
testexecute.exe z:\apparctest\apparctest_t_AppListFileUpdate.script
testexecute.exe z:\apparctest\apparctest_t_ApsScan.script 
testexecute.exe z:\apparctest\apparctest_t_AutoMMCReaderOpen.script
testexecute.exe z:\apparctest\apparctest_t_Backup.script
testexecute.exe z:\apparctest\apparctest_t_Capability1.script
testexecute.exe z:\apparctest\apparctest_t_Capability2.script
testexecute.exe z:\apparctest\apparctest_t_DataTypeMappingWithSid.script
testexecute.exe z:\apparctest\apparctest_t_Caption.script
testexecute.exe z:\apparctest\apparctest_t_Cmdln.script
testexecute.exe z:\apparctest\apparctest_t_ControlPanelTest.script
testexecute.exe z:\apparctest\apparctest_T_DataMappingPersistenceA.script
testexecute.exe z:\apparctest\apparctest_T_DataMappingPersistenceB.script
testexecute.exe z:\apparctest\apparctest_T_DataMappingPersistenceC.script
testexecute.exe z:\apparctest\apparctest_t_DriveNotification.script
testexecute.exe z:\apparctest\apparctest_t_EndTask.script
testexecute.exe z:\apparctest\apparctest_t_Exe.script
testexecute.exe z:\apparctest\apparctest_t_File2.script
testexecute.exe z:\apparctest\apparctest_t_File3.script
testexecute.exe z:\apparctest\apparctest_t_Foreground.script
testexecute.exe z:\apparctest\apparctest_t_GroupName.script
testexecute.exe z:\apparctest\apparctest_t_GroupName_ver1.script
testexecute.exe z:\apparctest\apparctest_t_GroupName_ver2.script
testexecute.exe z:\apparctest\apparctest_T_IntegritySupport.script
testexecute.exe z:\apparctest\apparctest_T_IntegritySupportReboot1.script
testexecute.exe z:\apparctest\apparctest_T_IntegritySupportReboot2.script
testexecute.exe z:\apparctest\apparctest_t_largestack.script
testexecute.exe z:\apparctest\apparctest_t_Locale.script
testexecute.exe z:\apparctest\apparctest_t_Mdr.script
testexecute.exe z:\apparctest\apparctest_t_mimecontentpolicy.script
testexecute.exe z:\apparctest\apparctest_t_Mru.script
testexecute.exe z:\apparctest\apparctest_t_NonNativeApps.script
testexecute.exe z:\apparctest\apparctest_t_Notif.script
testexecute.exe z:\apparctest\apparctest_t_OOM.script
testexecute.exe z:\apparctest\apparctest_t_Pro.script
testexecute.exe z:\apparctest\apparctest_t_Proc.script
testexecute.exe z:\apparctest\apparctest_t_RApaLsSession.script
testexecute.exe z:\apparctest\apparctest_t_RuleBasedLaunching.script
testexecute.exe z:\apparctest\apparctest_t_Serv2.script
testexecute.exe z:\apparctest\apparctest_t_Serv3.script
testexecute.exe z:\apparctest\apparctest_t_ServiceBase.script
testexecute.exe z:\apparctest\apparctest_t_ServiceRegistry.script
testexecute.exe z:\apparctest\apparctest_t_Services.script
testexecute.exe z:\apparctest\apparctest_t_StartApp.script
testexecute.exe z:\apparctest\apparctest_t_StartDoc.script
testexecute.exe z:\apparctest\apparctest_t_Wgnam.script
testexecute.exe z:\apparctest\apparctest_t_WindowChaining.script
testexecute.exe z:\apparctest\apparctest_t_RecUpgrade.script
testexecute.exe z:\apparctest\apparctest_t_UpdateAppList.script
testexecute.exe z:\apparctest\apparctest_t_forcereg.script
testexecute.exe z:\apparctest\apparctest_t_clientnotif.script
testexecute.exe z:\apparctest\apparctest_t_nonnativetest.script


:: *******************************************************************
:: This batch file is used to run tests on 9.3 and higher versions.
::
:: Any ADDITIONS or DELETIONS to the test cases in this script MUST
:: be copied to the partner script that is used on hardware:
:: ..\emulator\apparctest_run.bat
:: *******************************************************************

:: Logs are copied to MMC which is assumed to be drive E:
md e:\logs
md e:\logs\testexecute
copy c:\logs\testexecute\apparctest*.htm e:\logs\testexecute\
