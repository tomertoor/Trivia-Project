#pragma once

#include "RoomData.h"
#include "LoggedUser.h"
#include <vector>

#define GAME_STARTED 1

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
public:
	Room(RoomData data) : m_metadata(data) {};
	RoomData& getData();
	void addUser(const LoggedUser& user);
	void removeUser(const LoggedUser& user);
	std::vector<LoggedUser> getAllUsers() const;
	void startRoom();

};