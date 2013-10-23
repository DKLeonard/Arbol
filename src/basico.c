#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#define MAX_STR 50


typedef struct _persona {
    char apellido [20];
    char nombre   [20];
    struct _persona *sig;
} Persona;

typedef struct _familia {
    char  nomb_Familia [20];
    char  madre[MAX_STR];
    char  padre[MAX_STR];
    Persona *Hijos [5];
    struct _familia *familia_padre;
    struct _familia *familia_hija;
    struct _familia *sig;
    GtkWidget* label;
    int x;
    int y;
} Familia;

void formar_arbol(Familia* );
void buffer(void);
Persona *Buscar(char *, Persona *);
Persona * Crear(Persona *,char *,char *,char *);
void Reemplazar(Persona *,char *,char *,char *);
void Imprimir(Persona *);
void Imprimir_fam(Familia *);
void Recortar(char[50],char[50],int);
Familia * Crear_familia(Persona *,char *,char *,char *);

void insertText(GtkTextBuffer *buffer, const gchar *text ){
	GtkTextIter* iter;
	gtk_text_buffer_get_end_iter(buffer,iter);
	gtk_text_buffer_insert(buffer,iter,text,-1);
}
GtkWidget* createMemberArea(GtkWidget *container)
{
    GtkWidget* textArea               =  gtk_text_view_new();
    GtkWidget* scrolledwindow         =  gtk_scrolled_window_new(NULL, NULL);
    GtkWidget* memberLabel            =  gtk_label_new("INTEGRANTES");
    GtkTextTagTable *table            =  gtk_text_tag_table_new();
	GtkTextBuffer *buffer             =  gtk_text_buffer_new(table);
	
	gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW(scrolledwindow),180);
	gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW(scrolledwindow),100);
	
    gtk_container_add(GTK_CONTAINER(scrolledwindow),(textArea));
 
	gtk_fixed_put(GTK_FIXED(container), GTK_WIDGET(scrolledwindow), 10, 70);
	gtk_fixed_put(GTK_FIXED(container), GTK_WIDGET(memberLabel), 40, 50);
    
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textArea),buffer);
    
    return GTK_WIDGET(buffer);
}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, const gchar *nombre)
{
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 

	cairo_select_font_face(cr, "Purisa",
	  CAIRO_FONT_SLANT_NORMAL,
	  CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size(cr, 13);

	cairo_move_to(cr, 20, 30);
	cairo_show_text(cr, nombre);  
	

 return FALSE;
}

static void print_hello (GtkWidget *widget,gpointer   data)
{
  g_print ("Hello World\n");
}

void config_window(GtkWindow *window,int width,int height,const gchar *title, gboolean resizable){
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (window), title);
	gtk_window_set_default_size(GTK_WINDOW(window), width, height);
	gtk_window_set_resizable (GTK_WINDOW(window), resizable);
	}
	
static int WIDTH  = 600;
static int HEIGHT = 400;

