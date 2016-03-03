#include "CropGraphicsView.h"

CropGraphicsView::CropGraphicsView(QWidget* parent) : QGraphicsView(parent)
{

}

void CropGraphicsView::updateCropSelector()
{
	emit cropSelectorMoved();
}

void CropGraphicsView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	viewport()->setCursor(Qt::CrossCursor);
}

void CropGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
	viewport()->setCursor(Qt::ArrowCursor);
}

void CropGraphicsView::enterEvent(QEvent* event)
{
	QGraphicsView::enterEvent(event);
	viewport()->setCursor(Qt::ArrowCursor);
}
