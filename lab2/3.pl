%������� ��� ������� ����������� � �����
n_of_incl([],_,Num):-
    Num = 0,!. % ��� ��� ������� ������, �� ����� 1
n_of_incl(_,[],Num):-
    Num = 0,!.
n_of_incl(Elem,[H|T],Num):-
    Elem = H, n_of_incl(Elem,T,NumNew), Num is NumNew + 1,!;
    not(Elem = H), n_of_incl(Elem,T,NumNew), Num is NumNew.

% � ���� ���������� ��������, ���

uniq([],_):-!.
uniq([XH|XT],Y):-
    n_of_incl(XH,Y,1),
    uniq(XT,Y).
