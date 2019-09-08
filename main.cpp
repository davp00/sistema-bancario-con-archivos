#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <string>

#define DAT_USUARIOS "Usuarios.dat"
#define DAT_CUENTAS "Cuentas.dat"
#define DAT_MOVIMIENTOS "Movimientos.dat"

using namespace std;

struct Cuenta
{
	char numero[5];
	char nombre_persona[40];
	string cedula;
	string direccion;
	string telefono;
	string email;
	int saldo;
	char estado;
	char user[8];
};

struct Movimiento
{
	int ID;
	char numero_cuenta[5];
	string fecha;
	char tipo;
	int monto;
	char user[8];
};


struct Usuario
{
	char nick[8];
	char nombre[40];
	char password[6];
	char tipo;
};

char h = 196;
char v = 179;
char esi = 218;
char esd = 191;
char t = 180;
char t2 = 195;
char u = 233;
char eii = 192;
char eid = 217;
char o = 162;

char getcht();
char *leer_letras();
char *leer_numeros();
string leer_password();
void gotoxy(int x, int y);
int menus(int ini, int fin, int tipo);
void lista_menu(int tipo);
void formulario(int tipo);
void crear_archivo_si_no_existe(const char* txt);
int *crear_tabla(vector<string> fields, int tam, int n_datos);

void cargar_usuarios();
void cargar_cuentas();
void cargar_movimientos();

void login();
Usuario *buscar_usuario(const char*nick);
void cerrar_sesion();

void menu_admin();
void menu_cajero();
void menu_cuenta();


//FUNCIONES DEL ADMINISTRADOR
void agregar_usuarios();
void guardar_usuario(const char *file,Usuario *user);
void modificar_usuario();
void eliminar_usuario();
void edit_user(char *nick, Usuario *editedUser, bool del);
void lista_usuarios();

//FUNCIONES PANEL DE CUENTAS
void agregar_cuentas();
void guardar_cuenta(const char *file, Cuenta *cuenta);
Cuenta *buscar_cuenta(const char* n_cuenta);
void modificar_cuenta();
void edit_account(Cuenta* editedAccount);
void bloquear_cuenta();

vector<Usuario *> usuarios;
vector<Cuenta*> cuentas;
vector<Movimiento*> movimientos;

Usuario* logged_user = NULL;

ifstream ficheroEntrada;
ofstream ficheroSalida;

int main()
{
	bool salir = false;
	


	cargar_usuarios();
	cargar_cuentas();
	cargar_movimientos();

	system("color F0");

	logged_user = buscar_usuario("suaj");


	do {
		if (!logged_user)
		{
			login();
		}
		else
		{
			switch (logged_user->tipo)
			{
				case '1':
					menu_admin();
					break;
				case '2':
					break;
				case '3':
					menu_cuenta();
					break;
				case '4':
					break;

				default:
					cout << "Tipo de usuario infedifinido";
					break;
			}
		}

	} while (!salir);


}

void menu_cuenta()
{
	system("CLS");
	switch (menus(1, 4, 3))
	{
		case 1:
			agregar_cuentas();
			break;

		case 2:
			modificar_cuenta();
			break;

		case 3:
			bloquear_cuenta();
			break;

		case 4:

			break;

		case 27 - 48:
			cerrar_sesion();
			break;
	}
}

void agregar_cuentas()
{
	system("CLS");
	formulario(3);

	string in;
	Cuenta* cuenta = new Cuenta();

	gotoxy(41, 7); getline(cin, in);

	if (in.size() > 5)
	{
		gotoxy(30, 20); cout << "El numero excede los 5 caracteres";
		_getch();
		return;
	}
	else if (buscar_cuenta(in.c_str()))
	{
		gotoxy(30, 20); cout << "El numero de cuenta ya existe";
		_getch();
		return;
	}

	strcpy_s(cuenta->numero, in.c_str());

	gotoxy(49, 8); getline(cin, in);

	if (in.size() > 40)
	{
		gotoxy(30, 20); cout << "El nombre excede los 40 caracteres";
		_getch();
		return;
	}

	strcpy_s(cuenta->nombre_persona, in.c_str());

	gotoxy(41, 9); in = string(leer_numeros());

	cuenta->cedula = in;

	gotoxy(44, 10); getline(cin, in);
	cuenta->direccion = in;

	gotoxy(43, 11); in = string(leer_numeros());
	cuenta->telefono = in;

	gotoxy(40, 12); getline(cin, in);
	cuenta->email = in;

	gotoxy(42, 13); getline(cin, in);
	if (in.size() > 8)
	{
		gotoxy(30, 20); cout << "El usuario excede los 8 caracteres";
		_getch();
		return;
	}

	if (!buscar_usuario(in.c_str()))
	{
		gotoxy(30, 20); cout << "Usuario no encontrado";
		_getch();
		return;
	}
	strcpy_s(cuenta->user, in.c_str());
	cuenta->saldo = 0;
	cuenta->estado = 'A';

	guardar_cuenta(DAT_CUENTAS, cuenta);
	cuentas.push_back(cuenta);


	_getch();
}

