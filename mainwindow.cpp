#include <QtWidgets>

#include "mainwindow.h"
#include "doodlearea.h"

MainWindow::MainWindow()
{
    doodleArea = new DoodleArea();
    setCentralWidget(doodleArea);
    createActions();
    createMenus();
    createToolBars();
    setWindowTitle(tr("cooler_paint"));
    resize(500, 500);
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


void MainWindow::createActions(){
    newAction = new QAction(tr("&Ноывй холст"), this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newCanvas);

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

    clearScreenAct = new QAction(tr("&Очистить изображение..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), doodleArea, SLOT(clearImage()));

    resizeAction = new QAction(tr("&Изменить размер холста..."), this);
    connect(resizeAction, SIGNAL(triggered()), doodleArea, SLOT(resizeCanvas()));

    exitAct = new QAction(tr("В&ыйти"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(QIcon(":/images/Color.png"), "Цвет", this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction(QIcon(":/images/Width.png"), "Толщина линии", this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));



    fillAreaAct = new QAction(QIcon(":/images/fill.png"), "Заливка", this);
    connect(fillAreaAct, SIGNAL(triggered()), this, SLOT(setFillTool()));

    PencilAct = new QAction(QIcon(":/images/Pencil.png"), "Карандаш", this);
    RubberAct = new QAction(QIcon(":/images/Pencil.png"), "Ластик", this);

    lineAction = new QAction(QIcon(":/images/Line.png"), tr("&Прямая"), this);
    rectangleAction = new QAction(QIcon(":/images/Rectangle.png"), tr("&Ч"), this);
    ellipseAction = new QAction(QIcon(":/images/Ellipse.png"), tr("&Э"), this);

    TextAct = new QAction(QIcon(":/images/Pencil.png"), "Text", this);

    // undoActionBtn = new QAction(tr("&Undo"), this);
    // undoActionBtn->setShortcut(QKeySequence::Undo);
    // connect(undoActionBtn, &QAction::triggered, this, &MainWindow::undoAction);

    // redoActionBtn = new QAction(tr("&Redo"), this);
    // redoActionBtn->setShortcut(QKeySequence::Redo);
    // connect(redoActionBtn, &QAction::triggered, this, &MainWindow::redoAction);

    QActionGroup *toolGroup = new QActionGroup(this);
    toolGroup->addAction(PencilAct);
    toolGroup->addAction(RubberAct);
    toolGroup->addAction(fillAreaAct);
    toolGroup->addAction(lineAction);
    toolGroup->addAction(rectangleAction);
    toolGroup->addAction(ellipseAction);
    toolGroup->addAction(TextAct);

    PencilAct->setCheckable(true);
    fillAreaAct->setCheckable(true);
    lineAction->setCheckable(true);
    rectangleAction->setCheckable(true);
    ellipseAction->setCheckable(true);
    TextAct->setCheckable(true);

    PencilAct->setChecked(true);

    connect(PencilAct, SIGNAL(triggered()), this, SLOT(setPencilTool()));
    connect(RubberAct, SIGNAL(triggered()), this, SLOT(setRubberTool()));
    connect(fillAreaAct, SIGNAL(triggered()), this, SLOT(setFillTool()));
    connect(lineAction, &QAction::triggered, this, &MainWindow::setLineTool);
    connect(rectangleAction, &QAction::triggered, this, &MainWindow::setRectangleTool);
    connect(ellipseAction, &QAction::triggered, this, &MainWindow::setEllipseTool);
    connect(TextAct, &QAction::triggered, this, &MainWindow::setTextTool);

    aboutAct = new QAction(tr("&О программе..."), this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));
    aboutQtAct = new QAction(tr("&О &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    // undoActionBtn->setShortcut(tr("Ctrl+Z"));
    // redoActionBtn->setShortcut(tr("Ctrl+Y"));
}


void MainWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Сохранить как..."), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&Файл"), this);
    fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Настройки"), this);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(resizeAction);

    helpMenu = new QMenu(tr("&Помощь"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    editMenu = new QMenu(tr("&Правка"), this);
    // editMenu->addAction(undoActionBtn);
    // editMenu->addAction(redoActionBtn);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
    menuBar()->addMenu(editMenu);

}

void MainWindow::createToolBars() {
    toolBar = new QToolBar(this);
    addToolBar(Qt::LeftToolBarArea, toolBar);

    toolBar->addAction(PencilAct);
    toolBar->addAction(RubberAct);
    toolBar->addAction(fillAreaAct);
    toolBar->addAction(lineAction);
    toolBar->addAction(rectangleAction);
    toolBar->addAction(ellipseAction);
    toolBar->addAction(TextAct);

    toolBar->addSeparator();
    toolBar->addAction(penColorAct);
    toolBar->addAction(penWidthAct);
}


void MainWindow::setFillTool() {
    doodleArea->setTool(DoodleArea::Fill);
}

void MainWindow::setPencilTool() {
    doodleArea->setTool(DoodleArea::Pencil);
}
void MainWindow::setRubberTool() {
    doodleArea->setTool(DoodleArea::Rubber);
}

void MainWindow::setLineTool() {
    if(doodleArea){
        doodleArea->setTool(DoodleArea::Line);
    }
}

void MainWindow::setTextTool() {
    if(doodleArea){
        doodleArea->setTool(DoodleArea::Textt);
    }
}

void MainWindow::setRectangleTool() {
    if(doodleArea){
        doodleArea->setTool(DoodleArea::Rectangle);
    }
}

void MainWindow::setEllipseTool() {
    if(doodleArea){
        doodleArea->setTool(DoodleArea::Ellipse);
    }
}

void MainWindow::undoAction() {
    if (doodleArea) {
        doodleArea->undo();
    }
}

void MainWindow::redoAction() {
    if (doodleArea) {
        doodleArea->redo();
    }
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

void MainWindow::newCanvas() {
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Новый холст"));

    QFormLayout form(&dialog);

    QLineEdit *widthEdit = new QLineEdit(&dialog);
    widthEdit->setValidator(new QIntValidator(1, 2000, this));
    widthEdit->setText("1920");
    form.addRow(tr("Ширина:"), widthEdit);

    QLineEdit *heightEdit = new QLineEdit(&dialog);
    heightEdit->setValidator(new QIntValidator(1, 2000, this));
    heightEdit->setText("1080");
    form.addRow(tr("Высота:"), heightEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        bool okWidth, okHeight;
        int width = widthEdit->text().toInt(&okWidth);
        int height = heightEdit->text().toInt(&okHeight);

        if (okWidth && okHeight) {
            QSize size(width, height);

            if (doodleArea) {
                delete doodleArea;
            }
            doodleArea = new DoodleArea(size);

            if (!scrollArea) {
                scrollArea = new QScrollArea;
                scrollArea->setWidgetResizable(true);
                setCentralWidget(scrollArea);
            }

            scrollArea->setWidget(doodleArea);
            doodleArea->show();
        } else {
            QMessageBox::warning(this, tr("Ошибка"), tr("Пожалуйста, введите корректные числа от 1 до 2000."));
        }
    }
}