int main(int   argc,char *argv[]) {
	//~ Se define un archivo para trabajarlo
    FILE *archivo; 
    //~ Creamos dos variables de Persona Ref tiene la estructura inicial del problema y Pers indica el elemento actual
    Persona *personas =(Persona*)malloc (sizeof(Persona));
	Persona *auxp;
	Familia *familias=(Familia*)malloc(sizeof(Familia));
	Familia *auxf;
    
    
    int empieza=0,empieza_fam=0;
    char linea[50];
    char aux[50];
    char padre[50];
    char hijo[50];
    char madre[50];
    char apellido[MAX_STR];
    char nombre_familia[50];
    
    //~ creo las variables de nombre que ayudaran en el proceso
    archivo = fopen("Test.txt","r");
    while (feof(archivo) ==0) {
        fgets(linea,50,archivo); // Se le la linea que dice "Nombre de la familia"
        
        printf("Linea1: %s",linea);
		fgets(linea,50,archivo); // Se lee la linea que tiene los apellidos de la familia
		strcpy(familias->nomb_Familia,linea);
		printf("Linea2: %s",linea);
		fgets(linea,50,archivo); // Se lee la linea que dice "Integrantes"
		
		fgets(linea,50,archivo); // se lee la linea que dice el nombre del padre
		Recortar(linea,"Padre: ",7);
		strcpy(familias->padre,linea);
		printf("Linea3: %s",linea);
		fgets(linea,50,archivo); // se lee la linea que dice el nombre de la madre
		strcpy(familias->madre,linea);
		printf("Linea4: %s",linea);
		fgets(linea,50,archivo); // se lee la linea que dice los nombres de los hijos
		printf("Linea5: %s",linea);
		
		fgets(linea,50,archivo); // se lee el espacio
	}

    fclose(archivo);
    
    GtkWidget *window;  
	GtkWidget *button;
	GtkWidget *fixed_container;
	GtkWidget *fixed_container_tree;
	GtkWidget *toolbar;
	GtkWidget *drawing_area;
	GtkToolItem *open;
	GtkToolItem *sep;
	GtkToolItem *exit;
	GtkWidget *scroll;
	GtkWidget *memberTextArea;
	
	gtk_init (&argc, &argv);

	// Creacion de Ventana
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	config_window(GTK_WINDOW(window),WIDTH,HEIGHT,"Arbol Genealogico",FALSE);
	
	// Configuracion de Container
	fixed_container =  gtk_fixed_new ();
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);
	gtk_container_add (GTK_CONTAINER (window), fixed_container);
	
	// Configuracion de la barra de menu
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS );
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 0);
	
	// Configurar boton open
	open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);
	//g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(open_file), NULL);
	
	// Agregar separador
	sep = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 
	
	// Configurar boton exit
	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
	g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	
	// Configuracion de la seccion que contiene el arbol 
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_fixed_put(GTK_FIXED(fixed_container), GTK_WIDGET(scroll), 200, 50);
	gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW(scroll),WIDTH);
	gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW(scroll),HEIGHT - 50);
	

	// Configuracion del area del arbol
	fixed_container_tree =  gtk_fixed_new ();
	gtk_widget_set_size_request (GTK_WIDGET(fixed_container_tree), 1000, 1000);
	drawing_area = gtk_drawing_area_new();
	
	
	// configurar el area de miembros de la familia
	memberTextArea = createMemberArea(GTK_WIDGET(fixed_container));
	
	
	// Agregar elementos al area del arbol
	gtk_fixed_put(GTK_FIXED(fixed_container_tree), GTK_WIDGET(drawing_area), 0, 0);
	


	// se agrega la barra de herramientas a la ventana
	gtk_fixed_put(GTK_FIXED(fixed_container), toolbar, 0, 0);
	gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 1000, 1000);
	g_signal_connect (G_OBJECT (drawing_area), "draw",
                    G_CALLBACK (draw_callback), NULL);
    
	gtk_container_add(GTK_CONTAINER(scroll), GTK_WIDGET(fixed_container_tree));

	
	
	// se conecta la senial "destroy" con la funcion gtk_main_quit
	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	// los widget GTK estan escondidos por defecto, con esto los volvemos visibles
	gtk_widget_show_all (window);
	
	
	
	gtk_main ();
    
    return 0;
}

void formar_arbol(Familia* listado){
	Familia* tmp = listado;
	Familia* primero = listado;
	Familia* padre;
	
	while (listado!= NULL){
		printf("Nombre de Familia %s",listado->nomb_Familia);

		
		listado = listado->sig;
		}
	listado = tmp;
	}
/* Listado es el primer elemento de la lista
   nodo hijo, es el nodo al que se le busca padre */
void buscar_padre(Familia* nodo_hijo,Familia* listado){
	Familia* tmp = listado;
	while (listado!= NULL){
		listado = listado->sig;
		
		}
	listado = tmp;
	}

void Dibujar(Familia* nodos){
	
}
void Recortar(char string[MAX_STR],char substr,int base){
	int i=0;
	while(base<strlen(string) && string[base]!='\n' && string[base]!='\0'){
		substr[i]=string[base];
		base++;
		i++;
	}
	substr[base+i] = '\0';
	
	
	
}

void buffer(void) {
    while(getchar()!='\n') {}
}
