#include <QDebug>
#include <QFileDialog>
#include "ImagesList.h"

ImagesList::ImagesList(QObject *parent) :
	QObject(parent),
	m_imageIndex(-1)
{

}

void ImagesList::openImages()
{
	QWidget* parentWidget = dynamic_cast<QWidget*>(parent());

	m_imagesList = QFileDialog::getOpenFileNames(
				  parentWidget,
				  tr("Select files..."),
				  m_imagesDir,
				  "Portable network graphic (*.png);;JPEG (*.jpg)", 0, 0);

	if (m_imagesList.size() != 0)
	{
		m_imagesDir = QFileInfo(m_imagesList.at(0)).absolutePath();
		first();
	}
	else
	{
		// Используем старый список.
	}

	qDebug() << "ImagesList count:" << m_imagesList.size();
}

void ImagesList::first()
{
	qDebug() << "ImagesList first";
	selectImage(0);
}

void ImagesList::previous()
{
	qDebug() << "ImagesList previous";
	selectImage(m_imageIndex - 1);
}

void ImagesList::next()
{
	qDebug() << "ImagesList next";
	selectImage(m_imageIndex + 1);
}
QString ImagesList::getImagesDir() const
{
	return m_imagesDir;
}

void ImagesList::selectImage(int imageIndex)
{
	if (imageIndex >= 0 && imageIndex <= m_imagesList.size() - 1)
	{
		bool isFirst = (imageIndex == 0);
		bool isLast = (imageIndex == m_imagesList.size() - 1);

		m_imageIndex = imageIndex;
		emit selected(m_imagesList.at(m_imageIndex), isFirst, isLast);
	}

	qDebug() << "ImagesList selectImage" << m_imageIndex;
}

QString ImagesList::getImageFileName()
{
	if (m_imageIndex >= 0)
	{
		return m_imagesList.at(m_imageIndex);
	}
	else
	{
		return QString();
	}
}

int ImagesList::count()
{
	return m_imagesList.size();
}

void ImagesList::setImagesDir(const QString& imagesDir)
{
	m_imagesDir = imagesDir;
}
