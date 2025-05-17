#include "command.h"
#include "doodlearea.h"
#include <QPainter>

DrawLineCommand::DrawLineCommand(DoodleArea *area, const QPointF &startPoint, const QPointF &endPoint,
                                 const QColor &color, int width)
    : QUndoCommand("Draw Line"),
    area(area),
    startPoint(startPoint),
    endPoint(endPoint),
    color(color),
    width(width),
    oldImage(area->getImage())
{
}

void DrawLineCommand::undo() {
    area->setImage(oldImage);
    area->update();
}

void DrawLineCommand::redo() {
    QImage image = area->getImage();
    QPainter painter(&image);
    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);
    area->setImage(image);
    area->update();
}
