// Examen3.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "Examen3.h"
#include <fstream>
#include <CommDlg.h>

using namespace std;

#define MAX_LOADSTRING 100

struct nodo_boletos {
    char evento[25], promo[25];
    float subtotal, total;
    nodo_boletos* ant;
    nodo_boletos* sig;
};
nodo_boletos* aux = 0, * lista_eventos = 0, * ultimo_evento = 0, * nuevo_evento = 0;

char arch_boletos[] = "boletos2.dat";
char mensaje[50] = { 0 };
char nombrearchivo[MAX_PATH] = { 0 };
bool _login = true;

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Login(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Compras(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Historial(HWND, UINT, WPARAM, LPARAM);

void agregar_evento(nodo_boletos* aux);
void leer_archivo(char* archivo);
void escribir_archivo(char* archivo);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXAMEN3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXAMEN3));

    // leer el archivo
    leer_archivo(arch_boletos);

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // guardar el archivo con cada nodo de la lista ligada
    escribir_archivo(arch_boletos);

    return (int)msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXAMEN3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EXAMEN3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case ID_ARCHIVO_LOGIN:

                DialogBox(
                    hInst,
                    MAKEINTRESOURCE(IDD_LOGIN),
                    hWnd,
                    Login);
                break;

            case ID_ARCHIVO_COMPRAS:
                if (_login) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_COMPRAS), hWnd, Compras);
                }
                else
                {
                    MessageBox(0, "Debe conectarse primero", "ERROR",
                        MB_ICONEXCLAMATION + MB_OK);
                }
                break;

            case ID_ARCHIVO_HISTORIAL:
                if (_login) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_HISTORIAL), hWnd, Historial);
                }
                else
                {
                    MessageBox(0, "Debe conectarse primero", "ERROR",
                        MB_ICONEXCLAMATION + MB_OK);
                }
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK Compras(
    HWND hDlg,
    UINT message,   
    WPARAM wParam,
    LPARAM lParam)
{

    static HWND HWevento;
    static HWND HWpromo;
    static HWND HWsubtotal;
    static HWND HWtotal;

    nodo_boletos evento_temp;
    char evento[20] = { 0 };
    char promo[20] = { 0 };
    char subtotal[20] = { 0 };
    char total[20] = { 0 };

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:

            SendDlgItemMessage(hDlg, IDC_COMBOEVENTO, CB_ADDSTRING, 0, (LPARAM)"Concierto Banamex");
            SendDlgItemMessage(hDlg, IDC_COMBOEVENTO, CB_ADDSTRING, 0, (LPARAM)"Concierto BBVA");
            SendDlgItemMessage(hDlg, IDC_COMBOEVENTO, CB_ADDSTRING, 0, (LPARAM)"Concierto Arena Monterrey");

            SendDlgItemMessage(hDlg, IDC_COMBOPROMO, CB_ADDSTRING, 0, (LPARAM)"Promo 2x1");
            SendDlgItemMessage(hDlg, IDC_COMBOPROMO, CB_ADDSTRING, 0, (LPARAM)"10% de Descuento");

            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_ADD && HIWORD(wParam) == BN_CLICKED) {
                if (LOWORD(wParam) == IDC_ADD
                    && HIWORD(wParam) == BN_CLICKED)
                {
                    HWevento = GetDlgItem(hDlg, IDC_COMBOEVENTO);
                    HWpromo = GetDlgItem(hDlg, IDC_COMBOPROMO);

                    SendMessage(HWevento, WM_GETTEXT, 20, (LPARAM)evento_temp.evento);
                    SendMessage(HWpromo, WM_GETTEXT, 20, (LPARAM)evento_temp.promo);

                    evento_temp.subtotal = atoi(subtotal);
                    evento_temp.total = atoi(total);

                    nodo_boletos* nuevo = new nodo_boletos;
                    nuevo->subtotal = evento_temp.subtotal;


                    int eventoIndex = SendMessage(HWevento, CB_GETCURSEL, 0, 0);
                    if (eventoIndex != CB_ERR) {
                        char eventoSeleccionado[256]; // Definir un buffer para el texto seleccionado
                        SendMessage(HWevento, CB_GETLBTEXT, eventoIndex, (LPARAM)eventoSeleccionado);

                        // Comparar el texto seleccionado con una palabra específica
                        if (strcmp(eventoSeleccionado, "Concierto Banamex") == 0) {

                            SendMessage(GetDlgItem(hDlg, IDC_SUBTOTAL), WM_SETTEXT, 0, (LPARAM)"$100");


                        }
                        if (strcmp(eventoSeleccionado, "Concierto BBVA") == 0) {

                            SendMessage(GetDlgItem(hDlg, IDC_SUBTOTAL), WM_SETTEXT, 0, (LPARAM)"$250");

                        }
                        if (strcmp(eventoSeleccionado, "Concierto Arena Monterrey") == 0) {

                            SendMessage(GetDlgItem(hDlg, IDC_SUBTOTAL), WM_SETTEXT, 0, (LPARAM)"$200");

                        }
                    }
                }
            }
            else if (LOWORD(wParam) == IDC_ADDPROMO && HIWORD(wParam) == BN_CLICKED) {
                int eventoIndex = SendMessage(HWevento, CB_GETCURSEL, 0, 0);
                if (eventoIndex != CB_ERR) {

                    char eventoSeleccionado[256]; // Definir un buffer para el texto seleccionado
                    SendMessage(HWevento, CB_GETLBTEXT, eventoIndex, (LPARAM)eventoSeleccionado);

                    if (strcmp(eventoSeleccionado, "Concierto Banamex") == 0) {

                        int promoIndex = SendMessage(HWpromo, CB_GETCURSEL, 0, 0);
                        if (promoIndex != CB_ERR) {
                            char promoSeleccionado[256]; // Definir un buffer para el texto seleccionado
                            SendMessage(HWpromo, CB_GETLBTEXT, promoIndex, (LPARAM)promoSeleccionado);

                            if (strcmp(promoSeleccionado, "Promo 2x1") == 0) {

                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$58");

                            }
                            if (strcmp(promoSeleccionado, "%10 de Descuento") == 0) {
                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$104.4");

                            }
                        }

                    }
                    if (strcmp(eventoSeleccionado, "Concierto BBVA") == 0) {

                        int promoIndex = SendMessage(HWpromo, CB_GETCURSEL, 0, 0);
                        if (promoIndex != CB_ERR) {
                            char promoSeleccionado[256]; // Definir un buffer para el texto seleccionado
                            SendMessage(HWpromo, CB_GETLBTEXT, promoIndex, (LPARAM)promoSeleccionado);

                            if (strcmp(promoSeleccionado, "Promo 2x1") == 0) {

                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$145");

                            }
                            if (strcmp(promoSeleccionado, "%10 de Descuento") == 0) {
                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$261");

                            }
                        }

                    }
                    if (strcmp(eventoSeleccionado, "Concierto Arena Monterrey") == 0) {

                        int promoIndex = SendMessage(HWpromo, CB_GETCURSEL, 0, 0);
                        if (promoIndex != CB_ERR) {
                            char promoSeleccionado[256]; // Definir un buffer para el texto seleccionado
                            SendMessage(HWpromo, CB_GETLBTEXT, promoIndex, (LPARAM)promoSeleccionado);

                            if (strcmp(promoSeleccionado, "Promo 2x1") == 0) {

                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$116");

                            }
                            if (strcmp(promoSeleccionado, "%10 de Descuento") == 0) {
                                SendMessage(GetDlgItem(hDlg, IDC_TOTAL), WM_SETTEXT, 0, (LPARAM)"$208.8");

                            }
                        }
                    }
                }

            }
            else if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }

    return (INT_PTR)FALSE;
}

    INT_PTR CALLBACK Historial(
        HWND hDlg,
        UINT message,
        WPARAM wParam,
        LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        nodo_boletos* temp = 0;
        int idx = 0;

        idx = 0;
        aux = lista_eventos;
        while (aux != 0)
        {
            // por cada nodo, agregar al LISTBIOX             ​
            SendDlgItemMessage(hDlg, IDC_LISTA, LB_ADDSTRING,
                0,
                (LPARAM)aux->evento);
            idx++;
            aux = aux->sig;
        }

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }

    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND HWuser;
    static HWND HWpass;
    char user[10] = {};
    char pass[20] = {};
    char texto[100] = {};
    char u[10] = "Diego";
    char p[20] = "123";

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_OK && HIWORD(wParam) == BN_CLICKED)
        {
            // Leer el contenido de los EditControl
            // Buscar en el Archivo - Usuarios
            // si coincide, hacer true a la variable _login
            HWuser = GetDlgItem(hDlg, IDC_USER);
            HWpass = GetDlgItem(hDlg, IDC_PASS);
            SendMessage(HWuser, WM_GETTEXT, 20, (LPARAM)user);
            SendMessage(HWpass, WM_GETTEXT, 20, (LPARAM)pass);
            if ((strcmp(user, u) == 0) && (strcmp(pass, p) == 0)) {
                _login = true;
            }
            if (_login) {
                strcpy_s(texto, "Bienvenido al sistema de compra de boletos: ");
                strcat_s(texto, user);
                MessageBox(0, texto, "MENSAJE",
                    MB_ICONINFORMATION + MB_OK);
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else {
                MessageBox(0, "Usuario no existe y/o contraseña incorrecta", "MENSAJE",
                    MB_ICONERROR + MB_OK);
                return (INT_PTR)TRUE;
            }
        }
        if (LOWORD(wParam) == IDC_CERRAR && HIWORD(wParam) == BN_CLICKED)
        {
            // Leer el contenido de los EditControl
            // Buscar en el Archivo - Usuarios
            // si coincide, hacer true a la variable _login
            _login = false;
            MessageBox(0, "Usuario no existe y/o contraseña incorrecta", "MENSAJE",
                MB_ICONERROR + MB_OK);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void agregar_evento(nodo_boletos* aux) {
    aux->ant = 0;
    aux->sig = 0;
    if (lista_eventos == 0) { // está vacía
        lista_eventos = aux;
    }
    else { // no está vacía
        ultimo_evento->sig = aux;
        aux->ant = ultimo_evento;
    }
    ultimo_evento = aux;

}

void leer_archivo(char* archivo) {
    ifstream arch_boletos;
    arch_boletos.open(archivo, ios::binary);

    if (arch_boletos.fail())
    {
        MessageBox(0, "No se abrio el archivo", "ERROR",
            MB_ICONERROR + MB_OK);
    }
    if (arch_boletos.is_open())
    {
        // leer el archivo, hasta el fin 
        nuevo_evento = new nodo_boletos;
        arch_boletos.read((char*)nuevo_evento, sizeof(nodo_boletos));
        while (!arch_boletos.eof())
        {
            aux = new nodo_boletos;
            aux = nuevo_evento;

            agregar_evento(aux);

            nuevo_evento = new nodo_boletos;
            arch_boletos.read((char*)nuevo_evento, sizeof(nodo_boletos));
        }
        arch_boletos.close();
        //printf("archivo leído, lista ligada de alumnos creada\n");
        //system("pause");
    }
};

void escribir_archivo(char* archivo) {
    ofstream archi;
    archi.open(archivo, ios::trunc | ios::binary);

    if (archi.fail())
    {
        MessageBox(0, "No se abrio el archivo", "ERROR",
            MB_ICONERROR + MB_OK);
    }

    if (archi.is_open())
    {
        // recorrer toda la lista ligada, nodo x nodo
        aux = lista_eventos;
        while (aux != 0)
        {
            //guardar en el archivo cada nodo
            archi.write((char*)aux, sizeof(nodo_boletos));

            //leer el siguiente nodo
            aux = aux->sig;
        }
        archi.close();
        //printf("Archivo guardado\n\n");
    }
};