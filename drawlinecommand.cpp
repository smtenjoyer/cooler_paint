#include "drawlinecommand.h"
#include "doodlearea.h"
#include <QPainter>

DrawLineCommand::DrawLineCommand(DoodleArea *area, const QPoint &start, const QPoint &end,
                                 const QColor &c, int w, const QImage& oldImage)
    : Command(area, "Draw Line"), startPoint(start), endPoint(end), color(c), width(w), oldImage(oldImage)
{
}

void DrawLineCommand::undo()
{
    doodleArea->setImage(oldImage);
    doodleArea->update(); // Обновляем экран
}

void DrawLineCommand::redo()
{
    QImage tempImage = doodleArea->getImage();

    QPainter painter(&tempImage);
    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);

    doodleArea->setImage(tempImage);
    doodleArea->update(); // Обновляем экран
}
