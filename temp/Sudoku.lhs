#!/usr/bin/env ghci

GHCi, version 8.4.4: http://www.haskell.org/ghc/

Solve Every Sudoku Puzzle
===

See http://norvig.com/sudoku.html

Throughout this program we have:
- `r` is a row,     e.g., `'A'`
- `c` is a column,  e.g., `'3'`
- `s` is a square,  e.g., `"A3"`
- `d` is a digit,   e.g., `'9'`
- `u` is a unit,    e.g., `["A1","B1","C1","D1","E1","F1","G1","H1","I1"]`
- `grid` is a grid, e.g., 81 non-blank chars, e.g., starting with `".18...7...`
- `values` is a `Map` of possible values, e.g., `fromList [("A1", "12349"), ("A2", "8"), ...]`

> module Sudoku where
> import Data.Map (Map, (!))
> import Data.Maybe (catMaybes, isJust, isNothing)
> import Data.Set (Set)
> import Data.Text (Text)

> import qualified Data.Map as Map
> import qualified Data.Set as Set
> import qualified Data.Text as T

First attempt at cross:

```haskell
cross = zipWith (++)
```

This one, instead of performing the intended cartesian
product,
returned a one-to-one mapping of rows and columns.

The next one is a close translation of the Python version:
(`A` and `B` would be treated as types in Haskell,
 so I use `as` and `bs` instead)
And, as far as I know, Haskell doesn't allow the
concatenation of chars,
but I am emulating it by explicitly placing them inside a
list.

Cross product of elements in `as` and elements in `bs`.

> cross as bs = [[a,b] | a <- as, b <- bs]

> digits = "123456789"
> rows = "ABCDEFGHI"
> cols = digits

> squares = cross rows cols

> unitlist = [cross rows [c] | c <- cols] ++
>            [cross [r] cols | r <- rows] ++
>            [cross rs cs | rs <- ["ABC", "DEF", "GHI"], cs <- ["123", "456", "789"]]

> units = Map.fromList [(s, [u | u <- unitlist, s `elem` u]) | s <- squares]

> peers = Map.fromList [(s, Set.fromList [u | u <- concat (units ! s), u /= s]) | s <- squares]

Unit Tests
---

TODO

Parse a Grid
---

Convert grid to a `Map` of possible values,
`(square, digits)`,
or return `Nothing` if a contradiction is detected.

To start, every square can be any digit;
then assign values from the grid.

> parse_grid grid =
>   let values = Map.fromList [(s, T.pack digits) | s <- squares]
>   in Map.foldrWithKey tryAssign (Just values) (grid_values grid)
>   where tryAssign s d vs =
>           if d `elem` digits
>           then assign vs s d
>           else vs

Convert grid into a `Map` of `(square, char)`,
with `'0'` or `'.'` for empties.

TODO: translate assertion into Haskell.

> grid_values grid =
>   let chars = [c | c <- grid, c `elem` (digits ++ "0.")]
>   in Map.fromList (zip squares chars)

Constraint Propagation
---

Eliminate all the other values (except `d`) from `values ! s`,
and propagate.
Return `Just values`,
except return `Nothing` if a contradiction is detected.

> assign values s d =
>   do vs <- values
>      let other_values = T.replace (T.singleton d) (T.pack "") (vs ! s)
>      T.foldr tryEliminate values other_values
>  where tryEliminate d2 vs = eliminate vs s d2

Eliminate `d` from `values ! s`;
propagate when values or places <= 2.
Return `Just values`,
except return `Nothing` if a contradiction is detected.

> eliminate values s d =
>   do vs <- values
>      if not $ d `T.elem` (vs ! s) -- Already eliminated
>      then values
>      else let vs' = Map.insertWith (T.replace (T.singleton d)) s (T.pack "") vs
>               values' = Just vs'
>           in case T.compareLength (vs' ! s) 1 of -- (1) If a square s is reduced to one value d2, then eliminate d2 from the peers.
>                LT -> Nothing -- Contradiction: removed last value.
>                EQ -> let d2 = T.head (vs' ! s)
>                          values'' = Set.foldr (tryEliminate d2) values' (peers ! s)
>                      in complete values'' s d (units ! s)
>                GT -> complete values' s d (units ! s)
>   where tryEliminate d2 s2 values = eliminate values s2 d2

(2) If a unit u is reduced to only one place for a value `d`,
then put it there.

```haskell
complete :: Maybe (Map String Text) -> String -> Char
         -> [String] -> Maybe (Map String Text)
```

> complete values _ _ [] = values
> complete values s d (u:us) =
>   do vs <- values
>      let dplaces = [s | s <- u, d `T.elem` (vs ! s)]
>      case compare (length dplaces) 1 of
>        LT -> Nothing -- Contradiction: no place for this value.
>        EQ -> let values' = assign values (head dplaces) d
>              in complete values' s d us
>        GT -> complete values s d us

Display as 2-D grid
---

Display these values as a 2-D grid.

> display values =
>   let width = 1 + maximum [T.length (values ! s) | s <- squares]
>       line = T.intercalate (T.singleton '+') (replicate 3 (T.replicate (width * 3) (T.singleton '-')))
>       lines = map (lineFromRow width line) rows
>   in foldMap (putStr . T.unpack) (lines ++ [T.singleton '\n'])
>   where lineFromRow w l r =
>           let colsText = T.concat ([textFromCol w r c | c <- cols] ++ [T.singleton '\n'])
>               lineText = [l, T.singleton '\n']
>           in T.concat ([colsText] ++ if r `elem` "CF" then lineText else [])
>         textFromCol w r c =
>           let chars = T.center w ' ' (values ! [r,c])
>           in T.concat ([chars] ++ if c `elem` "36" then [T.singleton '|'] else [])

Search
---

> solve grid = search (parse_grid grid)

Using depth-first search and propagation,
try all possible values.

> search :: Maybe (Map String Text) -> Maybe (Map String Text)
> search values =
>   do vs <- values
>      if all ((== EQ) . ((flip T.compareLength) 1)) [(vs ! s) | s <- squares]
>      then values
>      else let (n, s) = minimum [(T.length (vs ! s), s) | s <- squares, T.length (vs ! s) > 1]
>           in case catMaybes [search (assign values s d) | d <- T.unpack (vs ! s)] of
>                [] -> Nothing
>                (v:_) -> Just v

> replace _ _ [] = []
> replace from to (x:xs)
>   | x == from = to : replace from to xs
>   | otherwise = x : replace from to xs

> _grid = "4 . . |. . . |8 . 5 |. 3 . |. . . |. . . |. . . |7 . . |. . . |. 2 . |. . . |. 6 . |. . . |. 8 . |4 . . |. . . |. 1 . |. . . |. . . |6 . 3 |. 7 .5 . . |2 . . |. . . |1 . 4 |. . . |. . ."

References Used
---

http://www.scanraid.com/BasicStrategies.htm
http://www.sudokudragon.com/sudokustrategy.htm
http://www.krazydad.com/blog/2005/09/29/an-index-of-sudoku-strategies/
http://www2.warwick.ac.uk/fac/sci/moac/currentstudents/peter_cock/python/sudoku/
