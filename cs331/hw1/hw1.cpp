#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include <cmath>
#include <iomanip>
#include <fstream>

class GameState {
	public:     
        class PtrComp {
            int max;
            public:
                PtrComp(int maxArg) { max = maxArg; }
                bool operator() (GameState *left, GameState *right) const {
                    double lt = 0;
                    double rt = 0;
                    for(int i = 0; i < 3; i++) {
                        lt += left->LBank[i] * pow(max,i);
                        rt += right->LBank[i] * pow(max,i);
                    }
                    return lt > rt; 
                }
        };


        class SearchToggleComp {
            int key;
            
            public:
                SearchToggleComp(int keyArg) {
                    key = keyArg;
                }

                bool operator() (GameState *left, GameState *right) {
                    if (key == 0) return true; // Makes this a stack
                    if (key == 1) return false; // Makes this a queue
                    
                    // Otherwise return a comparison of path costs?
                    // This will make it a priority_queue
                    return (left->pathCost + left->RBank[0] + left->RBank[1] - 1) > (right->pathCost + right->RBank[0] + right->RBank[1] - 1);

                }

        };
        typedef std::priority_queue<GameState*,std::vector<GameState*>,SearchToggleComp> Fringe;

		int LBank[3]; //state
		int RBank[3]; //state
		int action[2];
		GameState *parent; //parentNode
		int pathCost; //pathcost of parent + stepcost
		int depth; //depth
		
        bool operator== (GameState other) {
            bool equals = true;
            
            equals = equals && (LBank[1] == other.LBank[1]) && (RBank[1] == other.RBank[1]);
            equals = equals && (LBank[2] == other.LBank[2]) && (RBank[2] == other.RBank[2]);
            equals = equals && (LBank[3] == other.LBank[3]) && (RBank[3] == other.RBank[3]);

            return equals;
        }

		static bool isValid(GameState *state, int *argAction) { // No negative numbers, m >= c everywhere
			bool valid = true;

			int leftMis = state->LBank[0];
			int leftCan = state->LBank[1];
			int rightMis= state->RBank[0];
			int rightCan= state->RBank[1];
			int boatMis = argAction[0];
			int boatCan = argAction[1];
			bool boatOnLeft  = state->LBank[2] == 1;
			bool boatOnRight = state->RBank[2] == 1;

			if (boatOnLeft) {
				if ((leftMis - boatMis < 0) || (leftCan - boatCan < 0)) {
					valid = false;
				}
				if ((leftMis - boatMis < leftCan - boatCan) && (leftMis - boatMis != 0)) {
					valid = false; 

				}
				if ((rightMis + boatMis < rightCan + boatCan) && (rightMis + boatMis != 0)) {
					valid = false;
				}
			}
			else {
				if ((rightMis - boatMis < 0) || (rightCan - boatCan < 0)) {
					valid = false;
				}
				if ((rightMis - boatMis < rightCan - boatCan) && (rightMis - boatMis != 0)) {
					valid = false;
				}
				if ((leftMis + boatMis < leftCan + boatCan) && (leftMis + boatMis != 0)) {
					valid = false;
				}
			}

			return valid;
		}


		static void expand(GameState *node, GameState::Fringe *fringe) { // Does this need the goal state or some other "problem" state thing?
			int actions[5][2] = {{1,0},{2,0},{0,1},{1,1},{0,2}};
		
			for (int i = 0; i < 5; i++) {	
				if (isValid(node, actions[i])) {
					fringe->push(new GameState(node,actions[i])); // Memory leak. Oh God memory leak. o_o
                }
			}			
			

		}



		
		GameState(int lm, int lc, int lb, int rm, int rc, int rb) { 
			LBank[0] = lm; LBank[1] = lc; LBank[2] = lb;
			RBank[0] = rm; RBank[1] = rc; RBank[2] = rb;
			action[0] = 0; action[1] = 0;
			pathCost = 0;
			depth = 0;
            parent = NULL;
		}
		GameState(GameState *argParent, int *argAction) {
			int stepCost = 1; // Or might this be a float? Probably not.
			if (argParent->LBank[2] == 1) {
				LBank[0] = argParent->LBank[0] - argAction[0];
				LBank[1] = argParent->LBank[1] - argAction[1];
				LBank[2] = 0;
				RBank[0] = argParent->RBank[0] + argAction[0];
				RBank[1] = argParent->RBank[1] + argAction[1];
				RBank[2] = 1;
				// Calculate stepCost here? Would we then need to pass in heuristics?
			}
			else {
				RBank[0] = argParent->RBank[0] - argAction[0];
				RBank[1] = argParent->RBank[1] - argAction[1];
				RBank[2] = 0;
				LBank[0] = argParent->LBank[0] + argAction[0];
				LBank[1] = argParent->LBank[1] + argAction[1];
				LBank[2] = 1;
			}

			parent = argParent;
			action[0] = argAction[0];
			action[1] = argAction[1];
			depth = argParent->depth + 1;
			pathCost = argParent->pathCost + stepCost;
		}

