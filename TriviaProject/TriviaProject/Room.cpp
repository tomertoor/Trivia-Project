#include "Room.h"

//Returns data of room
RoomData& Room::getData()
{
	return this->m_metadata;
}

//Adds a user to the list
// Input - user: the user to add
void Room::addUser(const LoggedUser& user)
{
	this->m_users.push_back(user);
}

//Removes user, finding and erasing
void Room::removeUser(const LoggedUser& user)
{
	if (std::find(m_users.begin(), m_users.end(), user) != m_users.end())
	{
		this->m_users.erase(std::find(m_users.begin(), m_users.end(), user));

	}
}

//Returns a copy of the vector of the logged users
std::vector<LoggedUser> Room::getAllUsers() const
{
	return std::vector<LoggedUser>(this->m_users);
}

void Room::startRoom()
{
	this->m_metadata.isActive = GAME_STARTED;
}
