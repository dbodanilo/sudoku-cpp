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
using matrix = std::vector<std::vector<a>>;

template<class k, class v>
using map2d = std::map<k, std::map<k, v>>;

template<class a>
bool operator==(vector<a> const& xs, vector<a> const& ys) {
  if (xs.size() != ys.size()) return false;

  for (size_t i = 0; i < xs.size(); i += 1) {
    if (xs[i] != ys[i]) return false;
  }

  return true;
}

template<class a>
bool operator!=(vector<a> const& xs, vector<a> const& ys) {
  return !(xs == ys);
}

template<class a>
std::ostream& operator<<(std::ostream& out, std::set<a> const& s) {
  std::for_each(s.begin(), s.end(), 
		[&](a x) -> void {
		  out << " " << x;
		});

  return out;
}

template<class a>
std::ostream& operator<<(std::ostream& out, std::set<a>* sp) {
  out << *sp;

  return out; 
}

template<class a>
std::ostream& operator<<(std::ostream& out, vector<a> const& v) {
  std::for_each(v.begin(), v.end(), 
		[&](a x) -> void {
		  out << " " << x;
		});

  return out;
}

template<class a>
std::ostream& operator<<(std::ostream& out, matrix<a> const& m) {
  std::for_each(m.begin(), m.end(), 
		[&](vector<a> v) -> void {
		  out << v << std::endl;
		});

  return out;
}


template<class a, class b>
std::ostream& operator<<(std::ostream& out, std::pair<a, b> const& p) {
  out << p.first;
  out << ", "; 
  out << p.second;

  return out;
}


template<class k, class v>
std::ostream& operator<<(std::ostream& out, std::map<k, v> const& m) {
  std::for_each(m.begin(), m.end(),
    [&](std::pair<k, v> p) -> void {
      out << p.first; 
      out << ": "; 
      out << p.second;
      out << std::endl;
    }
  );

  return out; 
}

