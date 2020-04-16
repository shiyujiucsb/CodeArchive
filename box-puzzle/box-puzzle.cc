
#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <vector>

namespace pushbox{

    static constexpr int dx[4] = {1, 0, 0, -1};
    static constexpr int dy[4] = {0, 1, -1, 0};
    
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::map<std::vector<int>, int> mm;
    
    bool Check(const std::vector<std::vector<char>>& grid,
               int x, int y, int Bx, int By) {
        const int M = grid.size();
        const int N = grid[0].size();
        if (x == Bx && y == By) return false;
        if (x < 0 || y < 0 || x >= M || y >= N) return false;
        return grid[x][y] == ' ';
    }
    
    bool IsConnected(const std::vector<std::vector<char>>& grid,
                     int Sx, int Sy, int Dx, int Dy, int Bx, int By) {
        if (Sx == Bx && Sy == By) return false;
        if (Sx == Dx && Sy == Dy) return true;
        const int M = grid.size();
        const int N = grid[0].size();
	std::vector<std::vector<bool>> vis(M, std::vector<bool>(N));
	std::queue<std::vector<int>> q;
        q.push({Sx, Sy});
        vis[Sx][Sy] = true;
        while (q.empty() == false) {
            std::vector<int> top = q.front();
            q.pop();
            const int x = top[0];
            const int y = top[1];
            if (x == Dx && y == Dy) return true;
            for (int i = 0; i < 4; i++) {
                if (Check(grid, x+dx[i], y+dy[i], Bx, By) && vis[x+dx[i]][y+dy[i]] == false) {
                    vis[x+dx[i]][y+dy[i]] = true;
                    q.push({x+dx[i], y+dy[i]});
                }
            }
        }
        return false;
    }
    
    int Solver(const std::vector<std::vector<char>>& grid,
               int Sx, int Sy, int Bx, int By, int Tx, int Ty) {
        if (Tx == Bx && Ty == By) return 0;
        assert (mm.count({Bx, By, Sx, Sy}) == 0);
        mm[{Bx, By, Sx, Sy}] = INT_MAX;
        int ans = INT_MAX;
        for (int i = 0; i < 4; i++) {
            if (Check(grid, Bx+dx[i], By+dy[i], Bx, By) == false) continue;
            if (Check(grid, Bx-dx[i], By-dy[i], Bx, By) == false) continue;
            int subans = INT_MAX;
            if (IsConnected(grid, Sx, Sy, Bx-dx[i], By-dy[i], Bx, By)) {
                if (mm.count({Bx+dx[i], By+dy[i], Bx, By})) {
                    subans = mm[{Bx+dx[i], By+dy[i], Bx, By}];  
                    if (subans == INT_MAX) continue;
                } else {
                    subans = Solver(grid, Bx, By, Bx+dx[i], By+dy[i], Tx, Ty);
                }
            }
            if (subans < INT_MAX && subans + 1 < ans) {
                ans = subans+1;
            }
        }
        mm[{Bx, By, Sx, Sy}] = ans;
        return ans;
    }
    
    int MinPushBox(std::vector<std::vector<char>>& puzzle) {
	auto grid = puzzle;
        const int M = grid.size();
        const int N = grid[0].size();
        int Tx = 0, Ty = 0;
        int Sx = 0, Sy = 0;
        int Bx = 0, By = 0;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 'T') {
                    Tx = i; Ty = j;
                    grid[i][j] = ' ';
                }
                if (grid[i][j] == 'S') {
                    Sx = i; Sy = j;
                    grid[i][j] = ' ';
                }
                if (grid[i][j] == 'B') {
                    Bx = i; By = j;
                    grid[i][j] = ' ';
                }
            }
        }
        int ans = Solver(grid, Sx, Sy, Bx, By, Tx, Ty);
        return ans;
    }

    std::vector<std::vector<char>> GeneratePuzzle(int n_rows, int n_cols,
                                                 double wall_ratio) {
        assert(n_rows > 0);
        assert(n_cols > 0);
        assert(wall_ratio > 0 && wall_ratio < 1.0);
        
	std::vector<std::vector<char>> ans(n_rows, std::vector<char>(n_cols));
	for (int i = 0; i < n_rows; i++) {
	  for (int j = 0; j < n_cols; j++) {
	    if (distribution(generator) < wall_ratio) {
	      ans[i][j] = '#';
	    } else {
	      ans[i][j] = ' ';
	    }
	  }
	}
        // Initialze S position.
	const int Sx = static_cast<int>(n_rows * distribution(generator));
	const int Sy = static_cast<int>(n_cols * distribution(generator));
	ans[Sx][Sy] = 'S';
	// Initialize B position.
	int Bx = 0, By = 0;
	do {
          Bx = static_cast<int>(n_rows * distribution(generator));
          By = static_cast<int>(n_cols * distribution(generator));
	} while (ans[Bx][By] == 'S');
	ans[Bx][By] = 'B';
	// Initialize T position.
        int Tx = 0, Ty = 0;
        do {
          Tx = static_cast<int>(n_rows * distribution(generator));
          Ty = static_cast<int>(n_cols * distribution(generator));
        } while (ans[Tx][Ty] == 'S' || ans[Tx][Ty] == 'B');
	ans[Tx][Ty] = 'T';
        
	return ans;
    }

    void PrintPuzzle(const std::vector<std::vector<char>>& grid) {
      for (const std::vector<char>& row : grid) {
        for (char c : row) {
	  std::cout << c;
	}
	std::cout << std::endl;
      }
    }

} // namespace pushbox

int main(int argc, char **argv) {
  const int n_rows = 9;
  const int n_cols = 16;
  const double wall_ratio = 0.5;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  pushbox::generator = std::default_random_engine(seed);

  while(true) {
    std::vector<std::vector<char>> puzzle = pushbox::GeneratePuzzle(
		    n_rows, n_cols, wall_ratio);
    if (pushbox::MinPushBox(puzzle) < INT_MAX) {
      pushbox::PrintPuzzle(puzzle);
      break;
    } else {
      pushbox::mm.clear();
    }
  }

  return 0;
}

