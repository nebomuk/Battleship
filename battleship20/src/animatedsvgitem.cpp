#include "animatedsvgitem.h"
#include <QPainter>
#include <QQmlEngine>
#include <QStyleOptionGraphicsItem>

/*static*/ QHash<QString,QSvgRenderer*>& AnimatedSvgItem::getRendererHash()
{
		static QHash<QString,QSvgRenderer*> rendererHash;
		return rendererHash;
}

void AnimatedSvgItem::setPixmapCaching(bool pixmapCaching)
{
	if(pixmapCaching)
	{
		this->setCacheMode(QGraphicsItem::NoCache);
		if(!svgCache_)
			currentPixmap_ = SvgCache::renderToPixmap(renderer_,scaleFactor_);
	}
	else
		this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	pixmapCaching_ = pixmapCaching;
}

void AnimatedSvgItem::setRenderer(QSvgRenderer* renderer)
{
	renderer_ = renderer;
	if(pixmapCaching_)
		currentPixmap_ = SvgCache::renderToPixmap(renderer_,scaleFactor_);
}

void AnimatedSvgItem::setFile(const QString& svgFile)
{
	file_ = svgFile;
	QHash<QString,QSvgRenderer*> & rendererHashRef = getRendererHash();
	if(rendererHashRef.contains(svgFile))
	{
		setRenderer(rendererHashRef.value(svgFile));
	}
	else
	{
		setRenderer(new QSvgRenderer(svgFile));
		rendererHashRef.insert(svgFile,renderer());
	}
	boundingRect_ = QRectF(offset_,renderer()->defaultSize());
}

void AnimatedSvgItem::setSvgCache(SvgCache* cache)
{
	svgCache_ = cache;
	setRenderer(svgRendererAt(currentFrame_));
	currentPixmap_ = pixmapAt(currentFrame_);
	boundingRect_ = QRectF(offset_,renderer()->defaultSize());
}

void AnimatedSvgItem::setOffset(const QPointF &offset)
{
	offset_ = offset;
	boundingRect_.translate(offset_);
}


void AnimatedSvgItem::init()
{
	advancing_ = true;
	loopCount_ = 0; // loop forever
	loopsRemaining_ = loopCount_;
	scaleFactor_ = 1.0;

	frameRateDivisor_ = 1;
	currentFrame_ = 0;
	svgCache_ = NULL;

	renderer_ = NULL;

	pixmapCaching_ = false;

	this->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    QQmlEngine::setObjectOwnership(this,QQmlEngine::CppOwnership);

}

AnimatedSvgItem::AnimatedSvgItem(QGraphicsItem * parent)
	: QObject(), QGraphicsItem(parent)
{
	init();
}


QRectF AnimatedSvgItem::boundingRect() const
{
	//qreal pw = 1.0; // pen width
	//return QRectF(offset_, QGraphicsSvgItem::boundingRect().size()).adjusted(-pw/2, -pw/2, pw/2, pw/2);
	return boundingRect_;
}
QPainterPath AnimatedSvgItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect_);
	return path;
}

void AnimatedSvgItem::advance(int phase)
{
	// quotient is internal frameCount
	if(advancing_ && phase == 1 && svgCache_  && frameCount()/frameRateDivisor() > 1)
	{
		if(frame() >= frameCount()-1)
		{
			setFrame(0);
			if(loopsRemaining_ > 0)
				--loopsRemaining_;
		}
		if(loopsRemaining_ > 0 || loopCount_ == 0)
		{
			setFrame(frame() + 1);
		}
	}
}

void AnimatedSvgItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if(pixmapCaching_)
		painter->drawPixmap(offset_,currentPixmap_);
	else
		renderer()->render(painter,QRectF(offset_,this->renderer()->defaultSize()));
}

void AnimatedSvgItem::setFrame(int value)
{
	int newFrame = value % (svgCache_->size() * frameRateDivisor_); // operator  % has same precedence as *
	if (svgCache_ && currentFrame_ != newFrame)
	{
		currentFrame_ = newFrame;
		//currentFrameInternal_ = div(currentFrame_,frameRateDivisor_).quot;
		if(pixmapCaching_)
		{
			currentPixmap_ = pixmapAt(frame());
			prepareGeometryChange();
		}
		else
		{
			setRenderer(svgRendererAt(frame()));
			update();
		}
	}
}

