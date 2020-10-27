%Доступ к элементу списка
element([],0,[]):-!.
element([Head|Tail],Index,Elem):-
    Index = 1, Head = Elem,!;
    Index > 0, IndexNew is Index-1, element(Tail, IndexNew,Elem).

%Проверяем последний элемент
invert_sub(X,Y,Len,Len):-
     element(X,Len,Elem),element(Y,1,Elem),!.
%Проверяем не последний элемент
invert_sub(X,Y,Index,Len):-
    %Index - Len == 1,!;
    Index2 is Len-Index+1, element(X,Index,Elem),element(Y,Index2,Elem),
    IndexNew is Index+1, invert_sub(X,Y,IndexNew,Len).

invert([],[]):-!.
invert(X,Y):-
    length(X,Len), length(Y,Len),!,invert_sub(X,Y,1,Len).


