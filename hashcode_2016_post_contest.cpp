#include "hashcode_2016_post_contest.h"
#define FILE_ID 2

using namespace std;

const string inputFiles[] = { "mother_of_all_warehouses.in", "busy_day.in", "redundancy.in" };

int row, col, droneNB, maxTurn, maxPayload, productNB, warehouseNB, orderNB;

int *productWg, *orderLen, *droneAct;
int **warehouseStock;
pos *warehousePos, *orderPos, *dronePos;
vector<vector<int>> orderItem;

vector<int> currentOrder( 10 );

vector<string> output;

int main() {

    parse();

    bool finish = false;
    for ( int turn = 0; turn < maxTurn && !finish; turn++ ) {
        for ( int d = 0; d < droneNB && !finish; d++ ) {
            if ( droneAct[d] == 0 ) {

                int orderID = getNearestOrder( d );
                int warehouseID = getNearestWarehouse( d, orderID );

                int droneSpace = maxPayload;
                vector<int> items;
                int currentItem = getAvailableItemFromOrder( orderID, warehouseID, droneSpace );
                while ( currentItem != -1 ) {
                    items.push_back( currentItem );
                    removeItemFromOrder( orderID, currentItem );
                    removeItemFromWarehouse( warehouseID, currentItem );
                    droneSpace -= productWg[currentItem];
                    output.push_back( getLoadCmd( d, warehouseID, currentItem, 1 ) );
                    currentItem = getAvailableItemFromOrder( orderID, warehouseID, droneSpace );
                }

                for ( unsigned i = 0; i < items.size(); i++ )
                    output.push_back( getDeliverCmd( d, orderID, items[i], 1 ) );

                dronePos[d] = orderPos[orderID];

                droneAct[d] += getDistance( dronePos[d], warehousePos[warehouseID] );
                droneAct[d] += getDistance( warehousePos[warehouseID], orderPos[orderID] );

                finish = isAllOrderEmpty();
            }
        }

        processDroneTurn();

    }

    freeArray();

    printResult( "_" + inputFiles[FILE_ID].substr( 0, inputFiles[FILE_ID].size() - 3 ) +  ".out" );

}

void parse() {

    ifstream in( inputFiles[FILE_ID].c_str() );

    in >> row >> col >> droneNB >> maxTurn >> maxPayload;

    droneAct = ( int*) calloc( droneNB, sizeof( int ) );
    dronePos = ( pos* ) malloc( droneNB * sizeof( pos ) );
    for ( int i = 0; i < droneNB; i++ )
        dronePos[i] = pos( 0, 0 );

    in >> productNB;

    productWg = ( int* ) malloc( productNB * sizeof( int ) );

    for ( int i = 0; i < productNB; i++ )
        in >> productWg[i];

    in >> warehouseNB;
    warehousePos = ( pos* ) malloc( warehouseNB * sizeof( pos ) );
    warehouseStock = ( int** ) calloc( warehouseNB, sizeof( int* ) );
    for ( int i = 0; i < warehouseNB; i++ ) {
        int x, y;
        in >> x >> y;
        warehousePos[i] = pos( x, y );

        warehouseStock[i] = ( int* ) calloc( productNB, sizeof( int ) );
        for ( int u = 0; u < productNB; u++ )
            in >> warehouseStock[i][u];
    }

    in >> orderNB;
    orderPos = ( pos* ) malloc( orderNB * sizeof( pos ) );
    orderLen = ( int* ) calloc( orderNB, sizeof( int ) );
    orderItem.resize( orderNB );
    for ( int i = 0; i < orderNB; i++ ) {
        int x, y;
        in >> x >> y;
        orderPos[i] = pos ( x, y );

        int l;
        in >> l;
        orderLen[i] = l;

        vector<int> vec( l );
        for ( int u = 0; u < l; u++ )
            in >> vec[u];

        orderItem[i] = vec;
    }

    in.close();
}

void freeArray() {

    free( productWg );
    free( warehousePos );
    for ( int i = 0; i < warehouseNB; i++ )
        free( warehouseStock[i] );
    free( warehouseStock );
    free( orderPos );
    free( orderLen );
    free( droneAct );
    free( dronePos );

}

void processDroneTurn() {

    for ( int i = 0; i < droneNB; i++ ) {
        if ( droneAct[i] > 0 )
            droneAct[i]--;
    }
}

