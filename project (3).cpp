#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <windows.h> 

using namespace std;

class Point{
public:
    int row;
    int col;
    Point(int row, int col) {
        this->row = row;
        this->col = col;
    }
    Point(){};
};

class Maze{
private:
    int rows;
    int cols;
    int ** board;
public:
    Point start;
    Point end;
    void random_maze();
    void custom_maze();
    void get_dimensions();
    void clearPath();
    bool DFS(int x, int y);
    bool BFS();
    void print_board();
    void choose_start_and_end();
    void random_with_answer();
    bool check_valid();
};

int Menu();
int choose_algorithm();
bool flag = false;

int main(){
    while(1)
    {
        Maze maze = Maze();
        maze.get_dimensions();
        int choice = Menu();
        if (choice == 1)
            maze.custom_maze();
        else if(choice == 2)
            maze.random_maze();
        else
            maze.random_with_answer();
        cout<<"Your maze:\n";
        maze.print_board();
        if(choice != 3)
        {
        	maze.choose_start_and_end();
        	while(!maze.check_valid())
        	{
        		maze.choose_start_and_end();
			}
		}
        while(1)
        {
            int alg = choose_algorithm();
            if(alg == 1)
            {
                if(maze.BFS())
                    maze.print_board();
                else
                    cout<<"BFS can't find answer!";
            }
            else if(alg == 2)
            {
                if(maze.DFS(maze.start.row, maze.start.col))
                    maze.print_board();
                else
                    cout<<"DFS can't find answer!";
            }
            else
                break;
            maze.clearPath();
        }
    }
}

int Menu(){
    int choice;
    while (1){
        cout << "Enter your choice : \n";
        cout << "1. create maze\n2. random maze\n3. random maze with answer\n";
        cin >> choice;
        if(choice == 1 || choice == 2 || choice == 3)
            break;
        else
            cout << "Invalid input !!! ( Just 1 or 2 or 3)\n";
    }
    return choice;
}

bool Maze::check_valid(){
	if(start.row == end.row && start.col == end.col)
	{
		cout<<"start and end are on the same point.\n";
		return false;
	}
	if(board[start.row][start.col] == 1)
	{
		cout<<"Start in on a wall.\n";
		return false;
	}
	if(board[end.row][end.col] == 1)
	{
		cout<<"End in on a wall.\n";
		return false;
	}
	return true;
}


void Maze::get_dimensions(){
    cout << "Enter row dimension : " ;
    cin >> rows;
    while(rows < 2)
    {
    	cout << "Wrong input, rows > 1!\nTry again: " ;
    	cin >> rows;
	}
    cout << "Enter column dimension : " ;
    cin >> cols;
    while(cols < 2)
    {
    	cout << "Wrong input, cols > 1!\nTry again: " ;
    	cin >> rows;
	}
    board = new int *[rows];
    for(int i = 0; i < rows; i++)
        board[i] = new int[cols];


}

void Maze::custom_maze() {
    cout<<"Enter your maze:\n0 = Empty\n1 = Wall\n";
    for(int i = 0; i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {
            char c = getchar();
            while(c == '\n' || c == ' ')
                c = getchar();
            board[i][j] = c - '0';
        }
    }
    cout<<"Your board: \n";
}

void Maze::print_board() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
    for (int i = 0 ; i < cols; i++)
        cout << " _";
    cout << endl;
    for (int i = 0 ; i < rows ; i++){
        cout << "|";
        for (int j = 0 ; j < cols ; j++){
            if(board[i][j] == 1)
                cout << "& ";
            else if(board[i][j] == 2)
            {
            	SetConsoleTextAttribute(hConsole, 12);
            	cout << "* ";
            	SetConsoleTextAttribute(hConsole, 15);
			}
            else
                cout << "  ";
        }
        cout << "|" << endl;
    }
    for (int i = 0 ; i < rows; i++)
        cout << " -";
    cout << endl;
}