Cuenta* buscar_cuenta(const char* n_cuenta)
{
	for (int a = 0; a < cuentas.size(); a++)
	{
		if (strcmp(n_cuenta, cuentas[a]->numero) == 0)
		{
			return cuentas[a];
		}
	}

	return NULL;
}

void guardar_cuenta(const char* file, Cuenta* cuenta)
{
	ficheroSalida.open(file, ios::app);

	ficheroSalida << cuenta->numero << endl;
	ficheroSalida << cuenta->nombre_persona << endl;
	ficheroSalida << cuenta->cedula << endl;
	ficheroSalida << cuenta->direccion << endl;
	ficheroSalida << cuenta->telefono << endl;
	ficheroSalida << cuenta->email << endl;
	ficheroSalida << cuenta->saldo << endl;
	ficheroSalida << cuenta->estado << endl;
	ficheroSalida << cuenta->user << endl;

	ficheroSalida.close();

}

void modificar_cuenta()
{
	system("CLS");
	formulario(4);
	gotoxy(41, 10); cout << "Modificar Cuenta";

	string n_cuenta;

	gotoxy(42, 7); getline(cin, n_cuenta);

	Cuenta* cuenta = buscar_cuenta(n_cuenta.c_str());

	if (!cuenta)
	{
		gotoxy(30, 15); cout << "Cuenta no encontrada";
		_getch();
		return;
	}

	system("CLS");
	switch (menus(1,5,4))
	{
		case 1:
		{
			gotoxy(30, 21); cout << "Ingrese el nombre de la persona: ";
			string nombre;
			getline(cin, nombre);
			gotoxy(30, 23);
			if (nombre.size() > 40)
			{
				cout << "El nombre ha excedido los 40 caracteres";
				break;
			}
			strcpy_s(cuenta->nombre_persona, nombre.c_str());
			edit_account(cuenta);
			cout << "Editado con exito.";
		}
		break;
		case 2:
		{
			gotoxy(30, 21); cout << "Ingrese la nueva cedula: ";
			string cc = string(leer_numeros());
			cuenta->cedula = cc;
			edit_account(cuenta);
			gotoxy(30, 23); cout << "Editado con exito.";
		}
		break;
		case 3:
		{
			gotoxy(30, 21); cout << "Ingrese la nueva direccion: ";
			string direccion;
			getline(cin, direccion);
			cuenta->direccion = direccion;
			edit_account(cuenta);
			gotoxy(30, 23); cout << "Editado con exito.";
		}
		break;
		case 4:
		{
			gotoxy(30, 21); cout << "Ingrese el nuevo telefono: ";
			string telefono = string(leer_numeros());
			cuenta->telefono = telefono;
			edit_account(cuenta);
			gotoxy(30, 23); cout << "Editado con exito.";
		}
		break;
		case 5:
		{
			gotoxy(30, 21); cout << "Ingrese el nuevo email: ";
			string email;
			getline(cin, email);
			cuenta->email = email;
			edit_account(cuenta);
			gotoxy(30, 23); cout << "Editado con exito.";
		}
		break;
		case 27-48:
			return;
			break;
		default:
			break;
	}

	_getch();
}

