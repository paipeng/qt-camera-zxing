#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <QImage>

class ImageUtil
{
public:
    ImageUtil();

public:
    QImage convertToGray(QImage image);
};

#endif // IMAGEUTIL_H
