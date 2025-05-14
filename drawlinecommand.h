#ifndef DRAWLINECOMMAND_H
#define DRAWLINECOMMAND_H

#include "command.h"
#include <QPoint>
#include <QColor>
#include <QImage>

class DrawLineCommand : public Command
{
public:
    DrawLineCommand(DoodleArea *area, const QPoint &startPoint, const QPoint &endPoint,
                    const QColor &color, int width, const QImage& oldImage);
    void undo() override;
    void redo() override;

private:
    QPoint startPoint;
    QPoint endPoint;
    QColor color;
    int width;
    QImage oldImage;
};

#endif // DRAWLINECOMMAND_H
