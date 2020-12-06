mapop(_,[],[],[]):-!.
mapop(Op, [L1H|L1T], [L2H|L2T], [ResH|ResT]):-
  %op(300, xfx, )
  ResH is Op(L1H, L2H, ResH), mapop(Op, L1T, L2T).
