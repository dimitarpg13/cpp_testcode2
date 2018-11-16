#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

// directed graph structure
struct Node {
  std::unordered_map<int,std::shared_ptr<Node>> pred_;
  std::unordered_map<int,std::shared_ptr<Node>> succ_; 
  int score_;
};

template <int N>
bool constructGraph(const int adjMatrix[N][N], std::vector<std::shared_ptr<Node>>& graph) {
   graph.resize(N,std::make_shared<Node>()); 
   for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
         if (adjMatrix[i][j] == 1) {
            graph[i]->succ_.insert(std::make_pair(j,graph[j]));
            graph[j]->pred_.insert(std::make_pair(i,graph[i]));
         }
      }
   } 
   
   return true;
}

int main(int argc, char * argv[]) {
   const int vertCount = 10;
   int  adjMatrix[vertCount][vertCount] = { 
      { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    };

   std::vector<std::shared_ptr<Node>> graph; 
   bool res = constructGraph(adjMatrix, graph);


   return 0;
}

