#include "ManagerOdbc.hpp"
#include <unistd.h>

hoa::ManagerOdbc::  ManagerOdbc(){
    m_env_handle        = SQL_NULL_HENV;
    m_connect_handle    = SQL_NULL_HDBC;
    init_error_msg();
}

hoa::ManagerOdbc::  ~ManagerOdbc(){
    SQLRETURN   l_return_code;
    this->disconnect();
    if (m_connect_handle != nullptr) {
        l_return_code = SQLFreeConnect(m_connect_handle);
        if(l_return_code != SQL_SUCCESS) {
            check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
            fprintf(stderr, "~ManagerOdbc Error: %s", m_db_error_msg);
        }
    }

    if (m_env_handle != nullptr) {
        l_return_code = SQLFreeEnv(m_env_handle);
        if (l_return_code != SQL_SUCCESS) {
            check_error(l_return_code, SQL_HANDLE_ENV, m_env_handle);
            fprintf(stderr, "~ManagerOdbc Error: %s", m_db_error_msg); 
        }
    }

}

SQLRETURN       hoa::ManagerOdbc::  disconnect  (){
    SQLRETURN l_return_code;
    if (m_connect_handle != NULL) {
        l_return_code = SQLDisconnect(m_connect_handle);
        if (l_return_code != SQL_SUCCESS) {
            check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
            fprintf(stderr, "disconnect() Error: %s", m_db_error_msg);
        }
    }
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  connect         (std::string _connect_info
                                                     ){
    SQLRETURN l_return_code;

    /* init env handle */
    l_return_code = SQLAllocEnv(&m_env_handle);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_ENV, m_env_handle);
        fprintf(stderr, "connect() Error: %s", m_db_error_msg);
        return l_return_code;
    }
    
    /* init connection handle */
    l_return_code = SQLAllocConnect(m_env_handle, &m_connect_handle);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "connect() Error: %s", m_db_error_msg);
    }

    /* connect with connection information */
    l_return_code = SQLDriverConnect(m_connect_handle,
                                     nullptr, 
                                     (SQLCHAR*)(_connect_info.c_str()),
                                     (SQLINTEGER)strlen(_connect_info.c_str()),
                                     nullptr,
                                     0,
                                     nullptr,
                                     SQL_DRIVER_NOPROMPT);

    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "connect() Error: %s", m_db_error_msg);
    } 

    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  rollback        (){
    SQLRETURN l_return_code;

    l_return_code = SQLEndTran(SQL_HANDLE_DBC, m_connect_handle, SQL_ROLLBACK);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "rollback() Error: %s", m_db_error_msg);
    }
    
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  commit          (){
    SQLRETURN l_return_code;

    l_return_code = SQLEndTran(SQL_HANDLE_DBC, m_connect_handle, SQL_COMMIT);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "commit() Error: %s", m_db_error_msg);
    }

    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  prepare         (SQLHSTMT*      _stmt   ,
                                                     char*          _query
                                                     ){
    SQLRETURN l_return_code;

    /* alloc */
    l_return_code = SQLAllocStmt(m_connect_handle, _stmt);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "alloc Error: %s", m_db_error_msg);   
        return l_return_code;
    }
    
    /* prepare */
    l_return_code = SQLPrepare(*_stmt, 
                               (SQLCHAR*)_query, 
                               (SQLINTEGER)strlen(_query));
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "prepare() Error: %s", m_db_error_msg);
    }
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  bind_parameter  (SQLHSTMT*      _stmt   ,
                                                     SQLBINDPARAM*  _param  ,
                                                     int            _list_size
                                                     ){
    SQLRETURN l_return_code;

    for (int i = 0 ; i < _list_size; i++) {
        l_return_code = SQLBindParameter(*_stmt,
                                         _param[i].index_param,
                                         SQL_PARAM_INPUT,
                                         _param[i].val_type,
                                         _param[i].sql_val_type,
                                         _param[i].col_size,
                                         _param[i].decial_digit,
                                         _param[i].param_val_ptr,
                                         _param[i].max_buffer_size,
                                         _param[i].buffer_size
                                         );
        if (l_return_code != SQL_SUCCESS) {
            check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
            fprintf(stderr, "bind_parameter() Error: %s", m_db_error_msg);
            return l_return_code;
        }
    }
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  bind_column     (SQLHSTMT*      _stmt   ,
                                                     SQLBINDCOL*    _col    ,
                                                     int            _list_size
                                                     ){
    SQLRETURN l_return_code;
    for (int i = 0; i < _list_size; i++) {
        l_return_code = SQLBindCol(*_stmt,
                                   _col[i].index_param,
                                   _col[i].val_type,
                                   _col[i].target_ptr,
                                   _col[i].buffer_size,
                                   nullptr);
        if (l_return_code != SQL_SUCCESS) {
            check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
            fprintf(stderr, "bind_column() Error: %s", m_db_error_msg);
            return l_return_code;
        }
    }
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  execute         (SQLHSTMT*      _stmt
                                                     ){
    SQLRETURN l_return_code;
    
    l_return_code = SQLExecute(*_stmt);
    if (l_return_code != SQL_SUCCESS && l_return_code != SQL_NO_DATA_FOUND) {
        check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
        fprintf(stderr, "execute() Error: %s", m_db_error_msg);
    }
    return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  execute_direct  (char*          _query
                                                     ){
    SQLRETURN   l_return_code;
    SQLHSTMT    l_stmt;

    /* alloc */
    l_return_code = SQLAllocStmt(m_connect_handle, &l_stmt);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_DBC, m_connect_handle);
        fprintf(stderr, "execute_direct() Error: %s", m_db_error_msg);
        return l_return_code;
    }

    /* execute direct */
    l_return_code = SQLExecDirect(l_stmt,
                                  (SQLCHAR*)_query,
                                  (SQLINTEGER)strlen(_query));
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_STMT, l_stmt);
        fprintf(stderr, "execute_direct() Error: %s", m_db_error_msg);
        return l_return_code;
    }

    /* free stmt */
    l_return_code = SQLFreeStmt(l_stmt, SQL_DROP);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_STMT, l_stmt);
        fprintf(stderr, "free_stmt() Error: %s", m_db_error_msg);
    }
    return l_return_code;
    

}

