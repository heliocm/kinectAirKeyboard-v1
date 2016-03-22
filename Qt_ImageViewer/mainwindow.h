#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imagefile.h>
#include <list>
class BinNotFoundException{
};

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    ImageFile* root;
    Q_OBJECT
    list<ImageFile*> files;
    void getFiles(ImageFile* f);
public:
    explicit MainWindow(QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent * event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
