#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__

#include <sql.h>
#include <string>

namespace hoa {
class Manager{
    public:
        Manager();

        virtual SQLRETURN    connect        (std::string _connect_info) = 0;
        virtual SQLRETURN    disconnect     () = 0;
    
        const char*     get_error_msg       ()  const;
        int             get_error_code      ()  const;
    
    protected:
        int         m_db_error_code         ;
        char        m_db_error_msg[1024]    ;
        char        m_buff[1024]            ;
        bool        m_connected             ;
};
}


#endif