		// Things to make this a full node:
		// Step-Cost (function)

		// A show or print function might be useful as well.
		// Need a better show function...
		// Also, we need a close list and heuristic stuffs, and other things of the sort. IE: graphSearch ^_^

		void showStuffs(std::ofstream *out) {
			// Note: this is temporary.
		
            (*out) << "Move " << std::setw(4) << pathCost << ": ";
            (*out) << "Left: " << std::setw(3) << LBank[0] << " " << std::setw(3) << LBank[1] << " " << LBank[2] << " ";
            (*out) << "Right: "<< std::setw(3) << RBank[0] << " " << std::setw(3) << RBank[1] << " " << RBank[2] << " ";
            (*out) << "Action: "<< action[0] << " " << action[1] << std::endl;

		}

}; 


        class PtrComp {

            public:
                bool operator() (GameState *left, GameState *right) {
                    return (*left) == (*right);
                }
        };


GameState *GraphSearch(GameState *initial, GameState *goal, GameState::Fringe *fringe, int *expandedNodes, bool iterativeDeepening) {
    // Somewhere in here we need to keep track of expanded nodes, right?
	GameState *solution;

	std::set<GameState*,GameState::PtrComp> close(initial->LBank[0] + initial->RBank[0] + 1); // Can't get hash to work, so this will do for now.

	fringe->push(initial);

	GameState *node;
    std::set<GameState*>::iterator found;

    int iterativeDepth = 0;

	while(true) { // change this later?
        if (fringe->empty()) {
            return NULL; 
		}
		
		node = fringe->top(); fringe->pop();

        if (iterativeDeepening && node->pathCost >= iterativeDepth) {
            iterativeDepth++;
            close.clear();
            fringe->push(initial);
            continue;
        }


		if (*node == *goal) {
			return node;
		}

        (*expandedNodes)++;
        
        found = close.find(node);
		if (found != close.end() && (*found)->pathCost > node->pathCost) {
            close.erase(found);
        }

        if (close.count(node) == 0) { // Need to add in checking path cost to break ties, right?
			close.insert(node);
			GameState::expand(node,fringe);
		}

    }
	return solution;

}




int main(int argc, char *args[]) {

    int initArray[6];
    int goalArray[6];
    std::ifstream initFile;
    std::ifstream goalFile;
    
    initFile.open(args[1]);
    goalFile.open(args[2]);
   
    for (int i = 0; i < 6; i++) {
        initFile >> initArray[i];
        goalFile >> goalArray[i];
        initFile.get();
        goalFile.get();
    }
    
    initFile.close();
    goalFile.close();

	GameState initial(initArray[0],initArray[1],initArray[2],initArray[3],initArray[4],initArray[5]);
    GameState goal(goalArray[0],goalArray[1],goalArray[2],goalArray[3],goalArray[4],goalArray[5]);
    
    
    GameState *solution;
    int expandedNodes = 0;	
    GameState::Fringe dfs_fringe(0);
    GameState::Fringe bfs_fringe(1);
    GameState::Fringe iddfs_fringe(0);
    GameState::Fringe astar_fringe(2);


    if (args[3][0] == 'd') {
        solution = GraphSearch(&initial,&goal,&dfs_fringe,&expandedNodes,false);
    } 
    else if (args[3][0] == 'b') {
        solution = GraphSearch(&initial,&goal,&bfs_fringe,&expandedNodes,false);
    } 
    else if (args[3][0] == 'i') {
        solution = GraphSearch(&initial,&goal,&iddfs_fringe,&expandedNodes,true);
    } 
    else {
        solution = GraphSearch(&initial,&goal,&astar_fringe,&expandedNodes,false);
    } 
    
    if(solution){
        std::ofstream outputfile;
        outputfile.open(args[4]);
        solution->showStuffs(&outputfile);
        while(solution->parent) {
            solution = solution->parent;
            solution->showStuffs(&outputfile);
        }
        outputfile << "Expanded Nodes: " << expandedNodes << std::endl;
        outputfile.close();
    }
    else {
        std::cout<< "No solution\n";
        return 1;
    }
	return 0;

}


