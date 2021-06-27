
	Parsare json:
		- am folosit biblioteca nlohmann din C++.
		- am utilizat biblioteca pentru functiile add_book, register si login si 
		enter_library(parsare token). Pentru a prelua cookie-ul, respectiv JWT din
		raspunsul serverului m-am folosit de functii pe siruri de caractere (strstr).


	Cod laborator:
		- punctul de plecare in cadrul acestei teme a fost laboratoul 10. Am modificat functiile
			compute_get_request si compute_post_request din requests.cpp:
				a) adaugare camp JWT.
				b) verificari existenta cookie, token, body, etc (!= NULL && != "").
				c) modificare continut din vector de siruri in sir.
				d) modificare tip din (char *) in (const char *).
		- am adaugat o functie noua in requests.cpp, compute_delete_request care este aproape
			identica cu functie compute_get_request(in loc de "GET" este "DELETE"), folosita in
			cadrul delete_book. Creeaza un DELETE REQUEST pe care apoi il trimit la server.


	Structuri + citire date:
		- am folosit std::string si siruri de caractere(char * || char nume_sir[valoare]).
		- am citit o linie cu getline(cin, nume_string). Am considerat ca pot exista spatii
			in username, password, book_title, book_author, book_publisher si book_genre. Pentru
			id din cadrul get_book si delete_book, precum si pentru count_pages din add_book am
			verificat ca sirul introdus sa fie un numar(functia isNumber). Programul citeste pana
			cand este introdus un numar valid.


	Implementare:

	1) register:
		- citim username si parola
		- cream un obiect json cu aceste valori
		- construim un post request pe care il trimitem pe server
		- afisam mesajul trimis
		- afisam raspunsul de la server

	2) login:
		- analog cu register
		- salvam cookie de sesiune.

	3) enter_library:
		- construim un get request cu cookie-ul de conectare(primit la login).
		- afisam mesajul trimis.
		- afisam respunsul de la server.
		- salvam JWT primit.

	4) get_books:
		- get request cu JWT.
		- afisam mesajul trimis.
		- afisam raspunsul de la server.

	5) get_book:
		- citire id pana cand este introdus un id valid.
		- construim url-ul.
		- construim un get request pe care il trimitem pe server.
		- printam mesajul primit.
		- printam raspunsul de la server.

	6) add_book:
		- citim campurile necesare unei carti.
		- la count_pages citim pana cand este introdusa o valoare valida.
		- cream un obiect json.
		- cream string-ul asociat acestui obiect json(obiect_json.dump()).
		- trimitem un post request cu campurile cartii si cu JWT
		- afisare mesaj trimis + raspuns

	7) delete_book:
		- citire id pana cand este introdus un id valid.
		- am creat o functie auxiliara compute_delete_request care 
			creeaza un delete request.
		- trimitem un delete request cu JWT
		- afisare mesaj trimis + raspuns

	8) logout:
		- trimitem un post request cu cookiul
		- "stergem" token-ul si cookie-ul (le facem vide: "").
		- afisare mesaj trimis + raspuns

	9) exit:
		- exit(0).
