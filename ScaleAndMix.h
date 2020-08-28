#ifndef SCALEANDMIX_H
#define SCALEANDMIX_H

#include <QObject>
#include <opencv2/opencv.hpp>
using namespace cv;

class ScaleAndMix : public QObject
{
    Q_OBJECT
public:
    explicit ScaleAndMix(QObject *parent = nullptr);

public slots:
    void toBigPic(QString originFile, QString targetFile);

signals:

};

#endif // SCALEANDMIX_H