void bloquear_cuenta()
{
	system("CLS");
	formulario(4);
	gotoxy(41, 10); cout << "Bloquear Cuenta";

	string n_cuenta;

	gotoxy(42, 7); getline(cin, n_cuenta);

	Cuenta* cuenta = buscar_cuenta(n_cuenta.c_str());
	gotoxy(30, 15);

	if (!cuenta)
	{
		cout << "Cuenta no encontrada";
		_getch();
		return;
	}

	if (cuenta->estado == 'B')
	{
		cout << "La cuenta ya se encuentra bloqueada";
		_getch();
		return;
	}

	if (cuenta->saldo != 0)
	{
		cout << "No puede bloquear la cuenta porque tiene saldo";
		_getch();
		return;
	}

	cuenta->estado = 'B';
	edit_account(cuenta);
	cout << "Bloqueada con exito";

	_getch();
}

void edit_account(Cuenta* editedAccount)
{
	ficheroEntrada.open(DAT_CUENTAS, ios::in);
	ofstream salidaTemporal;
	const char* tempFile = "cuentasTemp.dat";
	salidaTemporal.open(tempFile, ios::out);
	salidaTemporal.close();

	while (!ficheroEntrada.eof())
	{
		Cuenta* cuenta = new Cuenta();
		ficheroEntrada.getline(cuenta->numero, 5);
		if (ficheroEntrada.eof())
		{
			ficheroEntrada.close();
			break;
		}
		ficheroEntrada.getline(cuenta->nombre_persona, 40);
		getline(ficheroEntrada, cuenta->cedula);
		getline(ficheroEntrada, cuenta->direccion);
		getline(ficheroEntrada, cuenta->telefono);
		getline(ficheroEntrada, cuenta->email);

		string saldo;
		getline(ficheroEntrada, saldo);
		cuenta->saldo = atoi(saldo.c_str());

		char* estado = new char();
		ficheroEntrada.getline(estado, 2);
		cuenta->estado = estado[0];

		ficheroEntrada.getline(cuenta->user, 8);

		if (strcmp(editedAccount->numero, cuenta->numero) == 0)
		{
			cuenta = editedAccount;
		}

		guardar_cuenta(tempFile, cuenta);
	}
	ficheroEntrada.close();
	remove(DAT_CUENTAS);
	rename(tempFile, DAT_CUENTAS);
}

void menu_admin()
{
	system("CLS");
	switch (menus(1,4,1))
	{
		case 1:
			agregar_usuarios();
			break;

		case 2:
			modificar_usuario();
			break;

		case 3:
			eliminar_usuario();
		break;

		case 4:
			lista_usuarios();
		break;

		case 27-48:
			cerrar_sesion();
			break;
	}
}

void agregar_usuarios()
{
	system("CLS");
	formulario(1);
	string in;
	Usuario* nuevo = new Usuario();

	gotoxy(42, 7); getline(cin, in);

	if (in.size() > 8)
	{
		gotoxy(30,20); cout << "El nick ha excedido los 8 caracteres";
		_getch();
		return;
	}
	else if (buscar_usuario(in.c_str()))
	{
		gotoxy(30, 20); cout << "El nick ya existe en los registros";
		_getch();
		return;
	}

	strcpy_s(nuevo->nick, in.c_str());

	gotoxy(41, 8); getline(cin, in);

	if (in.size() > 40)
	{
		gotoxy(30, 20); cout << "El nombre ha excedido los 40 caracteres";
		_getch();
		return;
	}

	strcpy_s(nuevo->nombre, in.c_str());


	gotoxy(45, 9); getline(cin, in);

	if (in.size() > 6)
	{
		gotoxy(30, 20); cout << "la clave ha excedido los 6 caracteres";
		_getch();
		return;
	}

	strcpy_s(nuevo->password, in.c_str());

	gotoxy(39, 10); char* tipo = leer_numeros();
	int tipo_numerico = atoi(tipo);

	if (tipo_numerico > 4)
	{
		cout << tipo_numerico;
		gotoxy(30, 20); cout << "Tipo de cuenta no valido";
		_getch();
		return;
	}

	nuevo->tipo = tipo[0];


	guardar_usuario(DAT_USUARIOS,nuevo);

	gotoxy(30, 20); cout << "Usuario Creado con exito";
	_getch();
}

void guardar_usuario(const char *file, Usuario* user)
{
	ficheroSalida.open(file, ios::app);
	ficheroSalida << user->nick << endl;
	ficheroSalida << user->nombre << endl;
	ficheroSalida << user->password << endl;
	ficheroSalida << user->tipo << endl;

	if(strcmp(file,DAT_USUARIOS) == 0)usuarios.push_back(user);

	ficheroSalida.close();
}

