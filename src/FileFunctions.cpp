#include "../include/FileFunctions.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
namespace fs = std::filesystem;
using OPR = FileFunctions::OpResult;


FileFunctions::FileFunctions() {
    std::locale::global(std::locale(""));
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        //("chcp 65001 > nul");
    #endif
}


//it needs work
OPR FileFunctions::search_file(const std::filesystem::path& the_path, const std::string& the_file) const {
    std::error_code ec;
    std::queue<fs::path> directories;
    directories.push(the_path);

    while (!directories.empty()) {
        fs::path current_path = directories.front();
        directories.pop();

        try {
            for (const auto& entry : fs::directory_iterator{ current_path, ec }) {
                if (entry.path().filename().string() == the_file) {
                    std::cout << entry.path() << std::endl;
                }

                if (is_directory(entry) && !is_symlink(entry)) {
                    directories.push(entry.path());
                }
            }
            if (ec) return OpResult::fail(ec);
        }
        catch (fs::filesystem_error const e) {
            return OpResult::fail(e.code());
            //I'll just skip this part, maybe i just exclude try-catch
        }
    }
    if (ec) return OpResult::fail(ec);
    return OpResult::scs();

}

OPR FileFunctions::display_files(const std::filesystem::path& the_path) const {
    std::error_code ec;
    if (!fs::is_directory(the_path, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::not_a_directory));
    for (fs::directory_iterator it(the_path, ec); !ec && it != fs::directory_iterator() ;it.increment(ec)) {
        //need wcout for some charachters, o/w it crashes
        std::wcout << it->path().filename().wstring() << std::endl;
    }
    if (ec) return OPR::fail(ec);
    return OPR::scs();

}

OPR FileFunctions::display_dirs(const std::filesystem::path& the_path) const {
    std::error_code ec;
    if (!fs::is_directory(the_path, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::not_a_directory));
    for (fs::directory_iterator it(the_path, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
        if (it->is_directory()) std::cout << it->path().filename().string() << std::endl;
    }
    if (ec) return OPR::fail(ec);
    return OPR::scs();

}

OPR FileFunctions::delete_folder(const std::filesystem::path& the_path, const std::string_view file_to_delete) const {
    std::error_code ec;
    if (!fs::is_directory(the_path / file_to_delete, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::not_a_directory));
    if (fs::exists(the_path / file_to_delete, ec)) {
        fs::remove_all(the_path / file_to_delete, ec);
    }
    if (ec) return OPR::fail(ec);
    return OPR::scs();

}

OPR FileFunctions::delete_file(const std::filesystem::path& the_path, const std::string_view file_to_delete) const {
    std::error_code ec;
    if (!fs::is_regular_file(the_path / file_to_delete, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::bad_file_descriptor));
    if (fs::exists(the_path / file_to_delete, ec)) {
        fs::remove(the_path / file_to_delete, ec);
    }
    if (ec) return OPR::fail(ec);
    return OPR::scs();

}

OPR FileFunctions::parent_directory(std::filesystem::path& the_path) const {
    std::error_code ec;
    if(the_path.has_parent_path()) {
	    the_path = the_path.parent_path();
    }
    else {
        return OPR::fail(std::make_error_code(std::errc::permission_denied));
    }
    return OPR::scs();
}

OPR FileFunctions::create_directory(const std::filesystem::path& the_path, const std::string_view directory_name) const {
    //this if statement might be unnecessary
    if (directory_name.empty()) return OPR::fail(std::make_error_code(std::errc::invalid_argument));
    std::error_code ec;
    if (fs::exists(the_path / directory_name, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::is_a_directory));

    fs::create_directory(the_path / directory_name, ec);

    if (ec) return OPR::fail(ec);
    return OPR::scs();
}

OPR FileFunctions::enter_directory(std::filesystem::path& the_path, const std::string_view directory_name) const {
    std::error_code ec;
    if(!fs::is_directory(the_path/directory_name, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::invalid_argument));
    fs::path target_path = the_path / directory_name;
    the_path = fs::canonical(target_path, ec);
    if (ec) return OPR::fail(ec);
    return OPR::scs();
    //need a little work here
}

OPR FileFunctions::rename_file(const std::filesystem::path& the_path, const std::string_view old_name, const std::string_view new_name) const {
    std::error_code ec;
    fs::rename(the_path / old_name, the_path / new_name ,ec);
    if (ec) return OPR::fail(ec);
    return OPR::scs();
}

OPR FileFunctions::copy_file(const std::filesystem::path& the_path, std::filesystem::path& copy_path, const std::string_view file_to_copy) {
    std::error_code ec;
    if (!fs::exists(the_path / file_to_copy, ec)) return OPR::fail(ec ? ec : std::make_error_code(std::errc::no_such_file_or_directory));
	copy_path = the_path / file_to_copy;
    return OPR::scs();
    //gotta check this again
}

OPR FileFunctions::paste_file(const std::filesystem::path& the_path, const std::filesystem::path& copy_path) const {
    std::error_code ec;

    if (!fs::exists(copy_path, ec)) return OPR::fail(ec);

    fs::copy(copy_path, the_path / copy_path.filename(),ec);

    if (ec) return OPR::fail(ec);
    return OPR::scs();

}

OPR FileFunctions::info_file(const std::filesystem::path& the_path, std::string_view file_name) const {
    std::error_code ec;
    if (!std::filesystem::exists(the_path / file_name,ec )) return OPR::fail(ec ? ec : std::make_error_code(std::errc::invalid_argument));
    std::cout << "File name:  " << file_name << std::endl
        << "File Size:  " << std::filesystem::file_size(the_path / file_name, ec) << std::endl;

    if (ec) return OPR::fail(ec);//file size error??
    std::filesystem::file_time_type ftime = std::filesystem::last_write_time(the_path / file_name, ec);
    if (ec) return OPR::fail(ec);//time error
    std::cout << std::format("Last change: {}\n", ftime);
    return OPR::scs();
}



