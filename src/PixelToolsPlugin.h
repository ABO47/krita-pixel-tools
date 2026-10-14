#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantList>

class PixelToolsPlugin : public QObject
{
    Q_OBJECT
public:
    PixelToolsPlugin(QObject *parent, const QVariantList &);
    ~PixelToolsPlugin() override;

    void injectTools();

private:
    QStringList m_toolIds;
};
