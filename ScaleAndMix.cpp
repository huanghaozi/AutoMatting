#include "ScaleAndMix.h"

ScaleAndMix::ScaleAndMix(QObject *parent) : QObject(parent)
{

}

void ScaleAndMix::toBigPic(QString originFile, QString targetFile)
{
    Mat originImg = imread(originFile.toLocal8Bit().toStdString());
    Mat mattedSmall = imread(targetFile.toLocal8Bit().toStdString(), IMREAD_UNCHANGED);
    Mat mattedBig;
    Mat background(originImg.size(), originImg.type(), Scalar(255, 255, 255));
    Mat rgbaMatted[4], outputMat[4];
    if(originImg.data != NULL && mattedSmall.data != NULL)
    {
        resize(mattedSmall, mattedBig, Size(originImg.cols, originImg.rows), 0, 0, INTER_LANCZOS4);
        split(mattedBig, rgbaMatted);
        split(originImg, outputMat);
        outputMat[3] = rgbaMatted[3];
        merge(outputMat, 4, mattedBig);
        imwrite(targetFile.toLocal8Bit().toStdString(), mattedBig);
    }

}
