/****************************************************************************
**
** Classe TeamManager
** Author: p482457
** Date: 25-oct-2019
**
****************************************************************************/

#include "TeamManager.h"

TeamManager::TeamManager(void)
	: m_teams()//, m_currentTeam(m_teams.end())
{
}

void TeamManager::loadTeamFromFile(std::string fileName)
{
}

#if 0
TeamMember& TeamManager::addMemberToTeam(std::string name, const QImage & photo)
{
	if (m_currentTeam == end())
		addTeam("Default Team");

	m_currentTeam->addTeamComponent(TeamMember(name, photo));
	return dynamic_cast<TeamMember&>( *(m_currentTeam->begin()) );
}

Team& TeamManager::addTeam(std::string name)
{
	m_teams.push_front(TeamComponentPtr(new Team(name)));
	m_currentTeam = m_teams.begin();
	return dynamic_cast<Team&>(*m_currentTeam);
}
#endif

const TeamMember& TeamManager::addMemberToTeam(Team& team, std::string name, const QImage & photo)
{
	return dynamic_cast<const TeamMember&>(team.addTeamComponent(TeamMember(name, photo)));
}

const TeamMemberRole & TeamManager::addMemberToTeamwRole(Team & team, std::string name, const QImage & photo, std::string role)
{
	AbsTeamComponent& newMember = team.addTeamComponent(TeamMemberRole(name, photo, role));
	return dynamic_cast<const TeamMemberRole&>(newMember);
	//return dynamic_cast<const TeamMemberRole&>(team.addTeamComponent(TeamMemberRole(name, photo, role)));
}

Team& TeamManager::addTeam(std::string name)
{
	m_teams.push_back(TeamComponentPtr(new Team(name)));
	return dynamic_cast<Team&>(*(m_teams.back()));
}

TeamComponentIterator TeamManager::begin()
{
	return m_teams.begin();
}

TeamComponentIterator_const TeamManager::cbegin() const
{
	return m_teams.cbegin();
}

TeamComponentIterator_const TeamManager::cend() const
{
	return m_teams.cend();
}

TeamComponentIterator TeamManager::end()
{
	return m_teams.end();
}

#if 0
TeamComponentIterator TeamManager::currentTeam()
{
	return m_currentTeam;
}

TeamComponentIterator_const TeamManager::currentTeam() const
{
	return m_currentTeam;
}
#endif

void TeamManager::deleteTeam(TeamComponentIterator_const child)
{
	m_teams.erase(child);
}

void TeamManager::deleteAllTeams(void)
{
	m_teams.clear();
	//m_currentTeam = m_teams.end();
}
