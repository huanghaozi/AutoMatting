#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    rmbg = new RemoveBGConn();
    sm = new ScaleAndMix();

    connect(this, &Widget::addTask, rmbg, &RemoveBGConn::postData);
    connect(rmbg, &RemoveBGConn::connectInfo, this, &Widget::solveMessage);
    connect(rmbg, &RemoveBGConn::connectError, this, &Widget::solveMessage);
    connect(rmbg, &RemoveBGConn::receivedData, this, &Widget::solveReply);
    connect(this, &Widget::clearTasks, rmbg, &RemoveBGConn::clearData);
    connect(this, &Widget::smallToBig, sm, &ScaleAndMix::toBigPic);

    rmbg->moveToThread(thread);
    sm->moveToThread(thread);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_inputBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("Images (*.png *.jpg *.JPG *.PNG)"));
    ui->inputEdit->setText(fileName);
}

void Widget::on_outputBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("Images (*.png *.jpg *.JPG *.PNG)"));
    ui->inputEdit->setText(fileName);
}

void Widget::on_pushButton_4_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("打开输入文件夹"), "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->inputEdit->setText(dirName);
}

void Widget::on_outputBtn_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("打开输出文件夹"), "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->outputEdit->setText(dirName);
}

void Widget::on_matBtn_clicked()
{
    thread->start();
    QString token = ui->lineEdit->text();
    QFileInfo fileinfo(ui->inputEdit->text());
    if(fileinfo.isFile())
    {
        emit clearTasks();
        emit addTask(fileinfo.filePath(), token, true);
    }
    if(fileinfo.isDir())
    {
        qDebug() << "isDir";
        QDir dir(fileinfo.filePath());
        QFileInfoList subFileList = dir.entryInfoList(QDir::Files | QDir::CaseSensitive);
        QStringList filePaths;
        for (int i = 0;i < subFileList.size(); i++)
        {
            QString suffix = subFileList[i].suffix();
            if(suffix.compare("png")==0 || suffix.compare("jpg")==0 || suffix.compare("JPG")==0 || suffix.compare("PNG")==0)
            {
                filePaths.append(subFileList[i].filePath());
            }
        }
        emit clearTasks();
        for(int i=0;i<filePaths.size();i++)
        {
            if(i == filePaths.size()-1)
                emit addTask(filePaths[i], token, true);
            else
                emit addTask(filePaths[i], token, false);
        }
    }
}

void Widget::on_stopBtn_clicked()
{
    emit clearTasks();
    thread->quit();
    ui->plainTextEdit->appendPlainText("已中止任务！");
}

void Widget::on_inputEdit_textChanged(const QString &arg1)
{
    QFileInfo fileinfo(arg1);
    if(fileinfo.isFile())
    {
        isFileNotDir = true;
        ui->outputEdit->setText(QString("%1-no-bg-small.png").arg(arg1));
    }
    if(fileinfo.isDir())
    {
        isFileNotDir = false;
        ui->outputEdit->setText(QString("%1-no-bg-small").arg(arg1));
    }
}

void Widget::solveReply(QByteArray picRaw, QString filePath)
{
    QString outputPath = ui->outputEdit->text();
    if(isFileNotDir)
    {
        QFileInfo fileinfo(outputPath);
        if(!fileinfo.dir().exists())
            if(!fileinfo.dir().mkdir(fileinfo.dir().absolutePath()))
            {
                ui->plainTextEdit->appendPlainText(QString("创建路径失败：%1").arg(fileinfo.dir().absolutePath()));
                return;
            }
    }
    else
    {
        QDir dir(outputPath);
        if(!dir.exists())
            if(!dir.mkdir(dir.absolutePath()))
            {
                ui->plainTextEdit->appendPlainText(QString("创建路径失败：%1").arg(dir.absolutePath()));
                return;
            }
        QFileInfo fileinfo(filePath);
        outputPath = QString("%1/%2-no-bg.png").arg(outputPath).arg(fileinfo.fileName());
    }
    QFile file(outputPath);
    if(!file.open(QIODevice::WriteOnly))
    {
        ui->plainTextEdit->appendPlainText(QString("创建文件失败：%1").arg(filePath));
        return;
    }
    file.write(picRaw);
    file.close();
    emit smallToBig(filePath, outputPath);
    ui->plainTextEdit->appendPlainText("保存成功！");
}

void Widget::solveMessage(QString msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

