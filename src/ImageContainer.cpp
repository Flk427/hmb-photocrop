#include "ImageContainer.h"

ImageContainer::ImageContainer(QObject* parent) :
	QObject(parent),
	m_imageChanged(false)
{

}

bool ImageContainer::loadImage(QString fileName)
{
	bool rc = m_image.load(fileName);

	if (rc)
	{
		m_originalImage = m_image;
		m_imageChanged = false;
		emit imageLoaded();
		emit imageChanged();
	}
	else
	{
		emit imageLoadError();
	}

	return rc;
}

void ImageContainer::restoreImage()
{
	m_image = m_originalImage;
	m_imageChanged = false;
	emit imageLoaded();
	emit imageChanged();
}

void ImageContainer::cropImage(const QRect& rect)
{
	m_image = m_image.copy(rect);
	m_imageChanged = true;
	emit imageChanged();
}

void ImageContainer::saveImage(QString fileName)
{
	m_image.save(fileName);
}

void ImageContainer::rotateLeft()
{
	QTransform transform;
	transform.rotate(-90);
	m_image = m_image.transformed(transform);
	m_imageChanged = true;
	emit imageChanged();
}

void ImageContainer::rotateRight()
{
	QTransform transform;
	transform.rotate(90);
	m_image = m_image.transformed(transform);
	m_imageChanged = true;
	emit imageChanged();
}

void ImageContainer::mirrorVertical()
{
	m_image = m_image.mirrored(false, true);
	m_imageChanged = true;
	emit imageChanged();
}

void ImageContainer::mirrorHorisontal()
{
	m_image = m_image.mirrored(true, false);
	m_imageChanged = true;
	emit imageChanged();
}

bool ImageContainer::isImageChanged() const
{
	return m_imageChanged;
}