void modificar_usuario()
{
	system("CLS");
	formulario(2);
	gotoxy(41, 10); cout << "Editar Usuario";
	string nick;

	gotoxy(42, 7); getline(cin, nick);

	Usuario* usuario = buscar_usuario(nick.c_str());

	if (!usuario)
	{
		gotoxy(30, 20); cout << "Usuario no encontrado";
		_getch();
		return;
	}

	system("CLS");
	gotoxy(42, 7); cout << usuario->nick;

	switch (menus(1,3,2))
	{
		case 1:
		{
			gotoxy(30, 20); cout << "Ingrese el nuevo nombre: ";
			char* nombre = leer_letras();
			gotoxy(30, 22);
			if (strlen(nombre) <= 40)
			{
				strcpy_s(usuario->nombre, nombre);
				edit_user(usuario->nick, usuario, false);
				cout << "Editado con exito";
			}
			else
			{
				cout << "El nombre excede los 40 caracteres";
			}
			_getch();
		}
		break;

		case 2:
		{
			gotoxy(30, 20); cout << "Ingrese la nueva clave: ";
			string pass;
			getline(cin, pass);
			gotoxy(30, 22);
			if (pass.size() <= 6)
			{
				strcpy_s(usuario->password, pass.c_str());
				edit_user(usuario->nick, usuario, false);
				cout << "Editado con exito";
			}
			else
			{
				cout << "La clave excede los 6 caracteres";
			}
			_getch();
		}
		break;
		case 3:
		{
			gotoxy(30, 20); cout << "Ingrese el nuevo tipo: ";
			string tipo;
			getline(cin, tipo);
			gotoxy(30, 22);
			if (tipo.size() <= 2 && tipo[0] >= '1' && tipo[0] <= '4')
			{
				usuario->tipo = tipo[0];
				edit_user(usuario->nick, usuario, false);
				cout << "Editado con exito";
			}
			else
			{
				cout << "Datos no validos";
			}
			_getch();
		}
		break;
		case 27-48:
		break;
	}
}

//Agregar restriccion para cuando tengan registros
void eliminar_usuario()
{
	system("CLS");
	formulario(2);
	gotoxy(40, 10); cout << "Eliminar Usuario";
	string nick;

	gotoxy(42, 7); getline(cin, nick);

	Usuario* usuario = buscar_usuario(nick.c_str());

	if (!usuario)
	{
		gotoxy(30, 20); cout << "Usuario no encontrado";
		_getch();
		return;
	}

	edit_user(usuario->nick, NULL, true);
	gotoxy(30, 15); cout << "Usuario Eliminado.";
	_getch();
}

void edit_user(char* nick, Usuario* editedUser, bool del)
{
	ficheroEntrada.open(DAT_USUARIOS, ios::in);

	ofstream salidaTemporal;
	const char* tempFile = "UserTemp.dat";
	salidaTemporal.open(tempFile, ios::out);
	salidaTemporal.close();
	bool find = false;

	while (!ficheroEntrada.eof())
	{
		Usuario* usuario = new Usuario();

		ficheroEntrada.getline(usuario->nick, 8);

		if (ficheroEntrada.eof())
		{
			ficheroEntrada.close();
			break;
		}

		ficheroEntrada.getline(usuario->nombre, 40);
		ficheroEntrada.getline(usuario->password, 6);
		char* tipo = new char();
		ficheroEntrada.getline(tipo, 2);
		usuario->tipo = tipo[0];

		if (_strcmpi(nick, usuario->nick) == 0)
		{
			usuario = editedUser;
			find = true;
		}

		if ( del && find ) 
		{
			del = false;
		}
		else
		{
			guardar_usuario(tempFile, usuario);
		}
	}

	ficheroEntrada.close();
	remove(DAT_USUARIOS);
	rename(tempFile, DAT_USUARIOS);
}

