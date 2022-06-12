#pragma once

#include "RoomData.h"
#include "LoggedUser.h"
#include <vector>
#include <mutex>

#define GAME_STARTED 1

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser>* m_users;
	std::mutex userMutex;
public:
	Room(RoomData data) : m_metadata(data), m_users(new std::vector<LoggedUser>()) {};
	~Room() { delete this->m_users; };
	RoomData& getData();
	void addUser(const LoggedUser& user);
	void removeUser(const LoggedUser& user);
	std::vector<LoggedUser> getAllUsers();
	void startRoom();

};