-If test cases fail, try reallycleaning and exporting test data before rerunning tests. Run from group folder:
    envsetup
    bldmake bldfiles
    abld test cleanexport
    abld test export
-In a very loaded system (e.g. when running tests in emulator) cases may fail if CF listener thread does not get initialized. Hence lengthening delay in CSensorDataCompensatorPlgTest::CreateCompensatorPluginL may help.
