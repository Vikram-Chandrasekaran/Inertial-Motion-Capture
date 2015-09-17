#include "csv_log.hpp"

int CSVLog::open() {
    if(!columns_locked) {
        std::cout << TAG_ERROR << "CSV columns must be locked to open" << std::endl;
        return IMC_FAIL;
    }
    
    if(!file_open) {
        file.open(file_path);
        file_open = true;
    }

    if(!columns_written) {
        for(int i = 0; i < columns.size(); i++) {
            file << columns[i];

            if(i != columns.size() - 1) {
                file << ",";
            }
        }

        file << std::endl;

        columns_written = true;
    }

    return IMC_SUCCESS;
}

int CSVLog::close() {
    if(file_open) {
        file.close();
        file_open = false;
    }

    return IMC_SUCCESS;
}

int CSVLog::add_column(std::string key) {
    for(int i = 0; i < columns.size(); i++) {
        if(columns[i] == key) {
            std::cout << TAG_ERROR << "Can not add column that exists \"" << key << "\"" << std::endl;
            return IMC_FAIL;
        }
    }

    if(!columns_locked) {
        columns.push_back(key);
    }
}

int CSVLog::lock_columns() {
    columns_locked = true;
}

int CSVLog::add_to_line(std::string key, std::string value) {
    // Make sure key exists
    bool key_found = false;

    for(int i = 0; i < columns.size(); i++) {
        if(key == columns[i]) {
            key_found = true;
        }
    }

    if(!key_found) {
        std::cout << TAG_ERROR << "Key \"" << key << "\" not found in columns" << std::endl;
        return IMC_FAIL;
    }

    // Insert
    line[key] = value;

    return IMC_SUCCESS;
}

int CSVLog::add_to_line(std::string key, double value) {
    return add_to_line(key, double_to_str(value));
}

int CSVLog::finish_line() {
    for(int i = 0; i < columns.size(); i++) {
        file << line[columns[i]];

        if(i != columns.size() - 1) {
            file << ",";
        }
    }

    file << std::endl;

    line.clear();
}

std::string CSVLog::double_to_str(double value) {
    std::stringstream stream;
    stream << value;
    return stream.str();
}