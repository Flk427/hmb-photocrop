#include "GraphicsRibbonItem.h"
#include <QApplication>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QGraphicsScene>
#include "CropGraphicsView.h"

GraphicsRibbonItem::GraphicsRibbonItem(QSizeF size) :
	m_grabWidth(5.0),
	QGraphicsItem(),
	m_aspect(1.0),
	m_fixedAspect(false),
	m_hoveredElement(heNone),
	m_mouseDown(false)
{
	m_width = size.width();
	m_height = size.height();

	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsSelectable);

	// setFlag(QGraphicsItem::ItemIsMovable);
	applyAspectToHeight();
}

void GraphicsRibbonItem::setSize(QSizeF size)
{
	m_width = size.width();
	m_height = size.height();
	m_fixedAspect = false;
	updateSceneView();
}

void GraphicsRibbonItem::setSize(qreal width, qreal height)
{
	m_width = width;
	m_height = height;
	m_fixedAspect = false;
	updateSceneView();
}

QRectF GraphicsRibbonItem::boundingRect() const
{
	return QRectF(0, 0, m_width, m_height);
}

void GraphicsRibbonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();
	painter->setPen(Qt::DashLine);
	painter->drawRect(boundingRect());
	painter->restore();
}

qreal GraphicsRibbonItem::width() const
{
	return m_width;
}

qreal GraphicsRibbonItem::height() const
{
	return m_height;
}

void GraphicsRibbonItem::setAspect(qreal aspect)
{
	m_aspect = aspect;
	applyAspectToHeight();
	updateSceneView();
}

void GraphicsRibbonItem::setAspect(int w, int h, bool potrait)
{
	qreal aspect;

	if (potrait)
	{
		// При портретном режиме "поворачиваем" аспект.
		aspect = ((qreal) h) / w;
	}
	else
	{
		aspect = ((qreal) w) / h;
	}

	setAspect(aspect);
}

void GraphicsRibbonItem::setFixedAspect(bool fixedAspect)
{
	m_fixedAspect = fixedAspect;
}

void GraphicsRibbonItem::applyAspectToHeight()
{
	qreal oldHeight = m_height;

	m_height = m_width / m_aspect;

	if (abs(m_height - oldHeight) > 0.001)
	{
		setY(y() - (m_height - oldHeight) / 2.0);
	}
}

void GraphicsRibbonItem::applyAspectToWidth()
{
	qreal oldWidth = m_width;

	m_width = m_height * m_aspect;

	if (abs(m_width - oldWidth) > 0.001)
	{
		setX(x() - (m_width - oldWidth) / 2.0);
	}
}

void GraphicsRibbonItem::updateSceneView()
{
	qDebug() << "Scene views" << scene()->views().size();

	if (scene()->views().size() != 0)
	{
		dynamic_cast<CropGraphicsView*>(scene()->views().at(0))->updateCropSelector();
	}
}

void GraphicsRibbonItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	if (scene()->views().size() != 0)
	{
		m_grabWidth = 5.0 / scene()->views().at(0)->transform().m11();
	}
	else
	{
		m_grabWidth = 5.0;
	}

	qDebug() << "grap width" << m_grabWidth;

	QApplication::setOverrideCursor(Qt::SizeAllCursor);
	QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsRibbonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	m_hoveredElement = heNone;
	QApplication::restoreOverrideCursor();
	QGraphicsItem::hoverLeaveEvent(event);
}

void GraphicsRibbonItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
//	qDebug() << "hoverMoveEvent";

