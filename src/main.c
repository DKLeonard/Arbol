#include <gtk/gtk.h>


static void print_hello (GtkWidget *widget,gpointer   data)
{
  g_print ("Hello World\n");
}
static void open_file (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World\n");
}

static int WIDTH  = 600;
static int HEIGHT = 400;

int main (int   argc,char *argv[])
{
	GtkWidget *window;  
	GtkWidget *fixed_layout;
	GtkWidget *toolbar;
	GtkToolItem *open;
	GtkToolItem *sep;
	GtkToolItem *exit;

	gtk_init (&argc, &argv);
	// Creacion de Ventana
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (window), "Arbol Genealogico");
	gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
	gtk_window_set_resizable (GTK_WINDOW(window), TRUE);
	
	// Configuracion de Container
	fixed_layout =  gtk_fixed_new ();
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);
	gtk_container_add (GTK_CONTAINER (window), fixed_layout);
	
	// Configuracion de la barra de menu
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS );
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 0);
	
	// Configurar boton open
	open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);
	g_signal_connect(G_OBJECT(open), "clicked", G_CALLBACK(open_file), NULL);
	
	// Agregar separador
	sep = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 
	
	// Configurar boton exit
	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
	g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit), NULL);
		
	// se agrega la barra de herramientas a la ventana
	gtk_fixed_put(GTK_FIXED(fixed_layout), toolbar, 0, 0);
	
	// se conecta la senial "destroy" con la funcion gtk_main_quit
	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	// los widget GTK estan escondidos por defecto, con esto los volvemos visibles
	gtk_widget_show_all (window);

	gtk_main ();
	

	return 0;
}
