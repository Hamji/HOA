#ifndef __MANAGER_ODBC_HPP__
#define __MANAGER_ODBC_HPP__

#include <sql.h>
#include <sqlext.h>
#include <memory>
#include <string>
#include <map>

#include "Manager.hpp"

namespace db{
namespace odbc{
    #define     ODBC_CONNECT_ERROR  1000
    #define     ODBC_STANDARD_ERROR 8000
    #define     ODBC_DUP_KEY_ERROR  8001
    #define     ODBC_UNKNOWN_ERROR  9999

    typedef struct  _STRUCT_DB_INFO {
        char user_id[256]   ;
        char passwd [256]   ;
        char ip     [64]    ;
        char port   [8]     ;
    } DB_INFO;

    typedef struct _STRUCT_SQL_BINDPARAMETER {
        SQLUSMALLINT    index_param     ;       // INDEX OF PARAMTER
        SQLSMALLINT     val_type        ;       // PARAMTER VALUE TYPE
        SQLSMALLINT     sql_val_type    ;       // SQL PARAMTER  VALUE TYPE
        SQLULEN         col_size        ;       // COLUMN SIZE
        SQLSMALLINT     decial_digit    ;       // 
        SQLPOINTER      param_val_ptr   ;       // 
        SQLLEN          max_buffer_size ;       // BUFFER LENGTH
        SQLLEN          *buffer_size    ;       // 
    } SQLBINDPARAM;


    typedef struct _STRUCT_SQL_BINDCOLUMN {
        SQLUSMALLINT    index_param     ;       // INDEX OF PARAMTER
        SQLSMALLINT     val_type        ;       // VALUESQLRETURN TYPE
        SQLPOINTER      target_ptr      ;       // TARGET POINTER 
        SQLLEN          buffer_size     ;       // BUFFER SIZE 
    } SQLBINDCOL;
}
}

using namespace db::odbc;

namespace hoa {
class ManagerOdbc : public Manager {
    public:
        ManagerOdbc();
        ~ManagerOdbc();

    public:
// func about connection to db
        SQLRETURN       disconnect      ();
        SQLRETURN       connect         (std::string _connect_info);

// func about SQL QUERY
        SQLRETURN       rollback       ();
        SQLRETURN       commit          ();
        
        SQLRETURN       prepare         (SQLHSTMT*          _stmt,
                                         char*              _query      );

        SQLRETURN       bind_parameter  (SQLHSTMT*          _stmt, 
                                         SQLBINDPARAM*      _param,
                                         int                _list_size  );
    
        SQLRETURN       bind_column     (SQLHSTMT*          _stmt,
                                         SQLBINDCOL*        _col,
                                         int                _list_size  );
        
        SQLRETURN       execute         (SQLHSTMT*          _stmt       );
        
        SQLRETURN       execute_direct  (char*              _query      );

        SQLRETURN       fetch           (SQLHSTMT*          _stmt       );
        
        SQLRETURN       close_cursor    (SQLHSTMT*          _stmt       );
        
        SQLRETURN       free_stmt       (SQLHSTMT*          _stmt,
                                         SQLSMALLINT        _option     );

//        unsigned long   set_auto_commit (unsigned long      _auto       );
//        unsigned long   get_auto_commit () const                         ;    

        SQLINTEGER      get_row_count   (SQLHSTMT*          _stmt       );


    private:
        void            init_error_msg  ();
        void            check_error     (SQLRETURN          _ret_code,  
                                         SQLSMALLINT        _h_type,
                                         SQLHANDLE          _handle     );

        SQLHENV         m_env_handle;
        SQLHDBC         m_connect_handle;
        char            m_DSN[512];

}; 
}

#endif
