#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <iomanip>

using namespace std;

// cuckoo tables' size
const int tablesize = 17;

// combine the two 1-dimensional table into one 2-dimensional table
char t[tablesize][2][255];

// compute the hash function
size_t f(char*, size_t);

// place a string in one of the hash tables
bool place_in_hash_tables(char*);

int main() {

     // the strings to be stored in the hash tables
     char s[255] = "";
     char null_st[] = "";
     size_t i, len;
     bool placed;

     // clear the tables
     for (i = 0; i < tablesize; i++) {
          strcpy(t[i][0], null_st);
          strcpy(t[i][1], null_st);
     }

     char filename[255] = "";

     // display the header
     cout << endl << "CPSC 335-03 - Programming Assignment #4: ";
     cout << "Cuckoo Hasing algorithm" << endl;
     cout << "Jose M. Urrutia" << endl << endl;

     // read the strings from a file
     cout << "Input the file name (no spaces)!" << endl;
     cin >> filename;
     cout << endl;
     // open the file for reading
     FILE *file = fopen(filename, "r");

     if (file != NULL) {
          // read line by line from the file
          while (fgets(s, 255, file) != NULL) {
               // place null character at the end of the line instead of <return>
               len = strlen(s);

               // Since I am using Visual Studio with Windows OS, I use s[len - 1]
               s[len - 1] = '\0';
               // I have tested the source code in Xubuntu and the line of code below works
               //s[len - 2] = '\0';
               
               // insert the string in the cuckoo table
               placed = place_in_hash_tables(s);

               // check whether the placement was successful
               if (!placed) {
                    cout << "Placement has failed" << endl;
                    return -1;
               }
          }
          fclose(file);
     }
     else {
          perror(filename); // why didn't the file open?
     }
     cout << endl;
     cout << setw(2) << "Index" << setw(23) << "Table 1" << setw(37) << "Table 2" << endl;
     cout << "---------------------------------------------------------------------------------------------" << endl;
     for (int x = 0; x < tablesize; x++) {
          cout << "  " << x << "\t\t";
          for (int y = 0; y < 2; y++) {
               cout << setw(27) << left << t[x][y] << "          ";
          }
          cout << endl << "---------------------------------------------------------------------------------------------" << endl;
     }
     return 0;
}

bool place_in_hash_tables(char*s) {

     bool placed;
     size_t pos;
     int index;
     char temp_s[255], temp[255];

     strcpy(temp_s, s);

     // use a counter to detect loops
     int counter = 0;

     // start with table T1
     index = 0;

     placed = false;

     pos = f(temp_s, index);

     while ((!placed) && (counter < 2 * tablesize)) {
          
          if (strcmp(t[pos][index], "") == 0) {
               // the entry at index <pos> in the <index> hash table is available so store the string <temp_s> there
               cout << "String <" << temp_s << "> will be placed at";
               cout << " t[" << pos << "][" << index << "]" << endl;
               strcpy(t[pos][index], temp_s);
               placed = true;
               return placed;
          }
          else {
               // the entry at index <pos> in the <index> hash table is not available so
               // obtain the string stored over there in variable <temp> and store the string <temp_s> there
               // now the string <temp> needs to be placed in the other table
               cout << "String <" << temp_s << "> will be placed at" << " t[" << pos;
               cout << "][" << index << "]" << " replacing <" << t[pos][index] << ">";
               cout << endl;
               // YOU NEED TO WRITE THE CODE TO STORE IN temp THE STRING STORED AT
               // t[pos][index] AND STORE IN t[pos]index THE STRING temp_s
               strcpy(temp, t[pos][index]);
               strcpy(t[pos][index], temp_s);
               strcpy(temp_s, temp);
               // NOW temp_s CONTAINING THE EVICTED STRING NEEDS TO BE STORED
               // IN THE OTHER TABLE
               // WRITE THE CODE TO SET index TO INDICATE THE OTHER TABLE
               // WRITE THE CODE TO CALCULATE IN pos THE HASH VALUE FOR temp_s
               index = !index;
               pos = f(temp_s, index);
               counter++;
          }
     }
     return placed;
}

size_t f(char *s, size_t index) {
     // compute the hash functions
     // s is the string (the key) to which we apply the hash function
     // index indicates which hash function will be used
     // index == 0 means the first hash function
     // index == 1 means the second hash function
     size_t po, len;
     int i, val, temp;
     po = 1;

     len = strlen(s);
     
     if (index == 0) {

          val = s[0];
          val = val % tablesize;
          if (val < 0) 
               val += tablesize;

          if (len == 1)
               return val;

          for (i = 1; i < len; i++) {
               temp = s[i];
               po *= 31;

               po = po % tablesize;
               if (po < 0)
                    po += tablesize;

               val += temp * po;
               val = val % tablesize;

               if (val < 0)
                    val += tablesize;
          }
          return val;
     }
     else {
          // YOU NEED TO IMPLEMENT THE STEPS TO CALCULATE THE SECOND
          // HASH FUNCTION

          //val = s[0]
          val = s[len - 1];
          val = val % tablesize;
          if (val < 0)
               val += tablesize;

          if (len == 1)
               return val;

          for (i = 1; i < len; i++) {
               temp = s[len - i - 1];
               po *= 31;

               po = po % tablesize;
               if (po < 0)
                    po += tablesize;

               val += temp * po;
               val = val % tablesize;

               if (val < 0)
                    val += tablesize;
          }
          val = val % tablesize;
          return val;
     }
}