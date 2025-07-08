#include <iostream>
#include <deque>
#include <set>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <tuple>
#include <termios.h>
#include <fcntl.h>


using namespace std;

const int WIDTH = 20;
const int HEIGHT = 17;

enum Direction {STOP = 0,LEFT,RIGHT,UP,DOWN};

struct Position{
	int x, y;
	//Assignment
	bool operator==(const Position& other) const{
		return x == other.x && y == other.y;
	}
	//Comparison
	bool operator<(const Position& other) const{
		return tie(x,y) < tie(other.x, other.y);
	}
};

// The below function enables or disables non-blocking input mode for the terminal
void setNonBlocking(bool enable) {
    //'oldt' stores the original terminal settings (only initialized once)
    static struct termios oldt, newt;

    if (enable) {
    	//Get current terminal settings and store in 'oldt'
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt; // Copying current settings to 'newt' and modify them
        newt.c_lflag &= ~(ICANON | ECHO); // disable buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking read
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore settings
    }
}

char getInput() {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) > 0) {
        return ch;
    }
    return 0;
}

class SnakeGame{
private:
	bool gameOver;
	Direction dir;
	Position head, fruit;
	int score;
	deque<Position> snake; // tail and head together
	set<Position> snakeSet; // O(1) for collision check
public:
	SnakeGame(){
           srand(time(0));
           setup();
	}
	
	void setup(){
	   gameOver = false;
	   dir = STOP;
	   head = {WIDTH/2,HEIGHT/2};
	   fruit = {rand() % WIDTH, rand() % HEIGHT};
	   score = 0;
	   snake.clear();
	   snake.push_back(head);
	   snakeSet.clear();
	   snakeSet.insert(head);
	}
	
	void draw(){
	   system("clear");
	   
	   for(int i=0; i < WIDTH+2; i++) cout << "#"; // Draws the top boundary 
	   cout << endl;
	   
	   for(int i=0; i < HEIGHT; i++){
	   	for(int j=0; j < WIDTH; j++){
		     if(j == 0) cout << "#"; // Draws the left wall at the start of each row
		     
		     Position current = {j,i};
		     
		     if(current == head) cout << "0"; // If it's the snake's head, draw 0
		     else if (current == fruit) cout << "F"; // If it's the fruit , draw F
		     else if (snakeSet.find(current) != snakeSet.end()) cout << "o"; // if the position is part of the snake's body, draw 'o'
		     else cout << " ";
		     
		     if(j == WIDTH-1) cout << "#";//draws the right wall at the end of each row
		}
		cout << endl;
	   }
	   
	   for(int i=0; i < WIDTH+2; i++) cout << "#";
	   cout << endl;
	  
	   cout << "Score: " << score << endl;
	   cout << "Controls: W A S D to move | X to exit" << endl;
	}
	
/*	void input(){
	   char key;
	   cin >> key;
	   switch(key){
	   	case 'a' : case 'A': if(dir != RIGHT) dir = LEFT; break;
	   	case 'd' : case 'D': if(dir != LEFT) dir = RIGHT; break;
	   	case 'w' : case 'W': if(dir != DOWN)  dir = UP; break;
	   	case 's' : case 'S': if(dir != UP)  dir = DOWN; break;
	   	case 'x' : case 'X': gameOver = true; break; // Pressing X exits the game by setting gameOver = true;
	   }
	} */
	
	void logic(){
	   Position newHead = head;
	   // Creates a new position (newHead) based on the current direction of movement
	   // The below switch statement updates (newHead) depending on the value of dir
	   switch(dir){
	   	case LEFT: newHead.x--; break;
	   	case RIGHT: newHead.x++; break;
	   	case UP: newHead.y--; break;
	   	case DOWN: newHead.y++; break;
	   	default: return;
	   }
	   //Wrap around when the snake hits a wall 
	   if(newHead.x >= WIDTH) newHead.x = 0;
	   else if(newHead.x < 0) newHead.x = WIDTH - 1;
	   
	   if(newHead.y >= HEIGHT) newHead.y = 0;
	   else if (newHead.y < 0) newHead.y = HEIGHT - 1;
	  	
	   //Collision with itself
	   // Here if the snake position is already there in the snakeSet, that means snake hit itself
	   if(snakeSet.count(newHead)){
	   	gameOver = true;
	   	return;
	   }
	   
	   //Move the snake
	   snake.push_front(newHead); // Here we add the newHead to the front of the snake(using the deque)
	   snakeSet.insert(newHead); // Here we also updating the snakeSet to include this position
	   head = newHead; // Updating the new head
	   
	   //Fruit COllision
	   // Here the do while loop ensures that the fruit doesn't spawn on the snake's body
	   if(head == fruit){
	   	score += 10;
	   	do{
	   	   fruit = {rand() % WIDTH, rand() % HEIGHT};
	   	} while(snakeSet.count(fruit));
	   }
	   else{
	   	Position tail = snake.back();
	   	snake.pop_back();
	   	snakeSet.erase(tail);
	   }
	   // Here on top , if the snake didn't eat the fruit we remove the last segment(tail) from both deque and set
	   //This makes snake appear to move forward without growing.
	}
	
	void run(){
	   setNonBlocking(true);
	   
	   while(!gameOver){
	   	draw();
	   	
	   	char key = getInput();
	   	if(key){
		   switch(key){
                    case 'a': case 'A': if (dir != RIGHT) dir = LEFT; break;
                    case 'd': case 'D': if (dir != LEFT)  dir = RIGHT; break;
                    case 'w': case 'W': if (dir != DOWN)  dir = UP; break;
                    case 's': case 'S': if (dir != UP)    dir = DOWN; break;
                    case 'x': case 'X': gameOver = true; break;
		   }		
		}
	   	logic();
	   	usleep(100000); // 100ms delay , Using this we control the speed of the loop 
	   }		
	   setNonBlocking(false);	   
	   cout << "Game Over! Final Score: " << score << endl;
	}
};

int main(){
	SnakeGame game;
	game.run();
	return 0;
}

