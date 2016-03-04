#include <QPixmap>
#include "ImagePreview.h"

ImagePreview::ImagePreview(QWidget *parent) :
	QLabel(parent)
{

}

void ImagePreview::updatePreview(const QImage& source, const QRectF& area)
{
	QImage preview = source.copy(area.toRect());
	this->setPixmap(QPixmap::fromImage(preview).scaled(width(), height(), Qt::KeepAspectRatio));
}



void ImagePreview::resizeEvent(QResizeEvent* event)
{
	emit resized();
	QLabel::resizeEvent(event);
}
