#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent> // Для обработки событий мыши
// #include <QScrollArea>

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
    // Ui::MainWindow *ui;

    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormet);
    DoodleArea *doodleArea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct; //
    QAction *aboutQtAct; //
    QAction *exitAct;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
};

// ~MainWindow();

#endif // MAINWINDOW_H
