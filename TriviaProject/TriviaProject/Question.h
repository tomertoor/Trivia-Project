#pragma once

#include <iostream>
#include <vector>

struct Question
{
	std::string content;
	std::string correctAnswer;
	std::vector<std::string> incorrectAnswers;

	Question operator=(const bsoncxx::v_noabi::document::view& other)
	{
		this->content = other["content"].get_utf8().value.to_string();
		this->correctAnswer = other["correctAnswer"].get_utf8().value.to_string();
		auto incorrectAnswers = other["incorrectAnswers"].get_array().value;
		for (auto& iter : incorrectAnswers)
		{
			auto test = iter.get_utf8();
			this->incorrectAnswers.push_back(iter.get_utf8().value.to_string());
		}
		return *this;
	}
};