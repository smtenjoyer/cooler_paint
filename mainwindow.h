#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QColorDialog>
#include <QQueue>
#include <QToolBar>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DoodleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void newCanvas();

    void createActions();
    void createMenus();
    void createToolBars();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormet);

    QAction *newAction;

    DoodleArea *doodleArea = nullptr;
    QScrollArea *scrollArea = nullptr;


    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QMenu *editMenu;
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exitAct;
    QAction *fillAreaAct;
    QAction *PencilAct;
    QAction *RubberAct;
    QAction *lineAction;
    QAction *rectangleAction;
    QAction *ellipseAction;
    QAction *TextAct;

    QAction *resizeAction;
    QAction *undoActionBtn;
    QAction *redoActionBtn;


    QToolBar *toolBar;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
    void updateBrushPreview(QLabel *label, int width, QColor color);

    void setFillTool();
    void setPencilTool();
    void setRubberTool();

    void setTextTool();
    void setLineTool();
    void setRectangleTool();
    void setEllipseTool();
    void undoAction();
    void redoAction();

};

#endif // MAINWINDOW_H
