
#include "Manager.hpp"

#include <string>


hoa::Manager::          Manager() : m_db_error_code(0), m_connected(false){

}

int         hoa::Manager::   get_error_code ()  const {
    return m_db_error_code;
}

std::string hoa::Manager::  get_error_msg   ()  const {
    return m_db_error_msg;
}
