:-set_prolog_flag(answer_write_options,[max_depth(0)]).

opora(sfera, '2.910.01', m6).
opora(sfera, '2.910.02', m8).
opora(sfera, '3.910.01', m12).
opora(sfera, '3.910.02', m16).

opora(nasechka, '2.911.01', m6).
opora(nasechka, '2.911.02', m8).
opora(nasechka, '3.911.01', m12).

shtyr('2.213.01', m6, 6, 8).
shtyr('2.213.04', m8, 8, 12).
shtyr('3.213.06', m12, 12, 26).

kulachok(rezba, '2.913.05', '30_18', m6, 16).
kulachok(rezba, '2.913.06', '45_22', m8, 20).
kulachok(rezba, '2.913.09', '65_30', m12, 38).

kulachok(prizma, '2.913.01', '30_18', 10, 8, 12, 3, 7).
kulachok(prizma, '2.913.02', '45_22', 12, 8, 12, 3, 7).
kulachok(prizma, '2.913.07', '65_30', 25, 12, 30, 8, 18).

zazhim('2.451.01', '45_30', '30_18', 29).
zazhim('2.451.02', '60_45', '45_22', 34).
zazhim('3.451.01', '60_45', '45_22', 35).
zazhim('3.451.02', '90_60', '65_30', 42).

prokladka('2.217.10', '45_30', 5).
prokladka('2.217.09', '45_30', 3).
prokladka('2.217.07', '45_30', 2).
prokladka('2.217.01', '45_30', 1).

prokladka('3.217.10', '60_45', 5).
prokladka('3.217.09', '60_45', 3).
prokladka('3.217.07', '60_45', 2).
prokladka('3.217.01', '60_45', 1).

prokladka('3.107.28', '90_60', 5).
prokladka('3.107.27', '90_60', 3).
prokladka('3.107.25', '90_60', 2).


% Пакет прокладок определенного типа и толщины
% Проблема - разный порядок прокладок - разные решения
% (список кодов, Num, Type, Tolshina пакета)
paket_prokladok([],0,_,0):-!.
paket_prokladok(_,_,_,Tolshina):-
    Tolshina < 0, !, fail.
paket_prokladok([SpisokH|SpisokT], Num, Type, Tolshina):-
    prokladka(SpisokH,Type,H),
    TolshinaNew is Tolshina - H,
    paket_prokladok(SpisokT, NumNew, Type, TolshinaNew),
    Num is NumNew + 1.

% Штука, которая считает повторения элемента в листе
frequency([],_,0):-!.
frequency([InputH|InputT], Elem, Freq):-
    InputH = Elem, frequency(InputT, Elem, Freq2), Freq is Freq2 + 1,!;
    not(InputH = Elem), frequency(InputT, Elem, Freq).

% Штука, которая формирует список всех типов прокладок заданного размера
prokl_codes(List, ProklType):-
  findall(ProklCode, prokladka(ProklCode,ProklType,_), List).

% Количество вхождений элементов одного листа во второй
list_frequency([],_,[]):-!.
list_frequency([WhatH|WhatT], Where, [ResultH|ResultT]):-
  frequency(Where, WhatH, Temp), ResultH = [WhatH,Temp], list_frequency(WhatT, Where, ResultT).

% То же, что и пакет прокладок, но вывод в другом формате
% (список кодов, Num, Type, Tolshina пакета)
paket_prokladok2(Spisok, Num, Type, Tolshina):-
    paket_prokladok(SpisokFull, Num, Type, Tolshina),
    prokl_codes(ProklCodes, Type),
    list_frequency(ProklCodes, SpisokFull, Spisok).


% Подвижная часть
podvizhnaya_chast(TypePov, D, CodeKul, TypeKul, CodeUst, H):-
    TypePov = plosk_chist, opora(sfera, CodeUst, Drezb),
    kulachok(rezba, CodeKul, TypeKul, Drezb, H);

    TypePov = plosk_chern, opora(nasechka, CodeUst, Drezb),
    kulachok(rezba, CodeKul, TypeKul, Drezb, H);

    TypePov = perfor, shtyr(CodeUst, Drezb, Dmin, Dmax),
    D>=Dmin, D=<Dmax,
    kulachok(rezba, CodeKul, TypeKul, Drezb, H);

    TypePov = cil_vert, CodeUst = none,
    kulachok(prizma, CodeKul, TypeKul, H, _, _, Dmin, Dmax),
    D>=Dmin, D=<Dmax;

    TypePov = cil_gor, CodeUst = none,
    kulachok(prizma, CodeKul, TypeKul, H, Dmin, Dmax, _, _),
    D>=Dmin, D=<Dmax.


%usp в сборе
usp(TypePov, D, H, CodeZash, CodeKul, CodeUst, CodeProkl, NProkl) :-
    podvizhnaya_chast(TypePov, D, CodeKul, TypeKul, CodeUst, HPodv),
    zazhim(CodeZash, TypeZazh, TypeKul, HZazh),
    HProkl is H-HPodv-HZazh,
    %sum([H, -HPodv, -HZazh], #=, HProkl),
    paket_prokladok2(CodeProkl, NProkl, TypeZazh, HProkl).
