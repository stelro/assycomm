/**
 * sql_info.cpp ---- sql queries executed!
 * Copyright (c) 2015 Stelmach Ro
 *
 * Author: Stelmach Ro (xtasy) <stelmach.ro@gmail.com>
 *
 * This file is part of Assy Comm.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <string>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>
#include "mysql_connection.h"

#include "post_info.h"
#include "sql_info.h"
#include "conflib.h"
#include "help_lib.h"

using namespace std;
using namespace sql;

sql_requests::sql_requests(string username, string password) {

    this->sql_name = username;
    this->sql_pass = password;
}

long long sql_requests::sql_get_id() {
    
    /* Getting the las inserted ID key from DB */
    long long id_number;

    try {
        
        statement = connection->createStatement();
        result = statement->executeQuery("SELECT @@identity AS id");
        result->next();
        id_number = result->getInt64("id");
        
        delete statement;
        delete result;
        
        return id_number;
    }
    
    catch (SQLException &e) {
        cout << "[SQL] Error: SQLException in: " << __FILE__ << endl;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "[SQL] Error: " << e.what() << endl;
        cout << "(MySQL Error Code: " << e.getErrorCode();
        cout << "SQLState: " << e.getSQLState() << " )" << endl;
        
        return 0;
    }
}

bool sql_requests::sql_connect() {

    ConfigFile cfg("config.assy");
    string url = cfg.getValueOfKey<string>("addr_url");
    string db_name = cfg.getValueOfKey<string>("db_name");

    try {
        
        driver = get_driver_instance();
        connection = driver->connect(url,sql_name,sql_pass);
        connection->setSchema(db_name); //Database name
        return true;
    }
    
    catch (SQLException &e) {
            cout << "[SQL] Error: SQLException in: " << __FILE__ << endl;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	    cout << "[SQL] Error: " << e.what() << endl;
	    cout << "(MySQL Error Code: " << e.getErrorCode();
	    cout << "SQLState : " << e.getSQLState() << " )" << endl;

            return false;
    }
}

bool sql_requests::sql_query() {
    
    try {
        pStatement = connection->prepareStatement(informations);
        pStatement->executeUpdate();
        delete pStatement;
        
        return true;
    }

    catch (SQLException &e) {
            cout << "[SQL] Error: SQLException in: " << __FILE__ << endl;
            cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	    cout << "[SQL] Error: " << e.what() << endl;
	    cout << "(MySQL Error Code: " << e.getErrorCode();
	    cout << "SQLState : " << e.getSQLState() << " )" << endl;

            return false;
    }
}

void sql_requests::set_post_info() {

    /*Colors Modifiers */ 
    Color::Modifier c_red(Color::FG_RED);
    Color::Modifier c_def(Color::FG_DEFAULT);
    Color::Modifier c_yel(Color::FG_YELLOW);
    Color::Modifier c_gre(Color::FG_GREEN);
    Color::Modifier c_bol(Color::FG_BOLD);
    /* --------------- */ 

    
    assycom asc; //AssyCom class
    string post_name, post_title;
    bool comment_status;

    cout  << "[-]" << ".. post informations : " << endl;
    cout << endl;
    /* ------ Get User Informations ------- */
    cout <<  "[+]" << ".. post Title: ";
    getline(cin,post_title);
    cin.ignore();
    cout << "[+]" << ".. comment Status, 1. for OPEN, 0. for Close: ";
    cin >> comment_status;
    cin.ignore();
    cout <<  "[+]" << ".. post Name: ";
    cin >> post_name;
    cin.ignore();
    cout << "[+]" << ".. post Content's[ENTER TO END]: ";
    asc.read_text();
    cin.ignore();
    cout << endl;

    asc.user_define(post_title,comment_status,post_name);
    this->informations = asc.return_values("config.assy");
    
}

void sql_requests::print_post_info() {
    
    cout << "\n";

    cout << "------------------------------\n";
    cout << informations << "\n";
    cout << "------------------------------\n";
    cout << "\n";
}


