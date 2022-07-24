#include<algorithm>
#include<iostream>
#include<iterator>
#include<map>
#include<numeric>
#include<set>
#include<string>
#include<vector>

#define MAX_RUNS 81

#define CHARSET_ONE2NINE std::set<char>{'1', '2', '3', '4', '5', '6', '7', '8', '9'}

bool isCharInString(char x, std::string cs) {
  return std::any_of(cs.begin(), cs.end(), 
                     [&x](char c) -> bool { return c == x; });
}

template<class a> 
bool isVectorSet(std::vector<a> xs, a ignore) {
  std::set<a> seen;

  for (a x : xs) {
    if (x == ignore || seen.count(x) == 0) seen.insert(x);
    else return false;
  }

  return true;
}

template<class a> 
void copy(std::set<a>* xs, std::set<a> const& ys) {
  xs->clear();

  for (a y : ys) xs->insert(y);
}

template<class a>
void copy(std::vector<a>& xs, std::vector<a> const& ys) {
  xs.clear();

  for (a const& y : ys) xs.push_back(y);
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

template<class a> 
void copy(matrix<a>& mx, matrix<a> const& my) {
  mx.clear();

  size_t i = 0;
  for (std::vector<a> const& ys : my) {
    mx.push_back(std::vector<a>());
    copy(mx[i], ys);

    i += 1;
  }
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
      }
  );

  return row;
}

void printBoard(matrix<char> const& board);

bool validate(matrix<char> const& board);

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

