/****************************************************************************
**
** Classe CommandCalculateSize
** Author: p482457
** Date: 17-nov-2019
**
****************************************************************************/

#include "CommandCalculateSize.h"
#include "TeamImageSizeCalculator.h"
#include "AbsTeamComponent.h"
#include "TeamMember.h" //jai ajoute ces trois includes
#include "TeamMemberRole.h" //
#include "Team.h"//

CommandCalculateSize::CommandCalculateSize(AbsTeamComponent & component)
	: m_target(component), m_computedSize(0)
{
}

void CommandCalculateSize::cancel()
{
	// Cette commande ne peut pas etre annulee
}

void CommandCalculateSize::execute()
{
	// Construire un visiteur de calcul de taille et l'appliquer a la composante
	TeamImageSizeCalculator calc;
	m_target.accept(calc);
	m_computedSize = calc.getTotalSize();
}

void CommandCalculateSize::clearSize(void)
{
	// Reinitialiser la taille totale
	m_computedSize = 0;
}

size_t CommandCalculateSize::getSize(void) const
{
	// Retourner la taille totale calculee
	return m_computedSize;
}