//	if (m_mouseDown)
//	{
//		QGraphicsItem::hoverMoveEvent(event);
//		return;
//	}

	if (event->pos().x() < m_grabWidth && event->pos().y() < m_grabWidth)
	{
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
	}
	else if (event->pos().x() > m_width - m_grabWidth && event->pos().y() > m_height - m_grabWidth)
	{
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
	}
	else if (event->pos().x() > m_width - m_grabWidth && event->pos().y() < m_grabWidth)
	{
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	}
	else if (event->pos().x() < m_grabWidth && event->pos().y() > m_height - m_grabWidth)
	{
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	}
	else if (event->pos().x() < m_grabWidth)
	{
		m_hoveredElement = heLeft;
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeHorCursor);
	}
	else if (event->pos().x() > m_width - m_grabWidth)
	{
		m_hoveredElement = heRight;
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeHorCursor);
	}
	else if (event->pos().y() < m_grabWidth)
	{
		m_hoveredElement = heTop;
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeVerCursor);
	}
	else if (event->pos().y() > m_height - m_grabWidth)
	{
		m_hoveredElement = heBottom;
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeVerCursor);
	}
	else
	{
		m_hoveredElement = heInner;
		QApplication::restoreOverrideCursor();
		QApplication::setOverrideCursor(Qt::SizeAllCursor);
	}

	QGraphicsItem::hoverMoveEvent(event);
}

void GraphicsRibbonItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	qDebug() << "mousePressEvent" << "Pos" << QCursor::pos() << "Left" << x();

	m_mouseDown = true;
	m_mouseDownPos = QCursor::pos();
	m_mouseDownRect = boundingRect();

	m_mouseDownRect.setLeft(m_mouseDownRect.left() + x());
	m_mouseDownRect.setTop(m_mouseDownRect.top() + y());
	m_mouseDownRect.setRight(m_mouseDownRect.right() + x());
	m_mouseDownRect.setBottom(m_mouseDownRect.bottom() + y());

	QApplication::setOverrideCursor(Qt::CrossCursor);
	QGraphicsItem::mousePressEvent(event);
}

void GraphicsRibbonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	qDebug() << "mouseReleaseEvent";

	m_mouseDown = false;
	QApplication::restoreOverrideCursor();
	QGraphicsItem::mouseReleaseEvent(event);
}


void GraphicsRibbonItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	qDebug() << "mouseMoveEvent" <<"From:" << m_mouseDownPos << "To:" << QCursor::pos();

	QPointF nowPos = QCursor::pos();

	if (m_mouseDown)
	{
		qreal viewScale = 1.0;

		if (scene()->views().size() != 0)
		{
			viewScale = scene()->views().at(0)->transform().m11();
			qDebug() << "Scale" << viewScale;
		}

		qreal deltaX = (nowPos.x() - m_mouseDownPos.x()) / viewScale;
		qreal deltaY = (nowPos.y() - m_mouseDownPos.y()) / viewScale;

		if (m_hoveredElement == heRight)
		{
			qDebug() << "X from" << m_mouseDownPos.x() << "To" << nowPos.x() << "dx" << deltaX;
			m_width = m_mouseDownRect.width() + deltaX;

			if (m_fixedAspect)
			{
				applyAspectToHeight();
			}
		}

		if (m_hoveredElement == heBottom)
		{
			m_height = m_mouseDownRect.height() + deltaY;

			if (m_fixedAspect)
			{
				applyAspectToWidth();
			}
		}

		if (m_hoveredElement == heLeft)
		{
			qDebug() << "X from" << m_mouseDownPos.x() << "To" << nowPos.x() << "dx" << deltaX;

			setX(m_mouseDownRect.left() + deltaX);
			m_width = m_mouseDownRect.width() - deltaX;

			if (m_fixedAspect)
			{
				applyAspectToHeight();
			}
		}

		if (m_hoveredElement == heTop)
		{
			setY(m_mouseDownRect.top() + deltaY);
			m_height = m_mouseDownRect.height() - deltaY;

			if (m_fixedAspect)
			{
				applyAspectToWidth();
			}
		}

		if (m_hoveredElement == heInner)
		{
			setX(m_mouseDownRect.left() + deltaX);
			setY(m_mouseDownRect.top() + deltaY);
		}

		updateSceneView();
		scene()->update();
		update();
	}

	QGraphicsItem::mouseMoveEvent(event);
}
