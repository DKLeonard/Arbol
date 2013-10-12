#include <gtk/gtk.h>
qweqweqwewqe
static void print_hello (GtkWidget *widget,gpointer   data)
{
  g_print ("Hello World\n");
}


int main (int   argc,char *argv[])
{
	GtkWidget *window;
	GtkWidget *button;  
	GtkWidget *grid;

	gtk_init (&argc, &argv);
	// Creacion de Ventana
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Arbol Genealogico");
	
	// se conecta la senial "destroy" con la funcion gtk_main_quit
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

        // Configuracion de Container
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	grid = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (window), grid);
	

	// Configuracion Boton 1
	button = gtk_button_new_with_label ("Boton 1");
	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
	
	// Configuracion Boton 2
	button = gtk_button_new_with_label ("Button 2");
  	g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
	
	button = gtk_button_new_with_label ("Quit");	  
	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);	


	// los widget GTK estan escondidos por defecto, con esto los volvemos visibles
	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}
