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
    
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
