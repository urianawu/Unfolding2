#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "meshviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool createComponents();
    bool layoutComponents();
    bool connectComponents();

protected:
    void createActions();
    void createMenus();
    void createDock();
    void createStatusBar();

private slots:
    void slot_newFile();

private:
    void loadOBJFile();

private:
    Ui::MainWindow *ui;
    QMap<QString, QAction*> actionsMap;
    MeshViewer *viewer;

};

#endif // MAINWINDOW_H
