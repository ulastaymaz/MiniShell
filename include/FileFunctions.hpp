#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <queue>
#include <ranges>
#include <system_error>
#include <chrono>
#include <locale>
#include <codecvt>
class FileFunctions
{

public:
	FileFunctions();

	//actually it is better to use std::variant<bool (or void), std::error_code> for output of the functions,
	//i'm just stick with the c++20, maybe i would implement later
	struct OpResult {
		bool success;
		std::error_code err_c;
		static OpResult scs() { return { true, {} }; }
		static OpResult fail(std::error_code ec) { return { false, ec }; }
	};

	OpResult search_file(const std::filesystem::path& the_path, const std::string& the_file) const;
	OpResult display_files(const std::filesystem::path& the_path) const;
	OpResult display_dirs(const std::filesystem::path& the_path) const;
	OpResult delete_file(const std::filesystem::path& the_path, const std::string_view file_to_delete) const;
	OpResult delete_folder(const std::filesystem::path& the_path, const std::string_view file_to_delete) const;
	OpResult parent_directory(std::filesystem::path& the_path) const;
	OpResult rename_file(const std::filesystem::path& the_path, const std::string_view old_name, const std::string_view new_name) const;
	OpResult create_directory(const std::filesystem::path& the_path, const std::string_view directory_name) const;
	OpResult enter_directory(std::filesystem::path& the_path, const std::string_view directory_name) const;
	OpResult copy_file(const std::filesystem::path& the_path, std::filesystem::path& copy_path, const std::string_view file_to_copy);
	OpResult paste_file(const std::filesystem::path& the_path, const std::filesystem::path& copy_path) const;
	OpResult info_file(const std::filesystem::path& the_path, std::string_view file_name) const;
};

