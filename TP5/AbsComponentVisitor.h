/****************************************************************************
**
** Classe AbsComponentVisitor
** Author: p482457
** Date: 25-oct-2019
**
****************************************************************************/

#ifndef _AbsComponentVisitor_Header_
#define _AbsComponentVisitor_Header_

class AbsComponentVisitor
{
public:
	AbsComponentVisitor(void) {};
	virtual ~AbsComponentVisitor(void) {};

	virtual void processTeamMember(class TeamMember& member) = 0;
	virtual void processTeamMemberRole(class TeamMemberRole& member) = 0;
	virtual void processTeam(class Team& team) = 0;
};

#endif