void lista_usuarios()
{
	system("CLS");
	vector<string> fields = { "Usuario", "Nombre", "Tipo" };
	int tam = 35;
	int y = 6;
	int *values = crear_tabla(fields, tam, usuarios.size());

	gotoxy(45, 2); cout << "Lista de Usuarios";

	for (int a = 0; a < usuarios.size(); a++)
	{
		gotoxy(2, y); cout << v;
		gotoxy(4, y); cout << usuarios[a]->nick;
		gotoxy(tam + 4, y); cout << usuarios[a]->nombre;
		gotoxy(2 * tam + 4, y); cout << usuarios[a]->tipo;
		gotoxy(values[1], y); cout << v;
		y++;
	}

	_getch();
}

int *crear_tabla(vector<string> fields, int tam, int n_datos)
{
	int *values = new int();
	int y = 1;
	int arreglo_visual = 2;

	int total_tam = (tam * fields.size()) + arreglo_visual;

	gotoxy(2, y); cout << esi;

	for (int a = arreglo_visual + 1; a < total_tam; a++)
	{
		gotoxy(a, y); cout << h;
	}
	cout << esd;
	gotoxy(arreglo_visual, ++y); cout << v;
	gotoxy(total_tam, y); cout << v;
	gotoxy(arreglo_visual, ++y); cout << t2;
	for (int a = arreglo_visual + 1; a < total_tam; a++)
	{
		gotoxy(a, y); cout << h;
	}
	gotoxy(total_tam, y); cout << t;
	++y;

	gotoxy(2, y); cout << v;
	gotoxy(2, y+1); cout << v;
	gotoxy(total_tam, y); cout << v;
	gotoxy(total_tam, y+1); cout << v;
	for (int a = 0; a < fields.size(); a++)
	{
		gotoxy(a * tam + 4, y); cout << fields[a];
	}

	gotoxy(arreglo_visual, ++y); cout << t2;
	for (int a = arreglo_visual + 1; a < total_tam; a++)
	{
		gotoxy(a, y); cout << h;
	}
	gotoxy(total_tam, y); cout << t;
	values[0] = y;
	values[1] = total_tam;

	y = y + n_datos;

	gotoxy(arreglo_visual, ++y); cout << eii;

	for (int a = arreglo_visual + 1; a < total_tam; a++)
	{
		gotoxy(a, y); cout << h;
	}
	gotoxy(total_tam, y); cout << eid;

	return values;
}

void cerrar_sesion()
{
	logged_user = NULL;
	system("CLS");
}

void login()
{
	lista_menu(0);
	string nick;
	string password;

	gotoxy(41, 7);

	getline(cin, nick);

	Usuario *user_encontrado = buscar_usuario(nick.c_str());

	if (!user_encontrado)
	{
		gotoxy(30,20); cout << "Usuario no encontrado";
		_getch();
		system("CLS");
		return;
	}

	gotoxy(44, 10); password = leer_password();//getline(cin, password);

	if (strcmp(user_encontrado->password, password.c_str()) == 0)
	{
		logged_user = user_encontrado;
		//system("CLS");
	}


}

Usuario* buscar_usuario(const char *nick)
{
	for (int a = 0; a < usuarios.size(); a++)
	{
		if (_strcmpi(nick, usuarios[a]->nick) == 0)
		{
			return usuarios[a];
		}
	}

	return NULL;
}

void cargar_usuarios()
{
	ficheroEntrada.open(DAT_USUARIOS, ios::in);
	if (ficheroEntrada.fail())
	{
		ficheroSalida.open(DAT_USUARIOS);
		ficheroSalida << "ADMIN" << endl;
		ficheroSalida << "Administrador" << endl;
		ficheroSalida << "ADMIN" << endl;
		ficheroSalida << "1" << endl;
		ficheroSalida.close();
		ficheroEntrada.close();
		ficheroEntrada.open(DAT_USUARIOS, ios::in);
	}

	

	while (!ficheroEntrada.eof())
	{
		Usuario* usuario = new Usuario();

		ficheroEntrada.getline(usuario->nick, 8);

		if (ficheroEntrada.eof())
		{
			ficheroEntrada.close();
			break;
		}

		ficheroEntrada.getline(usuario->nombre, 40);
		ficheroEntrada.getline(usuario->password, 6);
		char *tipo= new char();
		ficheroEntrada.getline(tipo, 2);
		usuario->tipo = tipo[0];

		usuarios.push_back(usuario);


	}



	ficheroEntrada.close();
}

