#include<algorithm>
#include<iostream>
#include<iterator>
#include<map>
#include<numeric>
#include<set>
#include<string>
#include<vector>

bool isCharInString(char x, std::string cs) {
  return std::any_of(cs.begin(), cs.end(), 
                     [&x](char c) -> bool { return c == x; });
}

template<class a>
using vector = std::vector<a>;

template<class a>
using matrix = vector<vector<a>>;

template<class a>
std::ostream& operator<<(std::ostream& out, std::set<a> s) {
  std::for_each(s.begin(), s.end(), 
		[&](a x) -> void {
		  out << " " << x;
		});

  return out;
}

template<class a>
std::ostream& operator<<(std::ostream& out, vector<a> v) {
  std::for_each(v.begin(), v.end(), 
		[&](a x) -> void {
		  out << " " << x;
		});

  return out;
}

template<class a>
std::ostream& operator<<(std::ostream& out, matrix<a> m) {
  std::for_each(m.begin(), m.end(), 
		[&](vector<a> v) -> void {
		  out << v << std::endl;
		});

  return out;
}


template<class a, class b>
std::ostream& operator<<(std::ostream& out, std::pair<a, b> p) {
  out << p.first;
  out << ", "; 
  out << p.second;

  return out;
}


template<class a, class b>
std::ostream& operator<<(std::ostream& out, std::map<a, b> m) {
  std::for_each(m.begin(), m.end(),
    [&](std::pair<a, b> p) -> void {
      out << p.first; 
      out << ": "; 
      out << p.second;
      out << std::endl;
    }
  );

  return out; 
}


vector<char> rowFromLine(std::string line) {
  vector<char> row;

  int i = 0;
  std::for_each(line.begin(), line.end(),
                [&](char c) -> void {
                  if (isCharInString(c, ".123456789") && i < 9) {
                    row.push_back(c);
                    i += 1;
                  }		  
		});

  return row;
}

matrix<char> readBoard() {
  matrix<char> board;

  std::string line;
  int i = 0;
  while (std::getline(std::cin, line) && i < 9) {
    vector<char> row = rowFromLine(line);
    if (row.size() == 9) {
      board.push_back(row);
      i += 1;
    }
  }

  return board;
}

int countEmpty(vector<char> const& v) {
  return std::accumulate(v.begin(), v.end(), 0, 
      [&](int current, char c) -> int {
        return current + (c == '.' ? 1 : 0);
      });
}

int countEmpty(matrix<char> const& board) {
  return std::accumulate(board.begin(), board.end(), 0, 
      [&](int current, vector<char> const& v) -> int {
        return current + countEmpty(v);
      });
}

void eraseRow(std::set<char>& allowed, matrix<char> const& board, 
    int rownum) {
  for (char c : board[rownum]) allowed.erase(c);
}

void eraseCol(std::set<char>& allowed, matrix<char> const& board, 
    int colnum) {
  for (int i = 0; i < 9; i += 1) {
    char c = board[i][colnum];
    allowed.erase(c);
  }
}

void eraseBlock(std::set<char>& allowed, matrix<char> const& board, 
    int yBlock, int xBlock) {
  int yMin = 0, yMax = 9;
  int xMin = 0, xMax = 9;

       if (yBlock == 0) yMin = 0, yMax = 3;
  else if (yBlock == 1) yMin = 3, yMax = 6;
  else if (yBlock == 2) yMin = 6, yMax = 9;

       if (xBlock == 0) xMin = 0, xMax = 3;
  else if (xBlock == 1) xMin = 3, xMax = 6;
  else if (xBlock == 2) xMin = 6, xMax = 9;

  for (int i = yMin; i < yMax; i += 1) {
    for (int j = xMin; j < xMax; j += 1) {
      allowed.erase(board[i][j]);
    }
  }
}

bool solveBoard(matrix<char> &board) {
  std::map<std::pair<int, int>, std::set<char>> allowed;

  int i = 0;
  for (const vector<char>& row : board) {
    int j = 0;
    for (char c : row) {
      if (c == '.') 
        allowed.insert(
          std::pair<std::pair<int, int>, std::set<char>>{
	    std::pair<int, int>{i, j}, 
	    std::set<char>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}
	  }
	);
      j += 1;
    }
    i += 1;
  }

  int maxRuns = 81;
  int nruns = 0;

  while (countEmpty(board) > 0 && nruns < maxRuns) {
    nruns += 1;

    for (std::pair<std::pair<int, int>, std::set<char>> const& sq : allowed) {
      std::pair<int, int> coords = sq.first;
      int y = coords.first;
      int x = coords.second;

      // while set is not erased, 
      // this prevents the loop from repeating squares 
      if (board[y][x] == '.') {
        std::set<char> &allowed_here = allowed[coords];

	eraseRow(allowed_here, board, y);
        //for (char c : board[y]) allowed_here.erase(c);

	eraseCol(allowed_here, board, x);
        //for (int i = 0; i < 9; i += 1) {
        //  char c = board[i][x];
        //  allowed_here.erase(c);
        //}

	int yBlock = y / 3;
	int xBlock = x / 3;

	eraseBlock(allowed_here, board, yBlock, xBlock);

        if (allowed_here.size() == 1) {
          board[y][x] = *allowed_here.begin();
          // erase mapped set 
        }
      }
    }
  }
  
  std::cout << allowed << std::endl;

  return true;
}

void printBoard(matrix<char> const& board) {
  int yMin = 0, yMax = 3;

  for (int i : vector<int>{1, 2, 3}) {
    std::for_each(board.begin() + yMin, board.begin() + yMax, 
       [&](vector<char> row) -> void {
         int xMin = 0, xMax = 3;
         for (int j = 0; j < 3; j += 1) {
           std::cout << 
	       vector<char>(row.begin() + xMin, row.begin() + xMax);

	   std::cout << " |";

           xMin = xMax;
           xMax += 3;
         }

	 std::cout << std::endl;
       }
    );

//                 6 8 3 | 1 9 7 | 2 5 4 |

    std::cout << " - - - - - - - - - - - -" << std::endl;

    yMin = yMax;
    yMax += 3;
  }
}

int main() {
  matrix<char> board = readBoard();

  std::cout << board << std::endl;

  bool ok = solveBoard(board);

  //std::cout << board << std::endl;

  printBoard(board);

  std::cout << "It compiles!" << std::endl;

  return 0;
}

