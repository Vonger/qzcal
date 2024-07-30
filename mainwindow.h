#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
public slots:
    void onCheck(bool);
    void onGenPdf();

public:
    void createAdd(int &a, int &b, int min, int max);
    void createSub(int &a, int &b, int min, int max);
    void createMul(int &a, int &b, int min, int max);
    void createDiv(int &a, int &b, int min, int max);
    
private:
    Ui::MainWindow *ui;
    QRandomGenerator rg;
};
#endif // MAINWINDOW_H
