#pragma once
#include "../include/InputHandler.hpp"
#include "../include/FileFunctions.hpp"
#include <iostream>
#include <optional>
#include <format>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>
#include <unordered_map>
class ShellManager
{
public:

	ShellManager();
	void run_shell();
	

private:
	std::filesystem::path copy_path;
	struct _COMMANDS {
		std::string_view name;
		std::string_view aliases;
		std::string_view description;
		int args_num;
		std::function<FileFunctions::OpResult(const std::vector<std::string>&)> handler;
	};

	InputHandler input_handler;
	FileFunctions file_handler;
	void handle_command(const std::string& command);
	std::filesystem::path path = std::filesystem::current_path();
	std::unordered_map<std::string, _COMMANDS> commandMap;
};
