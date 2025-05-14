#ifndef DOODLEAREA_H
#define DOODLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QUndoStack>

class DoodleArea : public QWidget
{
    Q_OBJECT

public:
    enum Tool {
        Pencil,
        Fill
    };

public:
    DoodleArea(QWidget *parent = 0);
    bool openImage(const QString &filename);
    bool saveImage(const QString &filename, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    bool isModified() const {return modified;}
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}
    QUndoStack* getUndoStack() const;
    void setTool(Tool tool);
    QImage getImage() const;
    void setImage(const QImage &newImage);

public slots:
    void clearImage();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void fillArea(const QPoint &seedPoint); // Делаем private

    bool modified;
    bool doodling;

    QColor myPenColor;
    int myPenWidth;

    QImage image;
    QPoint lastPoint;
    Tool currentTool;

    QUndoStack *undoStack;
};

#endif // DOODLEAREA_H
