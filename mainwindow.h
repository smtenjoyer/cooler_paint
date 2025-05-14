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
    void createActions();
    void createMenus();
    void createToolBars();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormet);
    DoodleArea *doodleArea;
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

    QToolBar *toolBar;

    QAction *undoAct;
    QAction *redoAct;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
    void updateBrushPreview(QLabel *label, int width, QColor color);

    void setFillTool();
    void setPencilTool();

    void undo();
    void redo();
};

#endif // MAINWINDOW_H
