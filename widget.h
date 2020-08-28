#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include <QFileDialog>
#include <QThread>
#include "RemoveBGConn.h"
#include "ScaleAndMix.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool isFileNotDir;
    void postData(QString filepath, QString token);

private slots:
    void on_inputBtn_clicked();

    void on_outputBtn_clicked();

    void on_pushButton_4_clicked();

    void on_outputBtn_2_clicked();

    void on_matBtn_clicked();

    void on_stopBtn_clicked();

    void on_inputEdit_textChanged(const QString &arg1);

    void solveReply(QByteArray picRaw, QString filePath);

    void solveMessage(QString msg);

private:
    Ui::Widget *ui;
    RemoveBGConn *rmbg;
    ScaleAndMix *sm;
    QThread *thread = new QThread();

signals:
    void addTask(QString fileName, QString token, bool startFlag);
    void clearTasks();
    void smallToBig(QString origin, QString target);
};
#endif // WIDGET_H
