#include <QtWidgets>

#include "mainwindow.h"
#include "doodlearea.h"

MainWindow::MainWindow()
{
    doodleArea = new DoodleArea;
    setCentralWidget(doodleArea);
    createActions();
    createMenus();
    createToolBars();
    setWindowTitle(tr("cooler_paint"));
    resize(500, 500);
}

void MainWindow::createToolBars() {
    toolBar = new QToolBar(this);
    addToolBar(Qt::LeftToolBarArea, toolBar);

    toolBar->addAction(PencilAct);
    toolBar->addAction(fillAreaAct);

    toolBar->addSeparator();
    toolBar->addAction(penColorAct);
    toolBar->addAction(penWidthAct);
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    } else{
        event->ignore();
    }
}

void MainWindow::open(){
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("Открыть"), QDir::currentPath());
        if(!fileName.isEmpty()){
            doodleArea->openImage(fileName);
        }
    }
}

void MainWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor(){
    QColor newColor = QColorDialog::getColor((doodleArea->penColor()));
    if(newColor.isValid()){
        doodleArea->setPenColor(newColor);
    }
}

void MainWindow::penWidth() {
    QDialog dialog(this);
    dialog.setWindowTitle(tr("cooler_paint - Толщина кисти"));

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(1, 50);
    slider->setValue(doodleArea->penWidth());

    QLabel *previewLabel = new QLabel();
    previewLabel->setAlignment(Qt::AlignCenter);
    updateBrushPreview(previewLabel, doodleArea->penWidth(), doodleArea->penColor()); // Изначальное отображение

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(slider);
    layout->addWidget(previewLabel);

    QObject::connect(slider, &QSlider::valueChanged, this, [=](int value) {
        updateBrushPreview(previewLabel, value, doodleArea->penColor());
    });

    QObject::connect(slider, &QSlider::valueChanged, doodleArea, &DoodleArea::setPenWidth);

    dialog.exec();
}

void MainWindow::updateBrushPreview(QLabel *label, int width, QColor color) {
    int size = 60;
    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QPoint center(size / 2, size / 2);
    painter.drawPoint(center);

    painter.end();

    label->setPixmap(QPixmap::fromImage(image)); // Преобразуем QImage в QPixmap
}

void MainWindow::about(){
    QMessageBox::about(this, tr("О программе cooler_paint"),
                       tr("<p> This is <b>cooler</b> paint for win11 </p>"));
}

void MainWindow::undo()
{
    doodleArea->getUndoStack()->undo();
}

void MainWindow::redo()
{
    doodleArea->getUndoStack()->redo();
}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Открыть"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    exitAct = new QAction(tr("В&ыйти"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(QIcon(":/images/Color.png"), "Цвет", this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction(QIcon(":/images/Width.png"), "Толщина линии", this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Очистить изображение..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), doodleArea, SLOT(clearImage()));

    fillAreaAct = new QAction(QIcon(":/images/fill.png"), "Заливка", this);
    connect(fillAreaAct, SIGNAL(triggered()), this, SLOT(setFillTool()));

    PencilAct = new QAction(QIcon(":/images/Pencil.png"), "Карандаш", this);
    connect(PencilAct, SIGNAL(triggered()), this, SLOT(setPencilTool()));

    aboutAct = new QAction(tr("&О программе..."), this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));
    aboutQtAct = new QAction(tr("&О &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcut(tr("Ctrl+Y"));

    undoAct->setEnabled(false);
    redoAct->setEnabled(false);

    connect(undoAct, &QAction::triggered, doodleArea->getUndoStack(), &QUndoStack::undo);
    connect(redoAct, &QAction::triggered, doodleArea->getUndoStack(), &QUndoStack::redo);

    connect(doodleArea->getUndoStack(), &QUndoStack::canUndoChanged, undoAct, &QAction::setEnabled);
    connect(doodleArea->getUndoStack(), &QUndoStack::canRedoChanged, redoAct, &QAction::setEnabled);
}


void MainWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Сохранить как..."), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&Файл"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Настройки"), this);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Помощь"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    editMenu = new QMenu(tr("&Правка"), this);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
    menuBar()->addMenu(editMenu);
}

void MainWindow::setFillTool() {
    doodleArea->setTool(DoodleArea::Fill);
}

void MainWindow::setPencilTool() {
    doodleArea->setTool(DoodleArea::Pencil);
}

bool MainWindow::maybeSave(){
    if(doodleArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("cooler_paint"),
                                   tr("Изображение изменено. \n"
                                      "Хотите сохранить изменения?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save){
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}


bool MainWindow::saveFile(const QByteArray &fileFormat){
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить изображение"),
                                                    initialPath,
                                                    tr("%1 Files(*.%2);; All Files(*")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                        .arg(QString::fromLatin1(fileFormat)));
    if(fileName.isEmpty()){
        return false;
    } else {
        return doodleArea->saveImage(fileName, fileFormat.constData());
    }
}
