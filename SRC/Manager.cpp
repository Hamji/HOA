
#include "Manager.hpp"

#include <string>


hoa::Manager::          Manager() : m_db_error_code(0), m_connected(false){
    memset(m_db_error_msg, 0x00, sizeof(m_db_error_msg));    
}

int             hoa::Manager::   get_error_code ()  const {
    return m_db_error_code;
}

const char*     hoa::Manager::  get_error_msg   ()  const {
    return m_db_error_msg;
}
