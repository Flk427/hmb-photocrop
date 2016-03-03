#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QLabel>
#include <QImage>

class ImagePreview : public QLabel
{
	Q_OBJECT
public:
	ImagePreview(QWidget *parent=0);

public slots:
	void updatePreview(const QImage& source, const QRectF& area);
};

#endif // IMAGEPREVIEW_H
