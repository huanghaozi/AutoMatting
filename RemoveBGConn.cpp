#include "RemoveBGConn.h"

RemoveBGConn::RemoveBGConn()
{
    //qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
}

void RemoveBGConn::postData(QString filepath, QString token, bool start)
{
    this->filepath.push_back(filepath);
    this->token.push_back(token);
    if(start)
    {
        i = 0;
        postDataReal();
    }
}

bool RemoveBGConn::postDataReal()
{
    emit connectInfo("");
    emit connectInfo(filepath[i]);
    file = new QFile(filepath[i]);
    file->open(QIODevice::ReadOnly);

    QByteArray ba;
    ba.append("size=auto&");
    ba.append("image_file_b64=");
    ba.append(file->readAll().toBase64(QByteArray::Base64UrlEncoding));

    QUrl url("https://api.remove.bg/v1.0/removebg");
    QNetworkRequest request(url);
    request.setRawHeader("X-Api-Key", token[i].toUtf8());
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

    reply = accessManager->post(request, ba);
    emit connectInfo("正在发送...");

    QObject::connect(reply, &QNetworkReply::finished, this, &RemoveBGConn::solveReply);
    return true;
}

void RemoveBGConn::solveReply()
{
    emit connectInfo("发送完成！正在接收...");
    if(reply->error() == QNetworkReply::NoError)
    {
        emit connectInfo("接收完成！");
        emit receivedData(reply->readAll(), filepath[i]);
    }
    else
    {
        emit connectError(reply->errorString());
    }
    i++;
    if(i<int(filepath.size()))
        postDataReal();
    if(i==int(filepath.size()))
        emit connectInfo("\n全部完成！");
}

void RemoveBGConn::clearData()
{
    filepath.clear();
    token.clear();
    i = 0;
}
