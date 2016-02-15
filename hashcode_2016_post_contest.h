#ifndef HASHCODE_2016_POST_CONTEST_H_INCLUDED
#define HASHCODE_2016_POST_CONTEST_H_INCLUDED

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>

struct pos {

    int x, y;
    pos( int pos_x, int pos_y ) : x( pos_x ), y( pos_y ) {}
};

void parse();
void freeArray();
void processDroneTurn();
void removeItemFromOrder( int orderID, int itemID );
void removeItemFromWarehouse( int warehouseID, int itemID );
void printResult( std::string fileName );
void updateCurrentOrderList();

int getDistance( pos a, pos b );
int getNearestOrder( int droneID );
int getNearestWarehouse( int droneID, int orderID );

bool warehouseContainOrder( int orderID, int warehouseID );
bool isOrderEmpty( int orderID );
bool isOrderInProgress( int orderID );
bool isAllOrderEmpty();

std::string getLoadCmd( int droneID, int warehouseID, int itemID, int nb );
std::string getUnloadCmd( int droneID, int warehouseID, int itemID, int nb );
std::string getDeliverCmd( int droneID, int orderID, int itemID, int nb );
std::string getWaitCmd( int droneID, int turn );

int getAvailableItemFromOrder( int orderID, int warehouseID, int droneSpace );

#endif // HASHCODE_2016_POST_CONTEST_H_INCLUDED