template<class k, class v>
std::ostream& operator<<(std::ostream& out, map2d<k, v> const& m) {
  std::for_each(m.begin(), m.end(),
    [&](std::pair<k, std::map<k, v>> km) -> void {
      std::for_each(km.second.begin(), km.second.end(), 
          [&](std::pair<k, v> kv) -> void {
            out << km.first << ", " <<  kv.first << ": "; 
            out << kv.second;
            out << std::endl; 
	  }
      );
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

size_t countEmpty(vector<char> const& v) {
  return std::accumulate(v.begin(), v.end(), 0, 
      [&](size_t current, char c) -> size_t {
        return current + (c == '.' ? 1 : 0);
      });
}

size_t countEmpty(matrix<char> const& board) {
  return std::accumulate(board.begin(), board.end(), 0, 
      [&](size_t current, vector<char> const& v) -> size_t {
        return current + countEmpty(v);
      });
}

bool isComplete(vector<char> const& cs) {
  for (char c : cs) {
    if (c == '.') return false;
  }

  return true;
}

std::map<size_t, std::set<char>*> mapFromRow(map2d<size_t, std::set<char>*> const& allowed, int rownum) {
  std::map<size_t, std::set<char>*> m = allowed.at(rownum);

  return m;
}

vector<char> vectorFromRow(matrix<char> const& board, int rownum) {
  vector<char> cs = board[rownum];

  return cs;
}

void rowFromVector(matrix<char>& board, vector<char> row, int rownum) { 
  board[rownum] = row;
}

std::map<size_t, std::set<char>*> mapFromCol(map2d<size_t, std::set<char>*> const& allowed, int colnum) {
  std::map<size_t, std::set<char>*> m;

  for (int i = 0; i < 9; i++) {
    if (allowed.count(i) > 0) {
      std::map<size_t, std::set<char>*> row = mapFromRow(allowed, i);
      if (row.count(colnum) > 0) {
        m[i] = row[colnum];
      }
    }
  }

  return m;
}

vector<char> vectorFromCol(matrix<char> const& board, int colnum) {
  vector<char> cs;
  for (int i = 0; i < 9; i++) {
    cs.push_back(board[i][colnum]);
  }

  return cs;
}

void colFromVector(matrix<char>& board, vector<char> col, int colnum) { 
  for (size_t row = 0; row < 9; row += 1) 
    board[row][colnum] = col[row];
}

std::map<size_t, std::set<char>*> mapFromBlock(map2d<size_t, std::set<char>*> const& allowed, 
    int yBlock, int xBlock) {
  std::map<size_t, std::set<char>*> m;

  int yMin = yBlock * 3, yMax = (yBlock + 1) * 3;
  int xMin = xBlock * 3, xMax = (xBlock + 1) * 3;

  int k = 0;
  for (int i = yMin; i < yMax; i += 1) {
    if (allowed.count(i) > 0) {
      std::map<size_t, std::set<char>*> row = mapFromRow(allowed, i);
      for (int j = xMin; j < xMax; j += 1) {
        if (row.count(j) > 0) {
          m[k] = row[j];
        }

	k += 1;
      }
    }
    else k += 3;
  }

  return m;
}

std::map<size_t, std::set<char>*> mapFromBlock(map2d<size_t, std::set<char>*> const& allowed, 
    int blocknum) {
  int yBlock = blocknum / 3;
  int xBlock = blocknum % 3;

  return mapFromBlock(allowed, yBlock, xBlock);
}

std::vector<char> vectorFromBlock(matrix<char> const& board, 
    int yBlock, int xBlock) {
  std::vector<char> cs;

  int yMin = yBlock * 3, yMax = (yBlock + 1) * 3;
  int xMin = xBlock * 3, xMax = (xBlock + 1) * 3;

  for (int i = yMin; i < yMax; i += 1) {
    for (int j = xMin; j < xMax; j += 1) {
      cs.push_back(board[i][j]);
    }
  }

  return cs;
}

std::vector<char> vectorFromBlock(matrix<char> const& board, int blocknum) {
  int yBlock = blocknum / 3;
  int xBlock = blocknum % 3;

  return vectorFromBlock(board, yBlock, xBlock);
}

void blockFromVector(matrix<char>& board, vector<char> block, int i) {
  int yBlock = i / 3;
  int xBlock = i % 3;

  int yMin = yBlock * 3, yMax = (yBlock + 1) * 3;
  int xMin = xBlock * 3, xMax = (xBlock + 1) * 3;

  int k = 0;
  for (int i = yMin; i < yMax; i += 1) {
    for (int j = xMin; j < xMax; j += 1) {
      board[i][j] = block[k++];
    }
  }
}

std::set<char> notInVector(std::set<char> const& initial, std::vector<char> cs) {
  std::set<char> updated(initial.begin(), initial.end());

  for (char c : cs) updated.erase(c);

  return updated;
}

vector<char> filterVector(std::vector<char> const& v, 
    std::map<size_t, std::set<char>*>& allowed) {
  vector<char> filtered = v;

  std::vector<size_t> seenCounts(9, 0);

  std::vector<size_t> seenIndexes(9, -1);

  size_t i = 0;
  for (char c : filtered) {
    if (c == '.') {
      for (char ai : *allowed[i]) {
	// one-based to zero-based index 
	seenCounts[ai - '1'] += 1;
	seenIndexes[ai - '1'] = i;
      }
    }

    i += 1;
  }

  char c = '1';
  for (size_t count : seenCounts) {
    // only allowed in a single square 
    if (count == 1) {
      int seenIndex = seenIndexes[c - '1'];

      std::cout << "char: " << c << ", index: " << seenIndex << std::endl;

      filtered[seenIndex] = c;
      //allowed[seenIndex]->clear();
      //allowed[seenIndex]->insert(c);
    }

    c += 1;
  }

  //std::cout << "filtered: " << filtered << std::endl;

  return filtered;
}

map2d<size_t, std::set<char>*> initAllowed(matrix<char> const& board) {
  map2d<size_t, std::set<char>*> allowed;

  size_t i = 0;
  for (vector<char> const& row : board) {
    allowed[i] = std::map<size_t, std::set<char>*>();
    size_t j = 0;
    for (char c : row) {
      if (c == '.') {
        allowed[i][j] = new std::set<char>{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
      }

      j += 1;
    }

    i += 1;
  }

  return allowed;
}

void setAllowed(map2d<size_t, std::set<char>*>& allowed, 
    matrix<char>& board) {
  // iterate over squares
  for (std::pair<size_t, std::map<size_t, std::set<char>*>> allowedRow : allowed) {
    size_t y = allowedRow.first;
    std::map<size_t, std::set<char>*> allowedSqs = allowedRow.second;

    for (std::pair<size_t, std::set<char>*> allowedSq : allowedSqs) {
      size_t x = allowedSq.first;

      // while set is not erased, 
      // this prevents the loop from repeating squares 
      if (board[y][x] == '.') {
        int yBlock = y / 3;
        int xBlock = x / 3;
        std::set<char>* allowedHere = allowedSq.second; 

        *allowedHere = notInVector(*allowedHere, board[y]);
        *allowedHere = notInVector(*allowedHere, vectorFromCol(board, x));
        *allowedHere = notInVector(*allowedHere, vectorFromBlock(board, yBlock, xBlock));

        if (allowedHere->size() == 1) {
          board[y][x] = *allowedHere->begin();
          // erase mapped set 
        }
      }
    }
  }
}

void filterBoard(matrix<char>& board,
    map2d<size_t, std::set<char>*>& allowed) {
  // iterate over rows and columns
  for (size_t i = 0; i < 9; i += 1) {

    vector<char> row = vectorFromRow(board, i);
    if (!isComplete(row)) {
      std::map<size_t, std::set<char>*> allowedRow = mapFromRow(allowed, i);
      std::vector<char> filtered = filterVector(row, allowedRow);

      rowFromVector(board, filtered, i);
    }

    vector<char> col = vectorFromCol(board, i);
    if (!isComplete(col)) {
      std::map<size_t, std::set<char>*> allowedCol = mapFromCol(allowed, i);
      vector<char> filtered = filterVector(col, allowedCol);

      colFromVector(board, filtered, i);
    }

    vector<char> block = vectorFromBlock(board, i);
    if (!isComplete(block)) {
      std::map<size_t, std::set<char>*> allowedBlock = mapFromBlock(allowed, i);
      vector<char> filtered = filterVector(block, allowedBlock);

      blockFromVector(board, filtered, i);
    }
  }
}

bool validateBoard(matrix<char> const& board) {
  std::set<char> complete{'1', '2', '3', '4', '5', '6', '7', '8', '9'};

  // iterate over rows, columns and blocks
  for (int i = 0; i < 9; i += 1) {
    vector<char> row = board[i];
    if (isComplete(row)) {
      std::set<char> filtered = notInVector(complete, row);

      if (filtered.size() != countEmpty(row)) return false;
    } 
    //else return false;

    vector<char> col = vectorFromCol(board, i);
    if (isComplete(col)) {
      std::set<char> filtered = notInVector(complete, col);

      if (filtered.size() != countEmpty(col)) return false;
    }
    //else return false;

    vector<char> block = vectorFromBlock(board, i);
    if (isComplete(block)) {
      std::set<char> filtered = notInVector(complete, col);

      if (filtered.size() != countEmpty(block)) return false;
    } 
    //else return false;
  }

  return true;
}

bool solveBoard(matrix<char> &board) {
  map2d<size_t, std::set<char>*> allowed = initAllowed(board);

  setAllowed(allowed, board);

  int maxRuns = 81;
  int nruns = 1;

  //bool changedAnyOrFirst = true;
  while (countEmpty(board) > 0 
      //&& validateBoard(board)
      //&& changedAnyOrFirst 
      && nruns < maxRuns) {
    //changedAnyOrFirst = false;
    
    //filterBoard(board, allowed);

    setAllowed(allowed, board);

    nruns += 1;
  }

  std::cout << allowed << std::endl;

  std::cout << "ran " << nruns << " times" << std::endl;

  //return countEmpty(board) == 0;
  return validateBoard(board);
}

void printBoard(matrix<char> const& board) {

  for (int i : vector<int>{0, 1, 2}) {
    int yMin = i * 3, yMax = (i + 1) * 3;
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

    std::cout << " - - - - - - - - - - - -" << std::endl;
  }
}

int main() {
  matrix<char> board = readBoard();

  //std::cout << board << std::endl;
  printBoard(board);

  bool ok = solveBoard(board);

  if (ok && countEmpty(board) == 0) std::cout << "board was solved";
  else {
    std::cout << "board was not solved" << std::endl;
    if (ok) std::cout << "board is in a valid state" << std::endl;
    else std::cout << "board is not in a valid state" << std::endl;
  }

  std::cout << std::endl;

  printBoard(board);

  return 0;
}
