#include "SingleImage.h"

#include <QLabel>

SingleImage::SingleImage()
	: m_imageLabel(new QLabel)
{
	m_imageLabel->setBackgroundRole(QPalette::Base);
	m_imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_imageLabel->setScaledContents(true);
}

QLabel * SingleImage::getImageLabel(void) const 
{ 
	return m_imageLabel; 
}

const QImage & SingleImage::getImage(void) const
{
	return m_image;
}

double SingleImage::getScaleFactor(void) const
{
	return m_scaleFactor;
}

void SingleImage::setImage(const QImage & newImage)
{
	m_image = newImage;
	m_imageLabel->setPixmap(QPixmap::fromImage(m_image));
	m_scaleFactor = 1.0;
}

void SingleImage::scaleImage(double factor)
{
	m_scaleFactor *= factor;
	m_imageLabel->resize(m_scaleFactor * m_imageLabel->pixmap()->size());
}