SQLRETURN       hoa::ManagerOdbc::  fetch           (SQLHSTMT*      _stmt
                                                     ){
     SQLRETURN l_return_code;

     l_return_code = SQLFetch(*_stmt);
     if (l_return_code != SQL_SUCCESS && l_return_code != SQL_NO_DATA_FOUND) {
         check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
         fprintf(stderr, "fetch() Error: %s", m_db_error_msg);
     }
     return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  close_cursor    (SQLHSTMT*      _stmt
                                                     ){
     SQLRETURN l_return_code;

     l_return_code = SQLCloseCursor(*_stmt);
     if (l_return_code != SQL_SUCCESS) {
         check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
         fprintf(stderr, "close_cursor() Error: %s", m_db_error_msg);
     }
     return l_return_code;
}

SQLRETURN       hoa::ManagerOdbc::  free_stmt       (SQLHSTMT*      _stmt,
                                                     SQLSMALLINT    _option
                                                     ){
     SQLRETURN l_return_code;

     l_return_code = SQLFreeStmt(*_stmt, _option);
     if (l_return_code != SQL_SUCCESS) {
         check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
         fprintf(stderr, "free_stmt() Error: %s", m_db_error_msg);
     }
     return l_return_code;
}

SQLINTEGER      hoa::ManagerOdbc::  get_row_count   (SQLHSTMT*      _stmt
                                                     ){
    SQLRETURN   l_return_code;
    SQLLEN      l_row = -1;

    l_return_code = SQLRowCount(*_stmt, &l_row);
    if (l_return_code != SQL_SUCCESS) {
        check_error(l_return_code, SQL_HANDLE_STMT, *_stmt);
        fprintf(stderr, "get_row_count() Error: %s", m_db_error_msg);
    }
    return l_row;
}

void            hoa::ManagerOdbc::  init_error_msg  (){
    memset(m_db_error_msg, 0x00, sizeof(m_db_error_msg));
}

void            hoa::ManagerOdbc::  check_error     (SQLRETURN      _ret_code,
                                                     SQLSMALLINT    _h_type,
                                                     SQLHANDLE      _handle
                                                      ){
    init_error_msg();
    char    l_msg[64]  = { 0 };
    SQLCHAR l_code[64] = { 0 };
    SQLCHAR db_msg[SQL_MAX_MESSAGE_LENGTH] = { 0 };

    switch (_ret_code){
        case SQL_SUCCESS_WITH_INFO:
            snprintf(l_msg, sizeof(l_msg), "%s", 
                    "Success But Warnning");
            break;
        case SQL_NO_DATA_FOUND:
            snprintf(l_msg, sizeof(l_msg), "%s",
                    "No Data Found");
            break;
        case SQL_INVALID_HANDLE:
            snprintf(l_msg, sizeof(l_msg), "%s",
                    "Invalid Handle");
            break;
        case SQL_ERROR:
            snprintf(l_msg, sizeof(l_msg), "%s",
                    "Error");
            break;
        default:
            snprintf(l_msg, sizeof(l_msg), "%s",
                    "Unknown Error");
            break;
    }
    SQLGetDiagRec(_h_type, 
                  _handle, 
                  1, 
                  l_code, 
                  nullptr,
                  db_msg, 
                  SQL_MAX_MESSAGE_LENGTH, 
                  nullptr);
    snprintf(m_db_error_msg, sizeof(m_db_error_msg),
             "%s |CODE: %s, MSG: %s]", l_msg, l_code, db_msg);
}
