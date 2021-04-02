/****************************************************************************
**
** Classe SingleImage
** Author: p482457
** Date: 25-oct-2019
**
****************************************************************************/

#ifndef _SingleImage_Header_
#define _SingleImage_Header_

#include "AbsImage.h"

#include <QImage>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class SingleImage : public AbsImage
{
public:
	SingleImage();

	virtual class QLabel* getImageLabel(void) const;
	virtual const QImage& getImage(void) const;
	virtual double getScaleFactor(void) const;
	virtual void setImage(const QImage &newImage);
	virtual void scaleImage(double factor);

private:
	QImage m_image;
	QLabel *m_imageLabel;
	double m_scaleFactor;

};

#endif