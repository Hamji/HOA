#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__

#include <string>

namespace hoa {
class Manager{
    Manager();
    virtual ~Manager();

    virtual void    connect         () = 0;
    virtual void    disconnect      () = 0;
    
    std::string     get_error_msg   ()  const;
    int             get_error_code  ()  const;
    
    protected:
        int         m_db_error_code         ;
        std::string m_db_error_msg          ;
        char        m_buff[1024]            ;
        bool        m_connected             ;
};
}


#endif
