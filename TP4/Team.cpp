#include "Team.h"

//constructeur
Team::Team(std::string name)
{
	// A Completer
	m_name = name;
}

//constructeur par copie
Team::Team(const Team & mdd)
{
	// A Completer (constructeur de copie)
	m_name = mdd.getName();
	//m_image = mdd.getImage();
	//m_members = mdd.m_members;
	
}

Team * Team::clone(void) const
{
	// A Completer (créer un nouvel objet identique a this)
	//Team* clone = new Team(*this);
	return (new Team(*this));
}

const QImage & Team::getImage(void) const
{
	// A Completer
	return m_image;
}

QImage & Team::getImage(void)
{
	// A Completer
	return m_image;
}

std::string Team::getName(void) const
{
	// A Completer
	return m_name;
}

void Team::setName(std::string name)
{
	// A Completer
	m_name = name;
}

AbsTeamComponent& Team::addTeamComponent(const AbsTeamComponent & member)
{
	// A Completer: Ajouter un nouvel element dans la liste et le clonant et
	// retrourner une reference a l'objet insere dans la liste

	AbsTeamComponent *memberClone = member.clone();
	
	m_members.push_back(TeamComponentPtr(memberClone));
	//m_members.insert(m_members.begin(), memberClone);

	return *memberClone;
}

TeamComponentIterator Team::begin()
{
	// A Completer
	return m_members.begin();
}

TeamComponentIterator_const Team::cbegin() const
{
	// A Completer
	return m_members.cbegin();
}

TeamComponentIterator_const Team::cend() const
{
	// A Completer
	return m_members.cend();
}

TeamComponentIterator Team::end()
{
	// A Completer
	return m_members.end();
}

void Team::deleteTeamComponent(TeamComponentIterator_const child)
{
	// A Completer: eliminer de la liste l'element indique par l'iterateur
	m_members.erase(child);
}

void Team::deleteAllComponents(void)
{
	// A Completer : vider la liste et les attributs membres
	m_members.clear();
}
