# ImageDB
C++ Implementation storing image in MySQL connected via localhost

Environtment:
This code is compiled, ran and tested in a Mac Big Sur setup.


Setting up the database:
1. Install MySQL using brew : brew install mysql
2. Follow installation instruction to start mysql service : brew services start mysql
3. Run mysql using root access : mysql -u root
4. Create the databse :
      CREATE DATABASE IF NOT EXISTS IMAGEDB;
      USE IMAGEDB;

      CREATE TABLE IF NOT EXISTS IMG_TABLE( 
      Id int PRIMARY KEY AUTO_INCREMENT, 
      Img MEDIUMBLOB NOT NULL,
      TimeStmp2 TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
      );
5. Create user to grant privileges for the database :
  CREATE USER 'imgdb'@'localhost' IDENTIFIED BY 'imgdb';
  GRANT ALL PRIVILEGES ON IMAGEDB.* TO 'imgdb'@'localhost';
  FLUSH PRIVILEGES;

6. In another terminal tab/window, Install C++-MySQL bindings : brew install mysql-connector-c++

7. Compile the CPP code in this repository using the command below :
   g++ -std=gnu++11  -I/usr/local/include -I/usr/local/include/mysql -W -lmysqlclient -L/usr/local/lib image_store.cpp -o store
   
8. Run the code specifying the file name of the image : 
   ./store image.jpg
   
    While running the code, you will be prompted for the following details (this is done in case the database schema is different from steps 1-5)
    Image to store: image.jpg
    Enter database user: imgdb
    Enter database password: imgdb
    Enter database name: IMAGEDB
    Enter table name: IMG_TABLE
    Enter column name:Img
    
    Upon table insertion success, the following message is printed:
    Inserted record #
  
