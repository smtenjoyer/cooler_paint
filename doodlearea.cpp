#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "doodlearea.h"

DoodleArea::DoodleArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents); //21:20
    doodling = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;

}

bool DoodleArea::openImage(const QString &fileName){
    QImage loadedImage;
    if(!loadedImage.load(fileName)){
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool DoodleArea::saveImage(const QString &fileName, const char *fileFormat){
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat)){
        modified = false;
        return true;
    } else {
        return false;
    }
}

void DoodleArea::setPenColor(const QColor &newColor){
    myPenColor = newColor;
}

void DoodleArea::setPenWidth(int newWidth){
    myPenWidth = newWidth;
}

void DoodleArea::clearImage(){
    image.fill(qRgb(255,255,255));
    modified = true;
    update();
}

void DoodleArea::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        lastPoint = event->pos();
        doodling = true;
    }
}

void DoodleArea::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons() & Qt::LeftButton) && doodling)
        drawLineTo(event->pos());
}
void DoodleArea::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton && doodling){
        drawLineTo(event->pos());
        doodling = false;
    }
}

void DoodleArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void DoodleArea::resizeEvent(QResizeEvent *event){
    if(width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DoodleArea::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void DoodleArea::resizeImage(QImage *image, const QSize &newSize){
    if(image->size() == newSize){
        return;
    }

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}
























