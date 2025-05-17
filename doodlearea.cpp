#include <QtWidgets>
#include "doodlearea.h"
#include "command.h"

DoodleArea::DoodleArea(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    doodling = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
    currentTool = Pencil;
    undoStack = new QUndoStack(this);
    setMouseTracking(true);
}

DoodleArea::DoodleArea(const QSize& size, QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    doodling = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
    currentTool = Pencil;
    undoStack = new QUndoStack(this);
    setMouseTracking(true);

    image = QImage(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    setFixedSize(size);
}
void DoodleArea::setScaleFactor(double scaleFactor) {
    m_scaleFactor = scaleFactor;
    update();
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
    image.fill(QColor(255,255,255));
    modified = true;
    update();
}


void DoodleArea::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        if (currentTool == Fill) {
            fillArea(event->pos());
        } else if (currentTool == Pencil
                   ||currentTool == Rubber
                   || currentTool == Rectangle
                   || currentTool == Ellipse
                   || currentTool == Line ){
            lastPoint = event->pos();
            doodling = true;
        }
    }
}

void DoodleArea::mouseMoveEvent(QMouseEvent *event) {
    if (doodling) {
        QPoint endPoint = event->pos();
        if (currentTool == Pencil || currentTool == Rubber) {
            drawLineTo(endPoint);
        } else {
            update();
        }
    }
}

void DoodleArea::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && doodling) {
        QPoint endPoint = event->pos();
        if (currentTool != Pencil) {
            drawShape(endPoint);
        }
        doodling = false;
    }
}


void DoodleArea::setTool(ShapeType tool) {
    currentTool = tool;
    doodling = false;
}

void DoodleArea::undo() {
    undoStack->undo();
}

void DoodleArea::redo() {
    undoStack->redo();
}

void DoodleArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // Для более гладкой отрисовки

    // Сохраняем текущее состояние painter
    painter.save();

    painter.translate(m_offset);
    painter.scale(m_scaleFactor, m_scaleFactor);

    painter.drawImage(0, 0, image);

    painter.restore();

    QPen framePen(Qt::black, 1, Qt::SolidLine);
    painter.setPen(framePen);
    painter.setBrush(Qt::NoBrush);

    QRect imageRect(m_offset.x(), m_offset.y(), image.width() * m_scaleFactor, image.height() * m_scaleFactor);
    painter.drawRect(imageRect);
}

void DoodleArea::resizeEvent(QResizeEvent *event) {
    if (event->size().width() > image.width() || event->size().height() > image.height()) {
        QImage newImage(event->size(), QImage::Format_ARGB32_Premultiplied);
        newImage.fill(QColor(255,255,255));

        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), image);
        image = newImage;
    }
    QWidget::resizeEvent(event);
}

void DoodleArea::drawLineTo(const QPoint &endPoint){

    QPainter painter(&image);
    if (currentTool == Pencil){
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    } else if (currentTool == Rubber){
        painter.setPen(QPen(QColor(255,255,255), myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    QPointF scaledStartPoint = lastPoint;
    QPointF scaledEndPoint = endPoint;
    DrawLineCommand *command = new DrawLineCommand(this, scaledStartPoint, scaledEndPoint, myPenColor, myPenWidth);
    undoStack->push(command);
    lastPoint = endPoint;
    update();
}

QImage DoodleArea::getImage() const {
    return image;
}

void DoodleArea::setImage(const QImage &newImage) {
    image = newImage;
    if (imageItem) {
        imageItem->setPixmap(QPixmap::fromImage(image));
    }
    update(); // Важно обновить виджет, чтобы увидеть изменения
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

void DoodleArea::fillArea(const QPoint &seedPoint) {
    if (!image.valid(seedPoint)) {
        qDebug() << "Seed point is invalid!";
        return;
    }

    QColor targetColor = image.pixelColor(seedPoint); 
    if (targetColor == myPenColor) {
        // Область уже залита этим цветом, ничего не делаем
        return;
    }

    QQueue<QPoint> pointsToFill; // Очередь точек для заливки
    pointsToFill.enqueue(seedPoint); // Добавляем начальную точку в очередь

    QImage newImage = image; // Создаем копию image для работы

    while (!pointsToFill.isEmpty()) {
        QPoint currentPoint = pointsToFill.dequeue();
        int x = currentPoint.x();
        int y = currentPoint.y();

        // Проверка границ изображения уже выполняется image.valid(seedPoint)
        if (!newImage.valid(currentPoint)) {
            continue;
        }


        // Проверка, что текущая точка имеет целевой цвет
        if (newImage.pixelColor(x, y) == targetColor) {
            newImage.setPixelColor(x, y, myPenColor); // Заливаем пиксель новым цветом
            // Добавляем соседние пиксели в очередь
            pointsToFill.enqueue(QPoint(x + 1, y));
            pointsToFill.enqueue(QPoint(x - 1, y));
            pointsToFill.enqueue(QPoint(x, y + 1));
            pointsToFill.enqueue(QPoint(x, y - 1));
        }
    }

    image = newImage; // Заменяем исходное изображение новой версией

    modified = true;
    update();
}

void DoodleArea::drawShape(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine));

    switch (currentTool) {
    case Line:
        painter.drawLine(lastPoint, endPoint);
        modified = true;
        break;
    case Rectangle:
        painter.drawRect(QRect(lastPoint, endPoint).normalized());
        modified = true;
        break;
    case Ellipse:
        painter.drawEllipse(QRect(lastPoint, endPoint).normalized());
        modified = true;
        break;
    default:
        break;
    }

    update();
    lastPoint = endPoint;
}