bool isComplete(matrix<char> const& board) {
  for (vector<char> const& row : board) {
    if (!isComplete(row)) return false;
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

void rowFromVector(matrix<char>& board, vector<char> const& row, int rownum) { 
  board[rownum] = row;
}

std::map<size_t, std::set<char>*> mapFromCol(map2d<size_t, std::set<char>*> const& allowed, int colnum) {
  std::map<size_t, std::set<char>*> m;

  for (int i = 0; i < 9; i += 1) {
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
  for (int i = 0; i < 9; i += 1) {
    cs.push_back(board[i][colnum]);
  }

  return cs;
}

void colFromVector(matrix<char>& board, vector<char> col, int colnum) { 
  for (size_t row = 0; row < 9; row += 1) 
    board[row][colnum] = col[row];
}

std::pair<size_t, size_t> coordsFromBlockNum(size_t blocknum) {
  // {i, j}: coords
  return std::pair<size_t, size_t>{blocknum / 3, blocknum % 3};
}

std::map<size_t, std::set<char>*> mapFromBlock(map2d<size_t, std::set<char>*> const& allowed, 
    size_t yBlock, size_t xBlock) {
  std::map<size_t, std::set<char>*> m;

  size_t yMin = yBlock * 3, yMax = (yBlock + 1) * 3;
  size_t xMin = xBlock * 3, xMax = (xBlock + 1) * 3;

  size_t k = 0;
  for (size_t i = yMin; i < yMax; i += 1) {
    if (allowed.count(i) > 0) {
      std::map<size_t, std::set<char>*> row = mapFromRow(allowed, i);
      for (size_t j = xMin; j < xMax; j += 1) {
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
    size_t blocknum) {
  std::pair<size_t, size_t> blockCoords = coordsFromBlockNum(blocknum);

  return mapFromBlock(allowed, blockCoords.first, blockCoords.second);
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
  std::pair<size_t, size_t> blockCoords = coordsFromBlockNum(blocknum);

  return vectorFromBlock(board, blockCoords.first, blockCoords.second);
}

void blockFromVector(matrix<char>& board, vector<char> block, size_t yBlock, size_t xBlock) {
  size_t yMin = yBlock * 3, yMax = (yBlock + 1) * 3;
  size_t xMin = xBlock * 3, xMax = (xBlock + 1) * 3;

  size_t k = 0;
  for (size_t i = yMin; i < yMax; i += 1) {
    for (size_t j = xMin; j < xMax; j += 1) {
      board[i][j] = block[k];
      k += 1;
    }
  }
}

void blockFromVector(matrix<char>& board, vector<char> block, size_t blocknum) {
  std::pair<size_t, size_t> blockCoords = coordsFromBlockNum(blocknum);

 return blockFromVector(board, block, blockCoords.first, blockCoords.second);
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
  std::vector<size_t> seenIndexes(9, 9);

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

      filtered[seenIndex] = c;
      copy(allowed[seenIndex], std::set<char>{c});
    }

    c += 1;
  }

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
        allowed[i][j] = new CHARSET_ONE2NINE;
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
      std::set<char>* allowedHere = allowedSq.second; 

      char c = board[y][x];

      // while set is not erased, 
      // this prevents the loop from repeating squares 
      if (c == '.') {
        copy(allowedHere, CHARSET_ONE2NINE);

        int yBlock = y / 3;
        int xBlock = x / 3;

        copy(allowedHere, notInVector(*allowedHere, vectorFromRow(board, y)));
        copy(allowedHere, notInVector(*allowedHere, vectorFromCol(board, x)));
        copy(allowedHere, notInVector(*allowedHere, vectorFromBlock(board, yBlock, xBlock)));

        if (allowedHere->size() == 1) {
          board[y][x] = *allowedHere->begin();
          // erase mapped set 
        }
      } 
      else if (allowedHere->size() > 1) {
        copy(allowedHere, std::set<char>{c});
      }
    }
  }
}

void filterBoard(matrix<char>& board,
    map2d<size_t, std::set<char>*>& allowed) {
  // iterate over rows, columns and blocks
  for (size_t i = 0; i < 9; i += 1) {
    vector<char> row = vectorFromRow(board, i);
    if (!isComplete(row)) {
      setAllowed(allowed, board);

      std::map<size_t, std::set<char>*> allowedRow = mapFromRow(allowed, i);
      std::vector<char> filtered = filterVector(row, allowedRow);

      if (filtered != row) {
        rowFromVector(board, filtered, i);
      }

      if (!validate(board)) break;
    }

    vector<char> col = vectorFromCol(board, i);
    if (!isComplete(col)) {
      setAllowed(allowed, board);


      std::map<size_t, std::set<char>*> allowedCol = mapFromCol(allowed, i);
      vector<char> filtered = filterVector(col, allowedCol);

      if (filtered != col) {
        colFromVector(board, filtered, i);
      }

      if (!validate(board)) break;
    }

    vector<char> block = vectorFromBlock(board, i);
    if (!isComplete(block)) {
      setAllowed(allowed, board);

      std::map<size_t, std::set<char>*> allowedBlock = mapFromBlock(allowed, i);
      vector<char> filtered = filterVector(block, allowedBlock);

      if (filtered != block) {
        blockFromVector(board, filtered, i);
      }

      if (!validate(board)) break;
    }
  }
}

bool validate(vector<char> const& cs) {
  return isVectorSet(cs, '.');
}

bool validate(matrix<char> const& board) {
  // iterate over rows, columns and blocks
  for (int i = 0; i < 9; i += 1) {
    vector<char> row = vectorFromRow(board, i);
    if (!validate(row)) {
      return false;
    }

    vector<char> col = vectorFromCol(board, i);
    if (!validate(col)) {
      return false;
    }

    vector<char> block = vectorFromBlock(board, i);
    if (!validate(block)) {
      return false;
    }
  }

  return true;
}

bool guessConstraint(matrix<char>& board, 
    map2d<size_t, std::set<char>*>& allowed, 
    map2d<size_t, std::set<char>*>& tried) {
  for (std::pair<size_t, std::map<size_t, std::set<char>*>> allowedRow : allowed) {
    size_t i = allowedRow.first;
    std::map<size_t, std::set<char>*> allowedSqs = allowedRow.second;

    for (std::pair<size_t, std::set<char>*> allowedSq : allowedSqs) {
      size_t j = allowedSq.first;
      std::set<char>* allowedHere = allowedSq.second;

      if (allowedHere->size() > 1) {
        if (tried.count(i) < 1) tried[i] = std::map<size_t, std::set<char>*>();

        if (tried[i].count(j) < 1) tried[i][j] = new std::set<char>();

        for (char c : *allowedHere) {
          if (tried[i][j]->count(c) < 1) { 
            tried[i][j]->insert(c);

            board[i][j] = c;

            return true;
          }
        }
      }
    }
  }

  return false;
}

size_t solveBoard(matrix<char>& board, 
    map2d<size_t, std::set<char>*>& allowed, size_t nruns) {
  size_t nempty = countEmpty(board);
  
  //setAllowed(allowed, board);
  filterBoard(board, allowed);

  if (validate(board) && countEmpty(board) < nempty && nruns < MAX_RUNS)
   nruns = solveBoard(board, allowed, nruns);
    
  return nruns + 1;
}

bool solveBoard(matrix<char> &board) {
  map2d<size_t, std::set<char>*> allowed = initAllowed(board);

  size_t nruns = 0;

  if (validate(board)) 
     nruns = solveBoard(board, allowed, nruns);

  // direct heuristics failed
  if (validate(board) && !isComplete(board)) {
    map2d<size_t, std::set<char>*> tried;
    matrix<char> initialBoard;
    copy(initialBoard, board);

    while (validate(board) && !isComplete(board) && nruns < MAX_RUNS) {
      setAllowed(allowed, board);

      bool guessed = guessConstraint(board, allowed, tried);
      if (!guessed) break;
      
      nruns = solveBoard(board, allowed, nruns);

      if (!isComplete(board)) 
        copy(board, initialBoard);
    }
  }

std::cout << "ran " << nruns << " times" << std::endl;

  // valid and complete
  return validate(board) && isComplete(board);
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

  bool solved = solveBoard(board);

  if (solved) std::cout << "board was solved" << std::endl;
  else {
    std::cout << "board was not solved" << std::endl;
    if (validate(board)) std::cout << "board is in a valid state" << std::endl;
    else std::cout << "board is not in a valid state" << std::endl;
  }

  printBoard(board);

  return 0;
}
