#include "imageutil.h"

ImageUtil::ImageUtil()
{

}

QImage ImageUtil::convertToGray(QImage image) {
    return image.convertToFormat(QImage::Format_Grayscale8);
}
