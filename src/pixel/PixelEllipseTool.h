#pragma once

#include "kis_tool_ellipse_base.h"
#include "KisToolPaintFactoryBase.h"
#include <kis_icon.h>

class KoCanvasBase;

class PixelEllipseTool : public KisToolEllipseBase
{
    Q_OBJECT
public:
    explicit PixelEllipseTool(KoCanvasBase *canvas);

protected:
    void finishRect(const QRectF &rect, qreal roundCornersX, qreal roundCornersY) override;
};

class PixelEllipseToolFactory : public KisToolPaintFactoryBase
{
public:
    PixelEllipseToolFactory()
        : KisToolPaintFactoryBase("KritaShape/PixelEllipseTool")
    {
        setToolTip(i18n("Pixel Ellipse"));
        setSection(ToolBoxSection::Shape);
        setActivationShapeId(KRITA_TOOL_ACTIVATION_ID);
        setIconName(koIconNameCStr("krita_tool_ellipse"));
        setPriority(4);
    }

    ~PixelEllipseToolFactory() override {}

    KoToolBase *createTool(KoCanvasBase *canvas) override
    {
        return new PixelEllipseTool(canvas);
    }
};
