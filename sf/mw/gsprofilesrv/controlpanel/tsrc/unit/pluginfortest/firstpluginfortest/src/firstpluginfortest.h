

//This class is used for the test case "testCpPluginInterface()". CpTestPlugin is a subclass from CpPluginInterface.

#ifndef FIRSTPLUGINFORTEST_H
#define FIRSTPLUGINFORTEST_H

#include <qobject.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>

class FirstPluginForTest : public QObject, public CpPluginInterface, public CpLauncherInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    Q_INTERFACES(CpLauncherInterface)
public:
    FirstPluginForTest();
    virtual ~FirstPluginForTest();
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const;
};

#endif
