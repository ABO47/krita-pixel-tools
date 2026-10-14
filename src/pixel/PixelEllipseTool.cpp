#include "PixelEllipseTool.h"
#include "ellipse_raster.h"

#include <KoCanvasBase.h>
#include <KoColor.h>
#include <kis_image.h>
#include <kis_paint_device.h>
#include <kis_transaction.h> // shortcut: verify header/name against v5.2.13 source at build

PixelEllipseTool::PixelEllipseTool(KoCanvasBase *canvas)
    : KisToolEllipseBase(canvas, KisToolEllipseBase::PAINT, QCursor(Qt::CrossCursor))
{
    setObjectName("tool_pixel_ellipse");
}

void PixelEllipseTool::finishRect(const QRectF &rect, qreal /*roundCornersX*/, qreal /*roundCornersY*/)
{
    if (nodePaintAbility() != NodePaintAbility::PAINT)
        return;
    KisNodeSP node = currentNode();
    if (!node || !node->paintDevice())
        return;
    if (!canvas() || !canvas()->resourceManager() || !currentImage())
        return;

    const QRect box = rect.normalized().toRect().intersected(currentImage()->bounds());
    if (box.isEmpty())
        return;

    const std::set<pixeltools::Point> pts =
        pixeltools::ellipseOutline(box.width(), box.height());
    if (pts.empty())
        return;

    KisPaintDeviceSP dev = node->paintDevice();
    const KoColor fg =
        canvas()->resourceManager()->foregroundColor().convertedTo(dev->colorSpace());
    if (!fg.data() || fg.colorSpace()->pixelSize() != dev->pixelSize())
        return;

    KisTransaction transaction(kundo2_i18n("Draw Pixel Ellipse"), dev);
    for (const auto &p : pts) {
        dev->writeBytes(fg.data(), box.x() + p.first, box.y() + p.second, 1, 1);
    }
    transaction.commit();
    dev->setDirty(box);
}
