#ifndef GRAPHICSRIBBONITEM_H
#define GRAPHICSRIBBONITEM_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsRibbonItem : public QGraphicsItem
{
public:
	GraphicsRibbonItem(QSizeF size = QSizeF(50, 50));

	void setSize(QSizeF size);
	void setSize(qreal width, qreal height);

	// QGraphicsItem interface
public:
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

	qreal width() const;
	qreal height() const;

	void setAspect(qreal aspect);
	void setAspect(int w, int h, bool potrait = true);
	void setFixedAspect(bool fixedAspect = true);

private:
	enum HoveredElement {heNone, heInner, heLeft, heTop, heRight, heBottom};

	qreal m_width;
	qreal m_height;
	qreal m_grabWidth;
	qreal m_aspect;
	bool m_fixedAspect;
	HoveredElement m_hoveredElement;
	bool m_mouseDown;

	QPointF m_mouseDownPos;
	QRectF m_mouseDownRect;

	void applyAspectToHeight();
	void applyAspectToWidth();

	void updateSceneView();

	// QGraphicsItem interface
protected:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

#endif // GRAPHICSRIBBONITEM_H
