#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->cbAdd, SIGNAL(toggled(bool)), SLOT(onCheck(bool)));
    connect(ui->cbSub, SIGNAL(toggled(bool)), SLOT(onCheck(bool)));
    connect(ui->cbMul, SIGNAL(toggled(bool)), SLOT(onCheck(bool)));
    connect(ui->cbDiv, SIGNAL(toggled(bool)), SLOT(onCheck(bool)));
    connect(ui->pbDo, SIGNAL(clicked(bool)), SLOT(onGenPdf()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCheck(bool checked)
{
    if (sender() == ui->cbAdd) {
        ui->leAddMin->setEnabled(checked);
        ui->leAddMax->setEnabled(checked);
        ui->leAddRate->setEnabled(checked);
        ui->leAddRate->setText("0");
    }
    if (sender() == ui->cbSub) {
        ui->leSubMin->setEnabled(checked);
        ui->leSubMax->setEnabled(checked);
        ui->leSubRate->setEnabled(checked);
        ui->leSubRate->setText("0");
    }
    if (sender() == ui->cbMul) {
        ui->leMulMin->setEnabled(checked);
        ui->leMulMax->setEnabled(checked);
        ui->leMulRate->setEnabled(checked);
        ui->leMulRate->setText("0");    
    }
    if (sender() == ui->cbDiv) {
        ui->leDivMin->setEnabled(checked);
        ui->leDivMax->setEnabled(checked);
        ui->leDivRate->setEnabled(checked);
        ui->leDivRate->setText("0");
    }
}

void MainWindow::onGenPdf()
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(qApp->desktopFileName() + "output.pdf");
    qDebug() << qApp->desktopFileName() + "output.pdf";

    enum SIGN { ADD, SUB, MUL, DIV };
    QList<int> data;

    QRandomGenerator rg = QRandomGenerator::securelySeeded();
    // now generate questions to fill data first.
    if (ui->cbAdd->isChecked()) {
        int count = ui->leAddRate->text().toInt();
        int max = ui->leAddMax->text().toInt();
        int min = ui->leAddMin->text().toInt();

        for (int i = 0; i < count; i++) {
             int a, b;

             a = rg.generate() % (max - min);
             b = (max - min) - a;
             b = rg.generate() % b;
             a = a + min;
             b = b + min;
             data << a << ADD << b;
        }
    }

    if (ui->cbSub->isChecked()) {
        int count = ui->leSubRate->text().toInt();
        int max = ui->leAddMax->text().toInt();
        int min = ui->leAddMin->text().toInt();

        for (int i = 0; i < count; i++) {
             int a, b;

             a = rg.generate() % (max - min);
             b = rg.generate() % (max - min);
             a = a + min;
             b = b + min;
             if (a > b)
                data << a << SUB << b;
             else
                data << b << SUB << a;
        }
    }

    if (ui->cbMul->isChecked()) {
        int count = ui->leMulRate->text().toInt();
        int max = ui->leMulMax->text().toInt();
        int min = ui->leMulMin->text().toInt();

        for (int i = 0; i < count; i++) {
             int a, b;
             a = rg.generate() % (max - min);
             b = rg.generate() % (max - min);
             a = a + min;
             b = b + min;
             data << a << MUL << b;
        }
    }

    // note: current input min and max is same way like mul.
    if (ui->cbDiv->isChecked()) {
        int count = ui->leDivRate->text().toInt();
        int max = ui->leDivMax->text().toInt();
        int min = ui->leDivMin->text().toInt();

        for (int i = 0; i < count; i++) {
             int a, b, m;
             a = rg.generate() % (max - min);
             b = rg.generate() % (max - min);
             a = a + min;
             b = b + min;
             m = rg.generate() % (max - min);
             if (ui->cbMod->isChecked()) {
                data << a * b + m << DIV << a;
             } else {
                data << a * b << DIV << a;
             }
        }
    }

    QString html;
    QString sign[4] = {QString("+"), QString("-"), QString("×"), QString("÷")};
    int index = 0;

    html += "<table>";
    while (data.size() > 0) {
        switch (index % 4) {
        case 0: html += "<tr>";
        case 1:
        case 2:
        case 3: html += "<td style=\"padding-bottom:8px;\" colspan=32><h2>"; break;
        }

        int pos = (rg.generate() % data.size()) / 3;
        int a = data.at(pos * 3), s = data.at(pos * 3 + 1), b = data.at(pos * 3 + 2);
        data.remove(pos * 3, 3);

        html += QString().asprintf("%d", a);
        html += QString(" ") + sign[s] + QString(" ");
        html += QString().asprintf("%d", b);
        html += QString(" =");

        switch (index % 4) {
        case 0:
        case 1:
        case 2: break;
        case 3: html += "</h2></td></tr>"; break;
        }

        index++;
    }
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);
}

