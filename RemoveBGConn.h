#ifndef REMOVEBGCONN_H
#define REMOVEBGCONN_H

#include <QObject>
#include <QFile>
#include <QThread>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class RemoveBGConn : public QObject
{
    Q_OBJECT
public:
    RemoveBGConn();

signals:
    void receivedData(QByteArray d, QString filePath);
    void connectError(QString e);
    void connectInfo(QString info);

public slots:
    void postData(QString filepath, QString token, bool start);
    bool postDataReal();
    void solveReply();
    void clearData();

private:
    QNetworkAccessManager *accessManager = new QNetworkAccessManager();
    QNetworkReply* reply;
    QFile *file;
    std::vector<QString> filepath;
    std::vector<QString> token;
    int i = 0;
};

#endif // REMOVEBGCONN_H
