#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H

#include <QObject>
#include <QPainter>
#include <QPicture>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QWidget>
#include <QRect>

class ImageContainer : public QObject
{
	Q_OBJECT
public:
	ImageContainer(QObject* parent = 0);

	QImage m_image;
	QPainter m_painter;

	bool loadImage(QString fileName);
	void cropImage(const QRect& rect);
	void saveImage(QString fileName);
	bool isImageChanged() const;

signals:
	void imageLoaded();
	void imageLoadError();
	void imageChanged();

public slots:
	void restoreImage();
	void rotateLeft();
	void rotateRight();
	void mirrorVertical();
	void mirrorHorisontal();

private:
	QImage m_originalImage;
	bool m_imageChanged;
};

#endif // IMAGECONTAINER_H
