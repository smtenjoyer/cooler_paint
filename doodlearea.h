#ifndef DOODLEAREA_H
#define DOODLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QUndoStack>
#include <QScrollBar>
#include <QGraphicsPixmapItem>

class DoodleArea : public QWidget
{
    Q_OBJECT

public:
    enum ShapeType {
        Pencil,
        Rubber,
        Fill,
        Line,
        Rectangle,
        Ellipse
    };

public:
    DoodleArea(QWidget *parent = 0);
    DoodleArea(const QSize& size, QWidget *parent = nullptr);
    bool openImage(const QString &filename);
    bool saveImage(const QString &filename, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    bool isModified() const {return modified;}
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}
    QUndoStack* getUndoStack() const;
    void setTool(ShapeType tool);
    QImage getImage() const;
    void setImage(const QImage &newImage);

    void setScaleFactor(double scaleFactor);
    double scaleFactor() const { return m_scaleFactor; }
public slots:
    void clearImage();

    void undo();
    void redo();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawLineTo(const QPoint &endPoint);
    void drawShape(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void fillArea(const QPoint &seedPoint);

    bool modified = false;
    bool doodling;
    bool shaping;

    QColor myPenColor;
    int myPenWidth;

    QImage image;
    QPoint lastPoint;
    ShapeType currentTool;

    QUndoStack *undoStack;
    QGraphicsPixmapItem *imageItem;

    double m_scaleFactor = 1.0;
    QPoint m_offset;
    QPoint m_lastMousePosition;
};

#endif // DOODLEAREA_H
