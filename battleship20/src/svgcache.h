#ifndef SVGCACHE_H
#define SVGCACHE_H

#include <QList>
#include <QSvgRenderer>


// This class caches the svgRenderers contents into pixmaps
class SvgCache
{
public:
	SvgCache();
	void setScaleFactor(qreal scale){ scaleFactor_ = scale;}
	qreal scaleFactor() const { return scaleFactor_;}
	void addSvgRenderer(QSvgRenderer * renderer){svgRenderers_+=renderer; update();}
	void addSvgRenderers(const QList<QSvgRenderer*> & renderers){svgRenderers_+=renderers;update();}

	static QPixmap renderToPixmap(QSvgRenderer * renderer, qreal scaleFactor = 1.0);

	const QList<QSvgRenderer*> & svgRenderers() const { return svgRenderers_;}
	const QList<QPixmap> & pixmaps() const { return pixmaps_;}
	int size() const { return svgRenderers_.size();}

	void update();

private:
	QList<QSvgRenderer*>  svgRenderers_;
	QList<QPixmap> pixmaps_;
	qreal scaleFactor_;

};

#endif // SvgCache_H
