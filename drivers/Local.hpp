/* Local driver for MagnasTareas */
#ifndef LOCAL_HPP
#define LOCAL_HPP

#include "sqlite3.h"
#include "../core/MagnasTareasDriver.hpp"
#include "../core/Task.hpp"
#include "../core/Category.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

extern std::string newUUID();

class LocalDriver : public MagnasTareasDriver {
public:
	LocalDriver(){
		int result;
		std::cout << "Local driver. Using SQlite: " << SQLITE_VERSION << std::endl;
		std::string tableSentence="CREATE TABLE IF NOT EXISTS MagnasTareas(TITLE TEXT, UUID TEXT UNIQUE, DESCRIPTION TEXT, CATEGORY TEXT, CATEGORY_UUID TEXT, FINISHED INTEGER);";
		#ifdef WIN32
		std::string path=std::getenv("APPDATA");
		#else
		std::string path=std::getenv("HOME");
		#endif
		path+="/.MagnasTareas";
		
		result=sqlite3_open(path.c_str(),&db);
		if(result!=SQLITE_OK){
			std::cerr << "Error while opening the database" << std::endl;
			std::exit(2);
		}
		result=sqlite3_exec(db,tableSentence.c_str(),0,0,0);
		if(result!=SQLITE_OK){
			std::cerr << "Error while creating the database table" << std::endl;
		}
		
		std::string sentence="INSERT INTO MagnasTareas VALUES('Welcome to Magnas Tareas','welcome-1','Welcome to Magnas Tareas. Start using it by adding new tasks','Help','help-1',0);";
		sqlite3_exec(db,sentence.c_str(),0,0,0);
		
		
		
		
	};
	~LocalDriver(){
		sqlite3_close(db);
	};
	bool IsEnabled(){
		return true; // Always true because local storage always works
	}
	std::vector<Category> GetCategories(){
		std::vector<Category> categories;
		int result;
		std::string sentence="SELECT DISTINCT CATEGORY_UUID,CATEGORY FROM MagnasTareas;";
		
		sqlite3_stmt* next;
		if(sqlite3_prepare(db,sentence.c_str(),std::strlen(sentence.c_str()),&next,0)==SQLITE_OK){
			while(sqlite3_step(next)==SQLITE_ROW){
				Category cat;
				const char* category=reinterpret_cast<const char*>(sqlite3_column_text(next,1));
				cat.name=category;
				const char* uuid=reinterpret_cast<const char*>(sqlite3_column_text(next,0));
				cat.uuid=uuid;
				
				std::string sent="SELECT TITLE, UUID, DESCRIPTION, FINISHED FROM MagnasTareas WHERE CATEGORY_UUID='"+cat.uuid+"';";
				sqlite3_stmt* next2;
				if(sqlite3_prepare(db,sent.c_str(),std::strlen(sent.c_str()),&next2,0)==SQLITE_OK){
					while(sqlite3_step(next2)==SQLITE_ROW){
						Task task;
						task.name=reinterpret_cast<const char*>(sqlite3_column_text(next2,0));
						task.uuid=reinterpret_cast<const char*>(sqlite3_column_text(next2,1));
						task.description=reinterpret_cast<const char*>(sqlite3_column_text(next2,2));
						task.completed=(bool)sqlite3_column_int(next2,3);
						cat.tasks.push_back(task);
					}
				}
				categories.push_back(cat);
			}
		}

		return categories;
	}
	
	void AddTask(std::string title, std::string description, std::string category, std::string preparedCategoryName){
		std::string search="SELECT CATEGORY FROM MagnasTareas WHERE CATEGORY_UUID='"+category+"';";
		sqlite3_stmt* next;
		std::cout << "SAVING IN SQLITE " << title << std::endl;
		if(sqlite3_prepare(db,search.c_str(),std::strlen(search.c_str()),&next,0)==SQLITE_OK){
			if(sqlite3_step(next)==SQLITE_ROW){
				std::string categoryName=reinterpret_cast<const char*>(sqlite3_column_text(next,0));
				std::cout << "Category UUID: " << categoryName << std::endl;
				std::string sentence="INSERT INTO MagnasTareas VALUES('"+title+"','"+newUUID()+"','"+description+"','"+categoryName+"','"+category+"',0);";
				sqlite3_exec(db,sentence.c_str(),0,0,0);
				std::cout << "Finished" << std::endl;
			}else{
				std::string sentence="INSERT INTO MagnasTareas VALUES('"+title+"','"+newUUID()+"','"+description+"','"+preparedCategoryName+"','"+category+"',0);";
				sqlite3_exec(db,sentence.c_str(),0,0,0);
			}
		}
	}
	
	void RemoveTask(std::string uuid){
		std::string sentence="DELETE FROM MagnasTareas WHERE UUID='"+uuid+"';";
		sqlite3_exec(db,sentence.c_str(),0,0,0);
	}
private:
	sqlite3* db;

};

#endif
