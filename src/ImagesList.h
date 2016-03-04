#ifndef IMAGESLIST_H
#define IMAGESLIST_H

#include <QObject>
#include <QStringList>

class ImagesList : public QObject
{
	Q_OBJECT
public:
	explicit ImagesList(QObject *parent = 0);

	void setImagesDir(const QString& imagesDir);
	void first();
	QString getImageFileName();
	int count();

signals:
	void selected(QString fileName, bool isFirst, bool isLast);

public slots:
	void openImages();
	void previous();
	void next();

private:
	QString m_imagesDir;
	QStringList m_imagesList;
	int m_imageIndex;

	void selectImage(int imageIndex);
};

#endif // IMAGESLIST_H
