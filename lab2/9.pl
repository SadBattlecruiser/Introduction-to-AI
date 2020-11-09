%Просто все клетки доски
pos([1, 1]).
pos([1, 2]).
pos([1, 3]).
pos([1, 4]).
pos([1, 5]).
pos([1, 6]).
pos([1, 7]).
pos([1, 8]).
pos([2, 1]).
pos([2, 2]).
pos([2, 3]).
pos([2, 4]).
pos([2, 5]).
pos([2, 6]).
pos([2, 7]).
pos([2, 8]).
pos([3, 1]).
pos([3, 2]).
pos([3, 3]).
pos([3, 4]).
pos([3, 5]).
pos([3, 6]).
pos([3, 7]).
pos([3, 8]).
pos([4, 1]).
pos([4, 2]).
pos([4, 3]).
pos([4, 4]).
pos([4, 5]).
pos([4, 6]).
pos([4, 7]).
pos([4, 8]).
pos([5, 1]).
pos([5, 2]).
pos([5, 3]).
pos([5, 4]).
pos([5, 5]).
pos([5, 6]).
pos([5, 7]).
pos([5, 8]).
pos([6, 1]).
pos([6, 2]).
pos([6, 3]).
pos([6, 4]).
pos([6, 5]).
pos([6, 6]).
pos([6, 7]).
pos([6, 8]).
pos([7, 1]).
pos([7, 2]).
pos([7, 3]).
pos([7, 4]).
pos([7, 5]).
pos([7, 6]).
pos([7, 7]).
pos([7, 8]).
pos([8, 1]).
pos([8, 2]).
pos([8, 3]).
pos([8, 4]).
pos([8, 5]).
pos([8, 6]).
pos([8, 7]).
pos([8, 8]).

% Клетка левее
left_pos([CurrH|CurrT], [ResH,ResT]):-
    ResH is CurrH - 1, ResT is CurrT, pos([ResH, ResT]),!.

% Клетка правее
right_pos([CurrH|CurrT], [ResH,ResT]):-
    ResH is CurrH + 1, ResT is CurrT, pos([ResH, ResT]),!.

% Клетка ниже
lower_pos([CurrH|CurrT], [ResH,ResT]):-
    ResH is CurrH, ResT is CurrT - 1, pos([ResH, ResT]),!.

% Клетка выше
higher_pos([CurrH|CurrT], [ResH,ResT]):-
    ResH is CurrH, ResT is CurrT + 1, pos([ResH, ResT]),!.

% Возможный ход
possible_move([PosH|PosT], [MoveH|MoveT]):-
    % По горизонтали
    pos([MoveH|MoveT]), MoveH = PosH, not(MoveT = PosT);
    % По вертикали
    pos([MoveH|MoveT]), MoveT = PosT, not(MoveH = PosH);
    % Диагональ влево вниз - вправо вверх
    pos([MoveH|MoveT]), Y is MoveH - PosH, X is MoveT - PosT,
    X = Y, not(MoveH = PosH);
    % Диагональ влево вверх - вправо вниз
    pos([MoveH|MoveT]), Y is MoveH - PosH, X is PosT - MoveT,
    X = Y, not(MoveH = PosH).

% Клетки, которые пройдем, если сделаем по горизонтали
hor_passed_cells(A, A, [A]):-!.
hor_passed_cells([FromH|FromT], [ToH|ToT], [CellsH|CellsT]):-
    FromH < ToH, right_pos([FromH|FromT], NewFrom),
    CellsH = [FromH|FromT], hor_passed_cells(NewFrom, [ToH|ToT], CellsT), !;

    FromH > ToH, left_pos([FromH|FromT], NewFrom),
    CellsH = [FromH|FromT], hor_passed_cells(NewFrom, [ToH|ToT], CellsT), !.

% Клетки, которые пройдем, если сделаем по горизонтали
ver_passed_cells(A, A, [A]):-!.
ver_passed_cells([FromH|FromT], [ToH|ToT], [CellsH|CellsT]):-
    FromT = [FromTH|_], ToT = [ToTH|_],
    FromTH < ToTH, higher_pos([FromH|FromT], NewFrom),
    CellsH = [FromH|FromT], ver_passed_cells(NewFrom, [ToH|ToT], CellsT), !;

    FromT = [FromTH|_], ToT = [ToTH|_],
    FromTH > ToTH,lower_pos([FromH|FromT], NewFrom),
    CellsH = [FromH|FromT],
    ver_passed_cells(NewFrom, [ToH|ToT], CellsT), !.

% Клетки, которые пройдем, если сделаем по диагонали
diag_passed_cells(A, A, [A]):-!.
diag_passed_cells([FromH|FromT], [ToH|ToT], [CellsH|CellsT]):-
    FromT = [FromTH|_], ToT = [ToTH|_],
    FromH < ToH, FromTH < ToTH,
    higher_pos([FromH|FromT], TempFrom), right_pos(TempFrom, NewFrom),
    CellsH = [FromH|FromT], diag_passed_cells(NewFrom, [ToH|ToT], CellsT), !;

    FromT = [FromTH|_], ToT = [ToTH|_],
    FromH < ToH, FromTH > ToTH,
    lower_pos([FromH|FromT], TempFrom), right_pos(TempFrom, NewFrom),
    %print('test'),
    CellsH = [FromH|FromT], diag_passed_cells(NewFrom, [ToH|ToT], CellsT), !;

    FromT = [FromTH|_], ToT = [ToTH|_],
    FromH > ToH, FromTH < ToTH,
    higher_pos([FromH|FromT], TempFrom), left_pos(TempFrom, NewFrom),
    CellsH = [FromH|FromT], diag_passed_cells(NewFrom, [ToH|ToT], CellsT), !;

    FromT = [FromTH|_], ToT = [ToTH|_],
    FromH > ToH, FromTH > ToTH,
    lower_pos([FromH|FromT], TempFrom), left_pos(TempFrom, NewFrom),
    CellsH = [FromH|FromT], diag_passed_cells(NewFrom, [ToH|ToT], CellsT), !.

% Клетки, которые пройдем, если сделаем ход
%passed_cells([FromH|FromT], [ToH|ToT], Cells):-
%    % По горизонтали
%    FromH = ToH, not(FromT = ToT), Cells


%moves(_,[],0,[]):-!.

% (текущая позиция, что надо посетить, оставшееся количество ходов, сами ходы)
%moves(Pos, Points, N, Moves)
