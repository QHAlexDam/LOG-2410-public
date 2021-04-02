/****************************************************************************
**
** Classe MemberTextFindReplace
** Author: p482457
** Date: 25-oct-2019
**
****************************************************************************/

#include "MemberTextFindReplace.h"
#include "TeamMember.h"
#include "TeamMemberRole.h"
#include "Team.h"
#include <String>

MemberTextFindReplace::MemberTextFindReplace(const std::string * find_s, const std::string * replace_s)
{
	setStrings(find_s, replace_s);
}

void MemberTextFindReplace::processTeamMember(TeamMember & member)
{
	// Verifier si le nom du membre contient la chaine cherchee. Si oui
	//      stocker l'iterateur sur le membre courant
	//      si on doit remplacer la chaine
	//           proceder au remplacement
	bool found = false;
	int start = 0, end = 0;

	for (int i = 0; i < member.getName().size(); i++) {
		if (member.getName()[i] == m_findString[0]) {
			start = i;
			found = true;
			for (int j = 1; j < m_findString.size(); j++) {
				if (member.getName()[i + j] == m_findString[j]) {
					end = i + j;
				}
				else {
					found = false;
					break;
				}
			}
		}
	}

	if (found) {
		m_result.push_back(m_currentMember);
		if (m_doReplace) {
			std::string newRole;
			for (int i = 0; i < start; i++) { // ajput debut du role
				newRole += member.getName()[i];
			}
			newRole += m_replaceString; // ajout du changement
			for (int i = end + 1; i < member.getName().size(); i++) { // ajout de la fin du role
				newRole += member.getName()[i];
			}
			member.setName(newRole);
		}
	}
}

void MemberTextFindReplace::processTeamMemberRole(TeamMemberRole & member)
{
	// Traiter le role
	// Verifier si le role du membre contient la chaine cherchee. Si oui
	//      stocker l'iterateur sur le membre courant
	//      si on doit remplacer la chaine
	//           proceder au remplacement

	// Pour traiter le nom, on delegue au membre

	bool found = false;
	int start, end;
	if (!member.getRole().empty() && !m_findString.empty()) {
		for (int i = 0; i < member.getRole().size(); i++) {
			if (member.getRole()[i] == m_findString[0]) {
				found = true;
				start = i;
				for (int j = 1; j < m_findString.size(); j++) {
					if (m_findString[j] != member.getRole()[i + j]) {
						found = false;
						break;
					}
					else
						end = i + j;
				}
			}
		}
	}
	

	if (found) {
		m_result.push_back(m_currentMember);
		if (m_doReplace) {
			std::string newRole;
			for (int i = 0; i < start; i++) { // ajput debut du role
				newRole += member.getRole()[i];
			}
			newRole += m_replaceString; // ajout du changement
			for (int i = end + 1; i < member.getRole().size(); i++) { // ajout de la fin du role
				newRole += member.getRole()[i];
			}
			member.setRole(newRole);
		}
	}

	//traiter le nom
	member.getMember().accept(*this);
}

void MemberTextFindReplace::processTeam(Team & team)
{
	// Pour traiter une equipe, on itere sur tous les membres en conservant dans le 
	// visiteur l'iterateur sur le composant courant en train d'etre visite
	
	for (auto it = team.begin(); it != team.end(); ++it) {
		m_currentMember = it;
		it->accept(*this);
	}
	
}

void MemberTextFindReplace::setStrings(const std::string * find_s, const std::string * replace_s)
{
	// Initialiser la chaine de recherche (qui peut etre nulle)
	// Verifier si la chaine de remplacement est non-nulle. Si oui initialiser m_doReplace a true et
	// conserver la chaine de remplacement
	m_findString = *find_s;
	m_replaceString = *replace_s;
	if (!m_replaceString.empty()) {
		m_doReplace = true;
	}
	else
		m_doReplace = false;
}

std::string MemberTextFindReplace::findString(void) const
{
	// Retourner la chaine de recherche
	return m_findString;
}

std::string MemberTextFindReplace::replaceString(void) const
{
	// Retourner la chaine de remplacement
	return m_replaceString;
}

bool MemberTextFindReplace::doReplace(void) const
{
	// Retourner l'indicateur de remplacement
	return m_doReplace;
}

TeamComponentIteratorContainer MemberTextFindReplace::searchResult(void) const
{
	// Retourner les iterateurs sur les composantes trouvees ou modifiees
	return m_result;
}

void MemberTextFindReplace::clearSearch(void)
{
	// Vider la liste des iterateurs sur les composantes trouvees ou modifiees
	m_result.empty();
}