void cargar_cuentas()
{
	//crear_archivo_si_no_existe(DAT_CUENTAS);

	ficheroEntrada.open(DAT_CUENTAS, ios::in);

	while (!ficheroEntrada.eof())
	{
		
		Cuenta* cuenta = new Cuenta();
		ficheroEntrada.getline(cuenta->numero, 5);
		if (ficheroEntrada.eof())
		{
			ficheroEntrada.close();
			break;
		}
		ficheroEntrada.getline(cuenta->nombre_persona, 40);
		getline(ficheroEntrada, cuenta->cedula);
		getline(ficheroEntrada, cuenta->direccion);
		getline(ficheroEntrada, cuenta->telefono);
		getline(ficheroEntrada, cuenta->email);

		string saldo;
		getline(ficheroEntrada, saldo);
		cuenta->saldo = atoi(saldo.c_str());

		char* estado = new char();
		ficheroEntrada.getline(estado, 2);
		cuenta->estado = estado[0];

		ficheroEntrada.getline(cuenta->user, 8);
		
		cuentas.push_back(cuenta);
		
	}

	ficheroEntrada.close();
	
}

void cargar_movimientos()
{
	crear_archivo_si_no_existe(DAT_MOVIMIENTOS);
}

int menus(int ini, int fin, int tipo)
{
	int tecla;
	bool error = true;

	int intentos = 0;
	lista_menu(tipo);
	do {
		tecla = _getch();

		if (tecla >= ini+48 && tecla <= fin+48 || tecla == 27)
		{
			error = false;
			return tecla-48;
		}
		else {
			intentos++;
			gotoxy(30, 20 + intentos); cout << ("Porfavor seleccione un numero del menu");
			if (intentos == 4) {
				intentos = 0;
				return tecla;
			}
		}

	} while (error);
}

void formulario(int tipo)
{
	char h = 196;
	char v = 179;
	char esi = 218;
	char esd = 191;
	char t = 180;
	char t2 = 195;
	char u = 233;
	char eii = 192;
	char eid = 217;
	char o = 162;
	char ene = 164;
	int y = 2;


	if (tipo == 1)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "          Crear Usuario				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Usuario:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Nombre: 							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Contrase" << ene << "a:			"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Tipo:  				            "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "        Accede a tu usuario        ";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}


	if (tipo == 2)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "          Buscar Usuario				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Usuario:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "                                    ";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}

	if (tipo == 3)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "          Crear Cuenta				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Numero:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Nombre Persona: 					"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Cedula: 							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Direccion:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Telefono: 							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Email: 							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Usuario: 							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "        Accede a tu usuario        ";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}

	if (tipo == 4)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "          Buscar Cuenta				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Numero:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "                                    ";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}
}

void lista_menu(int tipo)
{

	char h = 196;
	char v = 179;
	char esi = 218;
	char esd = 191;
	char t = 180;
	char t2 = 195;
	char u = 233;
	char eii = 192;
	char eid = 217;
	char o = 162;
	char ene = 164;
	int y = 2;


	if (tipo == 0)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "               Login                " << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  Usuario:							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "   				                    "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Contrase" << ene << "a:			"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "        Accede a tu usuario        ";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}


	if (tipo == 1)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "       Panel de Administrador        "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Usuario: " << logged_user->nombre; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  1-> Agregar Usuario				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  2-> Modificar Usuario              "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 3-> Eliminar Usuario				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 4-> Lista de Usuarios				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "  ESC-> Cerrar Sesion				";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}

	if (tipo == 2)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "         Modificar Usuario         "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Usuario: "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  1-> Nombre 				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 2-> Contrase" << ene << "a			"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 3-> Tipo							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "  ESC-> Salir				";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}


	if (tipo == 3)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "         Panel de Cuentas		    "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Usuario: " << logged_user->nombre; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  1-> Agregar Cuenta					"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  2-> Modificar Cuenta				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 3-> Bloquear Cuenta				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 4-> Consultar Cuenta				"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "  ESC-> Cerrar Sesion				";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}


	if (tipo == 4)
	{
		gotoxy(30, ++y); cout << esi << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << esd;
		gotoxy(30, ++y); cout << v << "         Modificar Usuario         "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 Usuario: "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "  1-> Nombre 				        "; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 2-> Cedula							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 3-> Direccion						"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 4-> Telefono						"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "	 5-> Email							"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << v << "										"; gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << t2 << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << t;
		gotoxy(30, ++y); cout << v << "  ESC-> Salir				";  gotoxy(67, y); cout << v;
		gotoxy(30, ++y); cout << eii << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << h << eid;
	}
}

