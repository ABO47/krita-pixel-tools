#include "PixelToolsPlugin.h"
#include "pixel/PixelEllipseTool.h"

#include <KoToolAction.h>
#include <KoToolManager_p.h>
#include <KoToolRegistry.h>
#include <QDebug>
#include <kis_debug.h>
#include <kis_global.h>
#include <kis_types.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY_WITH_JSON(PixelToolsPluginFactory, "pixeltools.json", registerPlugin<PixelToolsPlugin>();)

PixelToolsPlugin::PixelToolsPlugin(QObject *parent, const QVariantList &)
    : QObject(parent)
{
    auto addTool = [this](KoToolFactoryBase *toolFactory) {
        qDebug() << "[PixelToolsPlugin] Registering tool factory:" << toolFactory->id();
        KoToolRegistry::instance()->add(toolFactory);
        m_toolIds.append(toolFactory->id());
    };

    addTool(new PixelEllipseToolFactory());
}

PixelToolsPlugin::~PixelToolsPlugin()
{
}

// Late-loaded tools miss KoToolManager's registry poll, so append manually.
void PixelToolsPlugin::injectTools()
{
    KoToolRegistry *registry = KoToolRegistry::instance();
    KoToolManager::Private *p = KoToolManager::instance()->priv();

    for (const QString &id : registry->keys()) {
        if (m_toolIds.contains(id)) {
            qDebug() << "[PixelToolsPlugin] Injecting tool action for" << id;
            p->toolActionList.append(new KoToolAction(registry->value(id)));
        }
    }
}

extern "C" {

// Called from Python via ctypes (see python/extension.py).
Q_DECL_EXPORT void load_pixel_tools_plugin()
{
    qDebug() << "[PixelToolsPlugin] Loading PixelTools plugin via external loader.";
    PixelToolsPlugin plugin(nullptr, QVariantList());
    plugin.injectTools();
}

} // extern "C"

#include "PixelToolsPlugin.moc"