int choose_algorithm(){
    int ch;
    while(true){
        cout << "Using BFS or DFS to find the rout ?\n";
        cout << "1. BFS 	2. DFS   3. Back\n";
        cin >> ch;
        if(ch >= 1 && ch <= 3)
            return ch;
        else
            cout << "Only 1 or 2 or 3!!!!\n";
    }
}
bool Maze::DFS(int x, int y){
    if (x >= 0 && x < rows && y >= 0 && y < cols){ //Not exited
        if (x == end.row && y == end.col){
            board[x][y] = 2;
            print_board();
            return true;
        }
        if (board[x][y] == 0) {
            board[x][y] = 3; //Visited
            if (!DFS(x + 1, y)) {
                if (!DFS(x, y + 1)) {
                    if (!DFS(x - 1, y)) {
                        if (!DFS(x, y - 1)) {
                            return false;
                        }
                    }
                }
            }
            board[x][y] = 2;
            print_board();
            return true;
        }
    }
    return false;
}

bool Maze::BFS(){
    queue<Point> q;
    q.push(start);
    char parents[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(board[i][j] != 1)
                parents[i][j] = ' ';
            else
                parents[i][j] = 'X';
        }
    }
    parents[start.row][start.col] = 'S';
    while(!q.empty())
    {
        Point point = q.front();
        q.pop();
        if(point.row == end.row && point.col == end.col)
        {
            int x = end.row,y = end.col;
            while(x != start.row || y != start.col)
            {
                board[x][y] = 2;
                if(parents[x][y] == 'D')
                    x++;
                else if(parents[x][y] == 'U')
                    x--;
                else if(parents[x][y] == 'R')
                    y++;
                else if(parents[x][y] == 'L')
                    y--;
                print_board();
            }
            board[x][y] = 2;
            return true;
        }
        //Right
        if(point.col + 1 != cols && parents[point.row][point.col + 1] == ' ')
        {
            q.push(Point(point.row, point.col + 1));
            parents[point.row][point.col + 1] = 'L';
        }
        //Left
        if(point.col != 0 && parents[point.row][point.col - 1] == ' ')
        {
            q.push(Point(point.row, point.col - 1));
            parents[point.row][point.col - 1] = 'R';
        }
        //UP
        if(point.row + 1 != rows && parents[point.row + 1][point.col] == ' ')
        {
            q.push(Point(point.row + 1, point.col));
            parents[point.row + 1][point.col] = 'U';
        }
        //DOWN
        if(point.row != 0 && parents[point.row - 1][point.col] == ' ')
        {
            q.push(Point(point.row - 1, point.col));
            parents[point.row - 1][point.col] = 'D';
        }
    }
    return false;
}

void Maze::choose_start_and_end() {
    start.row = -1;
    start.col = -1;
    end.row = -1;
    end.col = -1;

    while(start.row < 0 || start.row >= cols) {
        cout << "Enter start Row:";
        cin >> start.row;
    }
    while(start.col < 0 || start.col >= cols) {
        cout<<"Enter start Column:";
        cin >> start.col;
    }

    while(end.row < 0 || end.row >= cols) {
        cout << "Enter end Row:";
        cin >> end.row;
    }
    while(end.col < 0 || end.col >= cols) {
        cout<<"Enter end Column:";
        cin >> end.col;
    }
    

}

void Maze::clearPath() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if(board[i][j] == 2 || board[i][j] == 3)
                board[i][j] = 0;
        }
    }

}

void Maze::random_maze() {
    srand (time(NULL));
    for (int i = 0; i < rows; i++){
        for (int j = 0 ; j < cols; j++){
            if (rand() % 2 == 0)
                board[i][j] = 0;
            else
                board[i][j] = 1;
        }
    }
}

void Maze::random_with_answer() {
    start.row = 0;
    start.col = 0;
    end.row = rows-1;
    end.col = cols-1;
    srand (time(NULL));
    for (int i = 0; i < rows; i++){
        for (int j = 0 ; j < cols; j++){
            if (rand() % 3 == 0)
                board[i][j] = 0;
            else
                board[i][j] = 1;
        }
    }

    int x = start.row;
    int y = start.col;
    board[x][y] = 0;
    while(x != rows-1 || y != cols-1)
    {
        if(y == cols -1)
        {
            x++;
        }
        else if(x == rows-1)
        {
            y++;
        }
        else if(rand()%2 == 0)
        {
        	x++;
		}
		else
		{
			y++;
		}
        board[x][y] = 0;
    }

}

