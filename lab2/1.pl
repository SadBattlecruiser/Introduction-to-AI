same([ ], [ ]):-!.
same([H1|R1], [H2|R2]):-
    H1 = H2,
    same(R1, R2).

concat([],[],[]):-!.
concat(X,Y,Z):-
    X = [], same(Y,Z),!;
    Y = [], same(X,Z),!;
    X = [XH|XT], Z = [ZH|ZT], XH=ZH, concat(XT,Y,ZT).
