#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rg = QRandomGenerator::securelySeeded();
    
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
    int min[4], max[4];

    // now generate questions to fill data first.
    if (ui->cbAdd->isChecked()) {
        int count = ui->leAddRate->text().toInt();
        max[ADD] = ui->leAddMax->text().toInt();
        min[ADD] = ui->leAddMin->text().toInt();

        for (int i = 0; i < count; i++) {
            int a, b;
            createAdd(a, b, min[ADD], max[ADD]);
            data << a << ADD << b;
        }
    }

    if (ui->cbSub->isChecked()) {
        int count = ui->leSubRate->text().toInt();
        max[SUB] = ui->leSubMax->text().toInt();
        min[SUB] = ui->leSubMin->text().toInt();

        for (int i = 0; i < count; i++) {
            int a, b;
            createSub(a, b, min[SUB], max[SUB]);
            data << a << SUB << b;
        }
    }

    if (ui->cbMul->isChecked()) {
        int count = ui->leMulRate->text().toInt();
        max[MUL] = ui->leMulMax->text().toInt();
        min[MUL] = ui->leMulMin->text().toInt();

        for (int i = 0; i < count; i++) {
            int a, b;
            createMul(a, b, min[MUL], max[MUL]);
            data << a << MUL << b;
        }
    }

    // note: current input min and max is same way like mul.
    if (ui->cbDiv->isChecked()) {
        int count = ui->leDivRate->text().toInt();
        max[DIV] = ui->leDivMax->text().toInt();
        min[DIV] = ui->leDivMin->text().toInt();

        for (int i = 0; i < count; i++) {
            int a, b;
            createDiv(a, b, min[DIV], max[DIV]);
            data << a << DIV << b;
        }
    }

    QString html;
    QString sign[4] = {QString("+"), QString("-"), QString("×"), QString("÷")};
    int index = 0;

    html += "<table>";
    while (data.size() > 0) {
        int pos = (rg.generate() % data.size()) / 3;
        int a = data.at(pos * 3), s = data.at(pos * 3 + 1), b = data.at(pos * 3 + 2);
        int c = 0, s3 = ADD, d = -1, s4 = -1;
        data.remove(pos * 3, 3);

        // code for X + Y * Z = Q likely...
        if (ui->rb3->isChecked()) {
            s3 = rg.generate() % 4;

            int r = 0;
            switch (s) {
            case ADD:
                r = a + b;
                break;

            case SUB:
                r = a - b;
                break;

            case MUL:
                r = a * b;
                break;

            case DIV:
                r = a / b;
                break;
            }

            switch (s3) {
            case ADD:
                createAdd(c, d, min[ADD], max[ADD]);
                break;

            case SUB:
                createSub(c, d, 0, r + 1);
                break;

            case MUL:
                createMul(c, d, min[MUL], max[MUL]);
                if (s == ADD || s == SUB) {
                    s4 = s3;
                    s3 = rg.generate() % 2;
                    if (s3 == SUB && (r - c * d) < 0)
                        s3 = ADD;
                }
                break;

            case DIV:
                createDiv(c, d, min[DIV], max[DIV]);
                if (s != DIV) {
                    s4 = s3;
                    s3 = rg.generate() % 2;
                    if (s3 == SUB && (r - c / d) < 0)
                        s3 = ADD;
                } else {
                    s4 = s3;
                    s3 = rg.generate() % 3;
                    if (s4 == SUB && (r - c / d) < 0)
                        s3 = ADD;
                }
                break;
            }
        }

        if (!ui->rb3->isChecked()) {
            switch (index % 4) {
            case 0: html += "<tr>";
            case 1:
            case 2:
            case 3: html += "<td style=\"padding-bottom:8px;\" colspan=32><h2>"; break;
            }
        } else {
            switch (index % 3) {
            case 0: html += "<tr>";
            case 1:
            case 2: html += "<td style=\"padding-bottom:8px;\" colspan=32><h2>"; break;
            }
        }

        // generate text.
        html += QString().asprintf("%d", a);
        html += QString(" ") + sign[s] + QString(" ");
        html += QString().asprintf("%d", b);
        if (ui->rb3->isChecked()) {
            html += QString(" ") + sign[s3] + QString(" ");
            html += QString().asprintf("%d", c);
            if (s4 >= 0) {
                html += QString(" ") + sign[s4] + QString(" ");
                html += QString().asprintf("%d", d);
            }
        }
        html += QString(" =");

        if (s4 < 0)
            qDebug() << a << sign[s] << b << sign[s3] << c;
        else
            qDebug() << a << sign[s] << b << sign[s3] << c << sign[s4] << d;

        if (!ui->rb3->isChecked()) {
            switch (index % 4) {
            case 0:
            case 1:
            case 2: break;
            case 3: html += "</h2></td></tr>"; break;
            }
        } else {
            switch (index % 3) {
            case 0:
            case 1: break;
            case 2: html += "</h2></td></tr>"; break;
            }
        }

        index++;
    }
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);
}

void MainWindow::createAdd(int &a, int &b, int min, int max)
{
    a = rg.generate() % (max - min);
    b = (max - min) - a;
    b = rg.generate() % b;
    a = a + min;
    b = b + min;
}

void MainWindow::createSub(int &a, int &b, int min, int max)
{
    a = rg.generate() % (max - min);
    b = rg.generate() % (max - min);
    a = a + min;
    b = b + min;

    if (a < b) {
        int t = a;
        a = b;
        b = t;
    }
}

void MainWindow::createMul(int &a, int &b, int min, int max)
{
    a = rg.generate() % (max - min);
    b = rg.generate() % (max - min);
    a = a + min;
    b = b + min;
}

void MainWindow::createDiv(int &a, int &b, int min, int max)
{
    a = rg.generate() % (max - min);
    b = rg.generate() % (max - min);
    a = a + min;
    b = b + min;

    int m = rg.generate() % (max - min);
    if (ui->cbMod->isChecked()) {
        a = a * b + m;
    } else {
        a = a * b;
    }
}

