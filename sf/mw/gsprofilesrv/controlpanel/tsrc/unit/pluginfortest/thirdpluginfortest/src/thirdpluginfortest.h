

//This class is used for the test case "testCpPluginInterface()". CpTestPlugin is a subclass from CpPluginInterface.

#ifndef THIRDPLUGINFORTEST_H
#define THIRDPLUGINFORTEST_H

#include <qobject.h>
#include <cpplugininterface.h>
#include <cplauncherinterface.h>

class ThirdPluginForTest : public QObject, public CpPluginInterface, public CpLauncherInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    Q_INTERFACES(CpLauncherInterface)
public:
    ThirdPluginForTest();
    virtual ~ThirdPluginForTest();
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;
    virtual CpBaseSettingView *createSettingView(const QVariant &hint) const;
};

#endif
