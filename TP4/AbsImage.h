/****************************************************************************
**
** Classe AbsImage
** Author: p482457
** Date: 25-oct-2019
**
****************************************************************************/

#ifndef _AbsImage_Header_
#define _AbsImage_Header_

class AbsImage
{
public:
	AbsImage() {};

	virtual void setImage(const class QImage &newImage) = 0;
	virtual void scaleImage(double factor) = 0;

};

#endif
