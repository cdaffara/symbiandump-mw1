

#include "secondpluginfortest.h"
#include "secondpluginviewfortest.h"
#include <cpsettingformentryitemdataimpl.h>

SecondPluginForTest::SecondPluginForTest()
{
}

SecondPluginForTest::~SecondPluginForTest()
{
}
QList<CpSettingFormItemData*> SecondPluginForTest::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<SecondPluginViewForTest>(
               itemDataHelper,
               tr("The plugin for test"), 
               tr("TestPlugin."));
}

CpBaseSettingView *SecondPluginForTest::createSettingView(const QVariant &hint) const
{
    SecondPluginViewForTest *pView = new SecondPluginViewForTest();
    return pView;
}

Q_EXPORT_PLUGIN2(SecondPluginForTest, SecondPluginForTest);