int getNearestOrder( int droneID ) {

    int distance = row * col, orderID = -1;
    for ( int o = 0; o < orderNB; o++ ) {
        int dt = getDistance( dronePos[droneID], orderPos[o] );
        if ( dt < distance && !isOrderEmpty( o ) ) {
            distance = dt;
            orderID = o;
        }
    }
    return orderID;
}

int getDistance( pos a, pos b ) {

    int c = abs( a.x - b.x ), d = abs( a.y - b.y );
    return sqrt( c * c + d * d );
}

bool isOrderEmpty( int orderID ) {

    for ( int i = 0; i < orderLen[orderID]; i++ ) {
        if( orderItem[orderID][i] != -1 )
            return false;
    }
    return true;
}

void removeItemFromOrder( int orderID, int itemID ) {

    for ( int p = 0; p < orderLen[orderID]; p++ ) {
        if ( orderItem[orderID][p] == itemID ) {
            orderItem[orderID][p] = -1;
            return;
        }
    }

    cout << "ERROR : " << orderID << " : " << itemID << endl;
}

bool isAllOrderEmpty() {

    for ( int o = 0; o < orderNB; o++ ) {
        if ( !isOrderEmpty(o) )
            return false;
    }
    return true;
}

int getNearestWarehouse( int droneID, int orderID ) {

    int distance = row * col, warehouseID = -1;
    for ( int w = 0; w < warehouseNB; w++ ) {
        int dt = getDistance( dronePos[droneID], warehousePos[w] );
        if ( dt < distance && warehouseContainOrder( orderID, w ) ) {
            distance = dt;
            warehouseID = w;
        }
    }
    return warehouseID;
}

bool warehouseContainOrder( int orderID, int warehouseID ) {

    for ( int i = 0; i < orderLen[orderID]; i++ ) {
        if ( orderItem[orderID][i] != -1
            && warehouseStock[warehouseID][orderItem[orderID][i]] > 0 )
            return true;
    }
    return false;
}

string getLoadCmd( int droneID, int warehouseID, int itemID, int nb ) {
    return to_string( droneID ) + " L " + to_string( warehouseID ) + " "
    + to_string( itemID ) + " " + to_string( nb );
}

string getUnloadCmd( int droneID, int warehouseID, int itemID, int nb ) {
    return to_string( droneID ) + " U " + to_string( warehouseID ) + " "
    + to_string( itemID ) + " " + to_string( nb );
}

string getDeliverCmd( int droneID, int orderID, int itemID, int nb ) {
    return to_string( droneID ) + " D " + to_string( orderID ) + " "
    + to_string( itemID ) + " " + to_string(nb);
}

string getWaitCmd( int droneID, int turn ) {
    return to_string( droneID ) + " W " + to_string( turn );
}

void printResult( string fileName ) {

    ofstream out( fileName.c_str() );
    out << output.size() << endl;
    ostream_iterator<string> output_iterator( out, "\n" );
    copy( output.begin(), output.end(), output_iterator );
    out.close();

}

void removeItemFromWarehouse( int warehouseID, int itemID ) {

    if ( warehouseStock[warehouseID][itemID] < 1)
        cout << "ERROR" << endl;
    else
        warehouseStock[warehouseID][itemID]--;
}

int getAvailableItemFromOrder( int orderID, int warehouseID, int droneSpace ) {

    for ( int o = 0; o < orderLen[orderID]; o++ ) {
        if ( orderItem[orderID][o] != -1
            && warehouseStock[warehouseID][orderItem[orderID][o]] > 0
            && productWg[orderItem[orderID][o]] <= droneSpace )
                    return orderItem[orderID][o];
    }
    return -1;
}

bool isOrderInProgress( int orderID ) {

    if ( isOrderEmpty( orderID ) || orderLen[orderID] == 1 )
        return false;
    bool itemSend = orderItem[orderID][0] == -1;
    for ( int o = 1; o < orderLen[orderID]; o++ ) {
        //if ( orderItem[orderID][o] )
    }
}

void updateCurrentOrderList() {

    currentOrder.erase( remove_if( currentOrder.begin(), currentOrder.end(),[]( int o ) {
                                  return isOrderEmpty( o );
                                } ), currentOrder.end() );
}

