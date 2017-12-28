#ifndef COUNTERITEM_H
#define COUNTERITEM_H


#include <QGraphicsItem>
#include <QSvgRenderer>
/**
  Counter shows counter with digits taken from input svg file.
  Feel free to add your own themes. To do that please follow id names int svg file.
*/

class CounterItem : public QGraphicsItem
{
	  public:
  CounterItem (QGraphicsItem * parent = NULL);

  int digits() const;
  int value() const;
  const QString digitsFile() const;
  public slots:

  void setValue(int);
  void setDigits(int);

  void resize(const QSizeF& size){m_boundingRect = QRectF(QPointF(0.0,0.0),size);}
  void resize ( qreal w, qreal h ){resize(QSizeF(w,h));}

  QRectF boundingRect()  const { return m_boundingRect;}

  qreal height() const { return m_boundingRect.height();}
  qreal width() const { return m_boundingRect.width();}

  /** Sets filepath to svg file with digits */
  void setDigitsFile(const QString & );

  protected:
   void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
   void init();

   int m_digits;
   int m_value;
   QString m_digitsFile;
   QSvgRenderer * m_svg;
   QRectF m_boundingRect;
};

#endif // COUNTERITEM_H
