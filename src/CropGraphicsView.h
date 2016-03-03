#ifndef CROPGRAPHICSVIEW_H
#define CROPGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

/*!
	\brief The CropGraphicsView class

	Меняем отображение курсора мыши QGraphicsView при включенном режиме
	dragMode на обычные курсоры.
*/

class CropGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	CropGraphicsView(QWidget* parent);

	void updateCropSelector();

signals:
	void cropSelectorMoved();

	// QWidget interface
protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void enterEvent(QEvent* event);
};

#endif // CROPGRAPHICSVIEW_H
