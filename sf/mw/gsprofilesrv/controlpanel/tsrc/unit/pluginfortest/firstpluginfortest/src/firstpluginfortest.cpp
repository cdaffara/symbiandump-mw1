

#include "firstpluginfortest.h"
#include "firstpluginviewfortest.h"
#include <cpsettingformentryitemdataimpl.h>

FirstPluginForTest::FirstPluginForTest()
{
}

FirstPluginForTest::~FirstPluginForTest()
{
}
QList<CpSettingFormItemData*> FirstPluginForTest::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<FirstPluginViewForTest>(
               itemDataHelper,
               tr("The plugin for test"), 
               tr("TestPlugin."));
}

CpBaseSettingView *FirstPluginForTest::createSettingView(const QVariant &hint) const
{
    return 0;
}

Q_EXPORT_PLUGIN2(FirstPluginForTest, FirstPluginForTest);
