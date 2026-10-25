#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>


MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->contactbook->setColumnCount(4);
    QStringList colTotal;
    colTotal<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->contactbook->setHorizontalHeaderLabels(colTotal);

}

MyWidget::~MyWidget()
{
    delete ui;
}


void MyWidget::on_insertButton_clicked()
{
    QTableWidgetItem *col1, *col2, *col3, *col4;
    col1=new QTableWidgetItem(ui->lineEdit_1->text());
    col2=new QTableWidgetItem(ui->lineEdit_2->text());
    col3=new QTableWidgetItem(ui->lineEdit_3->text());
    col4=new QTableWidgetItem(ui->lineEdit_4->text());
    int row = ui->contactbook->rowCount();
    qDebug()<<row;
    ui->contactbook->insertRow(row);
    ui->contactbook->setItem(row,0,col1);
    ui->contactbook->setItem(row,1,col2);
    ui->contactbook->setItem(row,2,col3);
    ui->contactbook->setItem(row,3,col4);
}
void CbWrite(QString fileName, QString str)
{
    QFile outFile(fileName);
    if(!outFile.open(QFile::WriteOnly |QFile::Text))
    {
        qDebug()<<"Open File failed!";
        return;
    }
    QTextStream out(&outFile);
    out.setEncoding(QStringConverter::System);
    out<<str;
    outFile.flush();
    outFile.close();
}

void MyWidget::on_exportButton_2_clicked()
{
    QString fileName;
    QString saveStr;
    fileName = QFileDialog::getSaveFileName(this,"匯出通訊錄",".",
                                             "csv(*.csv);;text(*.txt)");
    for(int i=0; i<ui->contactbook->rowCount();i++)
    {
        for(int j=0; j<ui->contactbook->columnCount();j++)
        {
            saveStr+=ui->contactbook->item(i,j)->text()+(j<ui->contactbook->columnCount() ? ",":"");
        }
        saveStr +="\n";
    }
    saveStr+="\n";
    CbWrite(fileName,saveStr);
}


void MyWidget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "匯入通訊錄", ".", "csv(*.csv);;text(*.txt)");
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Open File for reading failed!";
        return;
    }
    QTextStream in(&file);
    in.setEncoding(QStringConverter::System);
    ui->contactbook->setRowCount(0);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;
        QStringList fields = line.split(',');
        int row = ui->contactbook->rowCount();
        ui->contactbook->insertRow(row);
        for (int i=0; i<fields.size() && i<ui->contactbook->columnCount();i++)
        {
            ui->contactbook->setItem(row, i, new QTableWidgetItem(fields[i]));
        }
    }
    file.close();
}

