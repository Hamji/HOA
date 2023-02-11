#include <iostream>
#include "Manager.hpp"
#include "ManagerOdbc.hpp"
#include <string>
#include <sql.h>
#include <sqlext.h>
#include <unistd.h>

int main() { 
    std::string l_conn_info = "DSN=MYODBC";
    char        l_query[1024] = "INSERT INTO T_HOA_TEST  " 
                                "(INDEX_CODE, NAME, NUM) "
                                "VALUES                  "
                                "('01', 'TEST', 1)";

    std::cout << "INSERT SAMPLE START" << std::endl;

    hoa::ManagerOdbc l_odbc;
    
    SQLHSTMT l_stmt;
    SQLRETURN sql_return = l_odbc.connect(l_conn_info);
    if (sql_return != SQL_SUCCESS){
        exit(-1);
    }

    sql_return = l_odbc.prepare(&l_stmt, l_query);
    if (sql_return != SQL_SUCCESS){
        exit(-1);
    }

    sql_return = l_odbc.execute(&l_stmt);
    if (sql_return != SQL_SUCCESS){
        exit(-1);
    }

    sql_return = l_odbc.free_stmt(&l_stmt, SQL_DROP);

    return 0;
}