char *leer_letras()
{
	int i = 0;
	char tecla;
	char* p = new char();



	p[0] = '\0';
	do
	{
		tecla = getcht();

		if (i > 0 && tecla == 8)
		{
			cout << ("\b \b");
			p[--i] = '\0';
		}
		else
		{
			/*rango de letras mayusculas y minusculas y 32 es el espacio*/
			if ((tecla >= 65 && tecla <= 95) ||
				(tecla >= 97 && tecla <= 122) ||
				tecla == 32)
			{
				cout << tecla;
				p[i++] = tecla;
			}
		}
	} while ((tecla != 13 || p[0] == '\0'));
	/* 13 inidica el Enter, i tiene que ser menor
	que TAM pues de lo contrario, estarÃƒ­a accediendo
	a memoria no reservada*/

	p[i] = '\0'; /*colocamos el fin de cadena*/

	return (p);
}

string leer_password()
{
	int i = 0;
	char tecla;
	char* p = new char();



	p[0] = '\0';
	do
	{
		

		tecla = getcht();
		if (tecla == 13)break;

		if (i > 0 && tecla == 8)
		{
			cout << ("\b \b");
			p[--i] = '\0';
		}
		else
		{
			/*rango de letras mayusculas y minusculas y 32 es el espacio*/
			if (tecla != 8)
			{
				cout << "*";
				p[i++] = tecla;
			}
		}
	} while ((tecla != 13 || p[0] == '\0'));
	/* 13 inidica el Enter, i tiene que ser menor
	que TAM pues de lo contrario, estarÃƒ­a accediendo
	a memoria no reservada*/

	p[i] = '\0'; /*colocamos el fin de cadena*/

	return string(p);
}

char *leer_numeros()
{
	int i;
	char tecla;
	char* p = new char();
	/*importante inicializar los valores
	ya que la implementacion que hice lo amerita*/
	i = 0;
	p[0] = '\0';

	do
	{
		tecla = getcht();//atrapa un caracter sin dejar un eco

		/*8 es la tecla para eliminar un caracter
		y el i obligaotiamente tiene que ser mayor que cero
		ya que si entra al if y tiene valor cero, el valor de i
		decrementarÃ¯¿½ y p tendrÃ¯¿½a un subindice -1, "p[-1]"Ã¯¿½?*/
		if (i > 0 && tecla == 8)
		{
			cout << ("\b \b");
			p[--i] = '\0';
		}
		else
		{
			/*rango de letras mayusculas y minusculas y 32 es el espacio*/
			if ((tecla >= 48 && tecla <= 57))
			{
				cout << tecla;
				p[i++] = tecla;
			}
		}
	} while ((tecla != 13 || p[0] == '\0'));
	/* 13 indica el Enter, i tiene que ser menor
	que TAM pues de lo contrario, estarÃ¯¿½a accediendo
	a memoria no reservada*/

	p[i] = '\0'; /*colocamos el fin de cadena*/


	return p;
}

char getcht()
{
	char car;
	DWORD leidos, modo;

	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &modo);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), modo &
		!ENABLE_ECHO_INPUT & !ENABLE_PROCESSED_INPUT);
	ReadConsole(GetStdHandle(STD_INPUT_HANDLE), &car, 1, &leidos, NULL);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), modo);
	return car;
}

void gotoxy(int x, int y)
{
	HANDLE manipulador;
	COORD Coordenada;

	manipulador = GetStdHandle(STD_OUTPUT_HANDLE);
	Coordenada.X = x;
	Coordenada.Y = y;

	SetConsoleCursorPosition(manipulador, Coordenada);

}

void crear_archivo_si_no_existe(const char* txt)
{
	ficheroEntrada.open(txt, ios::in);
	if (ficheroEntrada.fail())
	{
		ficheroSalida.open(txt);
		ficheroSalida.close();
	}
	ficheroEntrada.close();
}
