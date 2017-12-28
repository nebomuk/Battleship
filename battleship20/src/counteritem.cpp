#include <QPainter>
#include <QPen>
#include <QSvgRenderer>
#include "counteritem.h"

CounterItem::CounterItem(QGraphicsItem * parent): QGraphicsItem(parent)
{
  m_boundingRect = QRectF(0.0,0.0,128.0,64.0);
  init();
}

int CounterItem::digits() const
{
  return m_digits;
}

int CounterItem::value() const
{
  return m_value;
}

const QString CounterItem::digitsFile() const
{
  return m_digitsFile;
}

/*--------------------------------------------------------------------------------------------
				  P U B L I C   S L O T S
--------------------------------------------------------------------------------------------*/
void CounterItem::setValue(int i)
{
   m_value = i;
   update();
}

void CounterItem::setDigits(int i)
{
  m_digits = i;
  update();
}

void CounterItem::setDigitsFile(const QString & i )
{
   m_digitsFile = i;
   if (m_svg)
	 delete m_svg;
   m_svg = new QSvgRenderer;
   if (!m_svg->load(m_digitsFile))
   {
	 qDebug("CounterItem::setDigitsFile can't load file %s",qPrintable(m_digitsFile));
	 m_svg->load(m_digitsFile);

   }
   //else
	//qDebug("CounterItem::setDigitsFile %s loaded",qPrintable(m_digitsFile));

  update();
}

/*--------------------------------------------------------------------------------------------
				  P R O T E C T E D
--------------------------------------------------------------------------------------------*/
#define X_OFFSET 10
#define Y_OFFSET 10

void CounterItem::paint ( QPainter * p, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
  p->setRenderHint(QPainter::Antialiasing,true);

  p->save();
  int side = this->height();
  p->scale(side/100.0,side/100.0);

  double width = 100.0 * this->width()/this->height(), height = 100.0;


  //p->setPen(Qt::NoPen);
  QPen pen = p->pen();

  pen.setColor(QColor(32,32,32));
  pen.setWidthF(6.0);
  p->setPen(pen);
  p->setBrush(Qt::black);
  p->drawRoundRect(3,3,width-6,height-6,7,(7*width)/(double)height);


  int w = (width-2*X_OFFSET)/m_digits;
  int x = (m_digits-1) * w;
  int h = height-2*Y_OFFSET;
  int r, c = m_value;

	for (int i=0 ; i<m_digits ; i++)
	{
	  r = c % 10;
	  c = c/10;
	  QRect rect(x+X_OFFSET,Y_OFFSET,w,h);
	  m_svg->render(p,QString("d%1").arg(r),rect);
	  x -= w;
	}
  p->restore();
}


void CounterItem::init()
{
   m_digits = 4;
   m_value = 0;
   m_svg = NULL;
}

