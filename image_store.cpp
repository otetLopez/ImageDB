/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;

#define ARR_LEN(arr_id) ((sizeof (arr_id))/(sizeof (arr_id)[0]))

int main(int argc, char *argv[])
{
    //Defaults
    string db_name = "";
    string table_name = "";
    string image_name = "";
    string col_name = "";
    string uname = "";
    string pwd = "";

    //Get the input filename, 
    if(argc > 1) {
        image_name = argv[1];
        cout << "Image to store: " << image_name.c_str() << endl;
    } else {
        cerr << "Please indicate image name" << endl;
        return EXIT_FAILURE;
    }

    //Get Database details
    // cout << "Enter database user: ";
    // cin >> uname;
    // cout << "Enter database password: ";
    // cin >> pwd;
    cout << "Enter database name: ";
    cin >> db_name;
    cout << "Enter table name: ";
    cin >> table_name;
    cout << "Enter column name:";
    cin >> col_name;


    //Initialize MySQL and C++ bindings
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL *connection, mysql;
    
    int state;
    mysql_init(&mysql);

    //Establish MySQL Connection
    connection = mysql_real_connect(&mysql,"127.0.0.1","root","",db_name.c_str(),0,0,0);
    if (connection == NULL)
    {
        std::cout << mysql_error(&mysql) << std::endl;
    }

  
    // Now we are about to store the image into the database
    MYSQL_STMT *pinsert_into_images_stmt = mysql_stmt_init(connection);
    //const char sql1[] = "INSERT INTO " + table_name + "(" + col_name + ") VALUES (?)";
    string squery =  "INSERT INTO " + table_name + "(" + col_name + ") VALUES (?)";
    //const char sql1[] = "INSERT INTO IMG_TABLE(Img) VALUES (?)";
    const char *sql1 = squery.c_str();
    if (mysql_stmt_prepare(pinsert_into_images_stmt, sql1, strlen(sql1)) != 0) {
        cerr << "Error: mysql_stmt_prepare() failed to prepare `" << sql1 << "`." << endl;
        return EXIT_FAILURE;
    }

    MYSQL_BIND bind_structs[] = {
        { 0 } // One for each ?-placeholder
    };

    unsigned long length0;
    bind_structs[0].length = &length0;
    bind_structs[0].buffer_type = MYSQL_TYPE_BLOB;
    bind_structs[0].is_null_value = 0;
    if (mysql_stmt_bind_param(pinsert_into_images_stmt, bind_structs) != 0) {
        cerr << "Error: mysql_stmt_bind_param() failed." << endl;
        return EXIT_FAILURE;
    }

    const char *file_name = image_name.c_str();
    FILE *fp = fopen(image_name.c_str(), "rb");
   // Use mysql_stmt_send_long_data() to send the file data in chunks.
    char buf[10*1024];
    while (!ferror(fp) && !feof(fp)) {
        size_t res = fread(buf, 1, ARR_LEN(buf), fp);
        if (mysql_stmt_send_long_data(pinsert_into_images_stmt, 0, buf, res) != 0) {
            cerr << "Error: mysql_stmt_send_long_data() failed." << endl;
            return EXIT_FAILURE;
        }
    }

    if (mysql_stmt_execute(pinsert_into_images_stmt) != 0) {
        cerr << "Error: mysql_stmt_execute() failed." << endl;
        return EXIT_FAILURE;
    }

    cout << "Inserted record #" << mysql_insert_id(connection) << endl;
  mysql_close(connection);
}