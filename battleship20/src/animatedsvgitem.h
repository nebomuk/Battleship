#ifndef ANIMATEDPIXMAPITEM_H
#define ANIMATEDPIXMAPITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QList>
#include <QSvgRenderer>
#include <QHash>
#include <cstdlib> // for quot and div
#include "svgcache.h"
#include "globalvariables.h"


// this class provides frame based animations for svg vector graphics
// to add vector graphics, first add QSvgRenderers do an svgCache object and set it as svgCache
// animations are very cpu intensive, so most items will be single framed.
// For single frame items its recommended to use setFile to specified an svgFile instead of setRenderer

// this class doesn't inherit QGraphicsSvgItem because QGraphicsItem::DeviceCoordinateCache is hardcoded
// into this class (bug fixed in qt > 4.41) but animations only work with QGraphicsItem::NoCache unless you call update() every time
// a frame changes

class AnimatedSvgItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

	Q_PROPERTY(qreal x READ x WRITE setX STORED false)
	Q_PROPERTY(qreal y READ y WRITE setY STORED false)
	Q_PROPERTY(qreal width READ width STORED false)
	Q_PROPERTY(qreal height READ height STORED false)
	Q_PROPERTY(QString file READ file WRITE setFile STORED false)
	Q_PROPERTY(int frame READ frame WRITE setFrame STORED false)
	Q_PROPERTY(qreal zValue READ zValue WRITE setZValue STORED false)
	Q_PROPERTY(bool pixmapCaching READ pixmapCaching WRITE setPixmapCaching STORED true)

public:
	AnimatedSvgItem ( QGraphicsItem * parent = 0 );

	void setX(qreal x){setPos(x,y());}
	void setY(qreal y){setPos(x(),y);}

	// unlike QGraphicsSvgItem, this function doesn't call update()
	void setRenderer(QSvgRenderer* renderer);
	QSvgRenderer* renderer() const { return renderer_; }

	qreal width() const { return boundingRect_.width();}
	qreal height() const { return boundingRect_.height();}

	// if caching is enabled, the pixmaps from svgCache_ are used instead of direct svg rendering
	// svg renderers then cache their contents into pixmaps before rendering, this is false by default
	void setPixmapCaching(bool pixmapCaching);
	bool pixmapCaching() const { return pixmapCaching_; }

	// automatically creates 1 renderer for each filename that is shared among all AnimatedSvgItem objects
	// this is only for single frame objects
	// the internal cache does not use reference counting, all svgs stay inside as long as the application is running
	void setFile(const QString& svgFile);
	const QString & file() const { return file_;} // only available if file has been set with setFile

	// a cache that enables pixmap caching for faster rendering
	void setSvgCache(SvgCache* cache);
	SvgCache * svgCache() const {return svgCache_;}

	// scale factor used for rendering
	void setScaleFactor(qreal scale){ scaleFactor_ = scale;}
	qreal scaleFactor() const { return scaleFactor_;}

	void setLoopCount(int loops) { loopCount_ = loopsRemaining_ =  loops; }
	int loopCount() const { return loopCount_; }

	void setFrame(int frame);
	inline int frame() const { return currentFrame_; }
	inline int frameCount() const { return svgCache_->size() *frameRateDivisor_;}

	inline QSvgRenderer * svgRendererAt(int frame) const
	{return svgCache_ == NULL ? NULL : svgCache_->svgRenderers().at(div(frame,frameRateDivisor_).quot);}

	inline QPixmap pixmapAt(int frame) const
	{return svgCache_ == NULL ? currentPixmap_ : svgCache_->pixmaps().at(div(frame,frameRateDivisor_).quot);}

	int loopsRemaining() const { return loopsRemaining_;}

	void setAdvancing(bool advancing){advancing_ = advancing;}
	bool isAdvancing() const {return advancing_;}

	// show the next frame every frameRateDivisor_ frames
	void setFrameRateDivisor(int value) { frameRateDivisor_ = value; }
	int frameRateDivisor() const { return frameRateDivisor_; }

	// drawing offset. the boundingRect() is translated by this offset too
	void setOffset ( const QPointF & offset );
	void setOffset ( qreal x, qreal y ) { offset_ = QPointF(x,y);}

	// returns the offset to center the item, see the documentation of GraphicsEngine for more details
	static QPointF center(QGraphicsItem * s)
	{return QPointF(-s->boundingRect().width()/2, -s->boundingRect().height()/2);}

	enum { Type = UserType+TYPE_ANIMATEDPIXMAPITEM};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

	QRectF boundingRect() const;
	QPainterPath shape() const; // returns bounding Rect

protected:
	void advance(int phase);

	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:
	void init();
	int frameRateDivisor_;
	int currentFrame_;

	QRectF boundingRect_;

	QPixmap currentPixmap_;

	SvgCache * svgCache_;

	// reference to a hash that stores svgrenderers shared among all objects of AnimatedSvgItem
	static QHash<QString,QSvgRenderer*>& getRendererHash();
	QString file_;

	bool advancing_;
	int loopCount_;
	int loopsRemaining_;
	qreal scaleFactor_;
	QPointF offset_;

	bool pixmapCaching_;

	QSvgRenderer * renderer_;
};
#endif
