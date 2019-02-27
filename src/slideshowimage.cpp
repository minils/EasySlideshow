#include "slideshowimage.h"

SlideshowImage::SlideshowImage(QString path) {
    _path = path;
    _orientation = 0;
}

const QString SlideshowImage::path() const{
    return _path;
}

const QString SlideshowImage::filename() const {
    return QFileInfo(_path).fileName();
}

const QString SlideshowImage::directory() const {
    return QDir::toNativeSeparators(QFileInfo(_path).absolutePath());
}

long long SlideshowImage::filesize() const {
    return QFileInfo(_path).size();
}

const QSize SlideshowImage::resolution() const {
    QImageReader reader(_path);
    return reader.size();
}

 int SlideshowImage::orientation() const
{
    return _orientation;
}

 QDateTime SlideshowImage::creationTime() const {
     return QFileInfo(_path).created();
 }

void SlideshowImage::rotateLeft() {
    rotate(-90);
}

void SlideshowImage::rotateRight() {
    rotate(90);
}

void SlideshowImage::rotate(int angle) {
    if (SettingsManager::readSetting(SETTING_SAVE_ORIENTATION).toBool()) {
        // Problem with this method is that the quality is decreased
//        QImage image(_path);
//        QImageWriter writer(_path);
//        int abs_angle = angle % 360;
//        QFlags<QImageIOHandler::Transformation> transformation;
//        switch (abs_angle) {
//        case 90:
//            transformation = QImageIOHandler::TransformationRotate90;
//            break;
//        case 180:
//            transformation = QImageIOHandler::TransformationRotate180;
//            break;
//        case -90:
//        case 270:
//            transformation = QImageIOHandler::TransformationRotate270;
//            break;
//        default:
//            transformation = QImageIOHandler::TransformationNone;
//            break;
//        }
//        writer.setTransformation(transformation);
//        writer.write(image);

        // Try to correct the orientation by using the exif data
        exif_transform(_path, abs(angle%360));
    } else {
        _orientation = (_orientation + angle) % 360;
    }
}

void SlideshowImage::setPath(QString path) {
    _path = path;
}

void SlideshowImage::exif_transform(const QString path, int angle) const {
    if (angle != 90 && angle != 180 && angle != 270)
        return;

    int current_orientation = 0;

    // Get current orientation from exif
    ExifLoader* l = exif_loader_new();
    if (l) {
        ExifData* ed;
        exif_loader_write_file(l, path.toStdString().c_str());
        ed = exif_loader_get_data(l);
        exif_loader_unref(l);
        if (ed) {
            exif_data_foreach_content(ed, [](ExifContent* ec, void* user_data) -> void {
                ExifEntry* ee;
                ee = exif_content_get_entry(ec, EXIF_TAG_ORIENTATION);
                if (ee) {
                    int *orient = static_cast<int*>(user_data);
                    *orient = ee->data[0];
                }
                //exif_entry_unref(ee);
            }, static_cast<void*>(&current_orientation));
        }
        exif_data_unref(ed);
    }
    qWarning() << "Orientation:" << current_orientation;

    // Assume that image was not flipped
    if (current_orientation != 1 && current_orientation != 3 && current_orientation != 6 && current_orientation != 8) {
        qWarning() << "Will not rotate mirrored or flipped image.";
        return;
    }

    int current_angle = 0;
    switch (current_orientation) {
    case 3:
        current_angle = 180;
        break;
    case 6:
        current_angle = 90;
        break;
    case 8:
        current_angle = 270;
        break;
    default:
        break;
    }

    int new_angle = current_angle + angle;
    new_angle %= 360;

    int new_orientation = 1;
    switch (new_angle) {
    case 0:
        new_orientation = 1;
        break;
    case 180:
        new_orientation = 3;
        break;
    case 90:
        new_orientation = 6;
        break;
    case 270:
        new_orientation = 8;
        break;
    default:
        break;
    }

    qWarning() << "New Orientation:" << new_orientation;

    // 1: no transform
    // 3: 180
    // 6: 90
    // 8: 270
}
