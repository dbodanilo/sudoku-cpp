Command used to run file interactively:

```sh
$ stack exec --resolver lts-19 --package "containers text" ghci sudoku.lhs
```

<pre>
Selected resolver: lts-19.33
GHCi, version 9.0.2: https://www.haskell.org/ghc/  :? for help
[1 of 1] Compiling Main             ( sudoku.lhs, interpreted )
Ok, one module loaded.
</pre>

For the time and memory statistics:

```haskell
ghci> :set +s
```

For the comparison of memory usage:

```haskell
ghci> putStr ""
```
<pre>
(0.00 secs, 73,376 bytes)
</pre>

> module SudokuTest where
> import Sudoku

> grid1    = "003020600900305001001806400008102900700000008006708200002609500800203009005010300"
> grid2    = "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......"
> hardest  = "85...24..72......9..4.........1.7..23.5...9...4...........8..7..17..........36.4."
> hardest1 = "..53.....8......2..7..1.5..4....53...1..7...6..32...8..6.5....9..4....3......97.."
> hard1    = ".....6....59.....82....8....45........3........6..3.54...325..6.................."

> main =
>   let (Just vs) = solve _grid
>       (Just vs_grid1) = solve grid1
>       (Just vs_grid2) = solve grid2
>       (Just vs_hardest) = solve hardest
>       (Just vs_hardest1) = solve hardest1
>       (Just vs_hard1) = solve hard1
>   in do display vs

<pre>
 4 1 7| 3 6 9| 8 2 5
 6 3 2| 1 5 8| 9 4 7
 9 5 8| 7 2 4| 3 1 6
------+------+------
 8 2 5| 4 3 7| 1 6 9
 7 9 1| 5 8 6| 4 3 2
 3 4 6| 9 1 2| 7 5 8
------+------+------
 2 8 9| 6 4 3| 5 7 1
 5 7 3| 2 9 1| 6 8 4
 1 6 4| 8 7 5| 2 9 3

(0.04 secs, 12,556,720 bytes)
</pre>

>         display vs_grid1

<pre>
 4 8 3| 9 2 1| 6 5 7
 9 6 7| 3 4 5| 8 2 1
 2 5 1| 8 7 6| 4 9 3
------+------+------
 5 4 8| 1 3 2| 9 7 6
 7 2 9| 5 6 4| 1 3 8
 1 3 6| 7 9 8| 2 4 5
------+------+------
 3 7 2| 6 8 9| 5 1 4
 8 1 4| 2 5 3| 7 6 9
 6 9 5| 4 1 7| 3 8 2

(0.02 secs, 6,689,192 bytes)
</pre>

>         display vs_grid2

<pre>
 4 1 7| 3 6 9| 8 2 5
 6 3 2| 1 5 8| 9 4 7
 9 5 8| 7 2 4| 3 1 6
------+------+------
 8 2 5| 4 3 7| 1 6 9
 7 9 1| 5 8 6| 4 3 2
 3 4 6| 9 1 2| 7 5 8
------+------+------
 2 8 9| 6 4 3| 5 7 1
 5 7 3| 2 9 1| 6 8 4
 1 6 4| 8 7 5| 2 9 3

(0.03 secs, 11,488,304 bytes)
</pre>

>         display vs_hardest

<pre>
 8 5 9| 6 1 2| 4 3 7
 7 2 3| 8 5 4| 1 6 9
 1 6 4| 3 7 9| 5 2 8
------+------+------
 9 8 6| 1 4 7| 3 5 2
 3 7 5| 2 6 8| 9 1 4
 2 4 1| 5 9 3| 7 8 6
------+------+------
 4 3 2| 9 8 1| 6 7 5
 6 1 7| 4 2 5| 8 9 3
 5 9 8| 7 3 6| 2 4 1

(0.02 secs, 8,825,792 bytes)
</pre>

>         display vs_hardest1

<pre>
 1 4 5| 3 2 7| 6 9 8
 8 3 9| 6 5 4| 1 2 7
 6 7 2| 9 1 8| 5 4 3
------+------+------
 4 9 6| 1 8 5| 3 7 2
 2 1 8| 4 7 3| 9 5 6
 7 5 3| 2 9 6| 4 8 1
------+------+------
 3 6 7| 5 4 2| 8 1 9
 9 8 4| 7 6 1| 2 3 5
 5 2 1| 8 3 9| 7 6 4

(0.03 secs, 12,227,472 bytes)
</pre>

>         display vs_hard1

<pre>
 4 3 8| 7 9 6| 2 1 5
 6 5 9| 1 3 2| 4 7 8
 2 7 1| 4 5 8| 6 9 3
------+------+------
 8 4 5| 2 1 9| 3 6 7
 7 1 3| 5 6 4| 8 2 9
 9 2 6| 8 7 3| 1 5 4
------+------+------
 1 9 4| 3 2 5| 7 8 6
 3 6 2| 9 8 7| 5 4 1
 5 8 7| 6 4 1| 9 3 2

(324.85 secs, 111,910,077,608 bytes)
</pre>