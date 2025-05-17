#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QPointF>
#include <QColor>
#include <QImage>

class DoodleArea; // Forward declaration

class DrawLineCommand : public QUndoCommand {
public:
    DrawLineCommand(DoodleArea *area, const QPointF &startPoint, const QPointF &endPoint,
                    const QColor &color, int width);

    void undo() override;
    void redo() override;

private:
    DoodleArea *area;
    QPointF startPoint;
    QPointF endPoint;
    QColor color;
    int width;
    QImage oldImage;
};
#endif // COMMAND_H


