#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR 50

enum{
   DATO,
   PUNTERO,
   N_COLUMNS
};

typedef struct _persona {
    struct _persona * Madre;
    struct _persona * Padre;
	struct _familia * familia;
    char nombre [20];
    struct _persona *sig;
} Persona;
typedef struct _familia {
	int x;
	int y;
    char  nomb_Familia [20];
    Persona *Madre;
    Persona *Padre ;
    Persona *Hijos [5];
    struct _familia *sig;
    struct _familia *derecha;
    struct _familia *izquierda;
    struct _familia *ancestro;
} Familia;


GtkWidget* setup_tree ();
void show_member_info(GtkWidget *,gpointer *data);
void add_family_to_list (GtkWidget *tree, Familia *familia);
void add_member_to_list (GtkWidget *tree, Persona *people);
void Reemplazar(Persona *,char *,char *,char *);
void Recortar(char[50],char[50],int,int);
void draw_node(GtkWidget *,cairo_t *, Familia *);
void show_family_info(GtkWidget *);
void set_position_to_left_tree(Familia *,int , int );
void set_position_to_right_tree(Familia *,int , int );
void open_file(Persona *, Familia *);
void set_position(Familia *);
void add_to_list(GtkWidget*,Familia* );
void make_tree(Familia *);
void draw_tree(GtkWidget *, cairo_t *, Familia *);
void config_window(GtkWindow *,int ,int ,const gchar *, gboolean );
Persona* Buscar(char *, Persona *);
Persona* Crear(Persona *,char *,char *,char *);
Familia* Crear_familia(Persona *,char *,char *,char *);
Familia* find_root(Familia *);


static int WIDTH  = 600;
static int HEIGHT = 400;
GtkWindow *main_window;

int main (int   argc,char *argv[])
{
	Persona p;
	Familia f;
	open_file(&p,&f);
	make_tree(&f);
	set_position(&f);
	
	GtkWidget *window;  
	GtkWidget *paned_container_horizontal;
	GtkWidget *paned_container_vertical;
	GtkWidget *fixed_container;
	GtkWidget *list_container;
	GtkWidget *drawing_area;
	GtkWidget *scroll_draw_zone;
	GtkWidget *scroll_tree_zone;
	GtkWidget *scroll_list_zone;
	GtkWidget *memberTextArea;
	GtkWidget *tree_box;
	GtkWidget *list_box;
	
	gtk_init (&argc, &argv);

	// Configuracion de Ventana
	main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	window = main_window;
	config_window(GTK_WINDOW(window),WIDTH,HEIGHT,"Arbol Genealogico",TRUE);
	// Configuracion de ventana que muestra los datos de las familias

	
	// Configuracion de Container
	paned_container_horizontal =  gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
	paned_container_vertical   =  gtk_paned_new (GTK_ORIENTATION_VERTICAL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);
	gtk_container_add (GTK_CONTAINER (window), paned_container_horizontal);
	fixed_container = gtk_fixed_new();
	list_container = gtk_fixed_new();
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request (GTK_WIDGET(drawing_area), 2000, 2000);

	scroll_draw_zone = gtk_scrolled_window_new(NULL, NULL);
	scroll_list_zone = gtk_scrolled_window_new(NULL, NULL);
	scroll_tree_zone = gtk_scrolled_window_new(NULL, NULL);
	
	gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW(scroll_draw_zone),800);
	gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW(scroll_draw_zone),600 - 50);
	
	
	gtk_fixed_put(GTK_FIXED(fixed_container), GTK_WIDGET(scroll_draw_zone), 0, 20);
	gtk_fixed_put(GTK_FIXED(list_container), GTK_WIDGET(scroll_draw_zone), 0, 0);
	
	
	// Se configura la caja que contiene a las familias
	tree_box = setup_tree("FAMILIAS");
	add_family_to_list(tree_box,&f);
	// Se configura la caja que muestr a las personas
	list_box = setup_tree("PERSONAS");
	add_member_to_list(list_box,&p);
	
	gtk_container_add(GTK_CONTAINER(scroll_draw_zone), GTK_WIDGET(drawing_area));
	gtk_container_add(GTK_CONTAINER(scroll_tree_zone), GTK_WIDGET(tree_box));
	gtk_container_add(GTK_CONTAINER(scroll_list_zone), GTK_WIDGET(list_box));
	
	// Se agregan los elementos a la ventana
	gtk_paned_add1(GTK_PANED(paned_container_vertical),scroll_tree_zone);
	gtk_paned_add2(GTK_PANED(paned_container_vertical),scroll_list_zone);

	gtk_paned_add1(GTK_PANED(paned_container_horizontal),paned_container_vertical);
	gtk_paned_add2(GTK_PANED(paned_container_horizontal),fixed_container);

	
	// se conecta la senial "destroy" con la funcion gtk_main_quit
	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_tree), &f);
	g_signal_connect(G_OBJECT(tree_box), "row-activated", G_CALLBACK(show_family_info),GTK_WINDOW(window));
	g_signal_connect(G_OBJECT(list_box), "row-activated", G_CALLBACK(show_member_info),NULL);
	
	// los widget GTK estan escondidos por defecto, con esto los volvemos visibles
	gtk_widget_show_all (window);
	gtk_main ();	
	return 0;
}
Persona* find_oldest(Persona* person){
	printf("1111111111");
	if (person != NULL){
		if (person->Padre != NULL && person->Madre != NULL){
			if (person->Padre == ((person->familia)->ancestro)->Padre || person->Madre == ((person->familia)->ancestro)->Madre ){
				printf("TIENE PADRE Y MADRE\n");
				return (find_root(person->familia))->Padre;
			}
		}
		else {
			printf("NO TIENE ANCESTROS\n");
			return person;
		}
	}
	printf("22222");
	return person;
}
	
int count_progeny(Familia *familia){
	Familia *first = familia;
	// Falta comprobar si la familia tiene hijos pero no familia, eso cuenta como una generacion mas
	int left  = 0;
	int right = 0;
	if (familia != NULL){
		left  = count_progeny(familia->izquierda);
		right = count_progeny(familia->izquierda);
		familia = first;
		if (left > right)
			return left+1;
		else
			return right+1;
		}
	return 0;
	}
 /* Funcion que muestra la informacion de una familia al hacer
  * doble click sobre ella en la lista (tree view) */
void show_member_info(GtkWidget *widget,gpointer *data) {	
	GtkWidget *dialog;
	Persona *selected;
	Persona *oldest;
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	int count = 0;
	
	/* Se obtiene que familia esta seleccionada en la tree view y 
     * se guarda el puntero a ella en selected*/
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
	gtk_tree_selection_get_selected (selection, &model, &iter);
	gtk_tree_model_get (GTK_TREE_MODEL(model), &iter, 
						PUNTERO, &selected,
						-1);
	//oldest = find_oldest(selected);
	count = count_progeny(selected->familia);
	printf("Cuenta es %d",count);
	dialog = gtk_message_dialog_new((main_window),
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_INFO,
								GTK_BUTTONS_CLOSE,
								"Nombre : %s\nAntepasado mas antiguo : %s\nGeneraciones descendientes: %d",
								selected->nombre,
								selected->Padre->nombre,
								count-1
								);
		
		

	
	
	
	gtk_dialog_run (GTK_DIALOG (dialog));
	/* Destroy the dialog when the user responds to it (e.g. clicks a button) */
	g_signal_connect_swapped (dialog, "response",G_CALLBACK (gtk_widget_destroy), dialog);
	
	gtk_widget_destroy (dialog);
	}



 /* Funcion que muestra la informacion de una familia al hacer
  * doble click sobre ella en la lista (tree view) */
void show_family_info(GtkWidget *widget){	
	GtkWidget *dialog;
	Familia *selected;
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	
	
	/* Se obtiene que familia esta seleccionada en la tree view y 
     * se guarda el puntero a ella en selected*/
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
	gtk_tree_selection_get_selected (selection, &model, &iter);
	gtk_tree_model_get (GTK_TREE_MODEL(model), &iter, 
						PUNTERO, &selected,
						-1);
	dialog = gtk_message_dialog_new((main_window),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_INFO,
						 GTK_BUTTONS_CLOSE,
						 "Familia : %s\nPadre : %s\nMadre: %s",
						 selected->nomb_Familia,
						 selected->Padre->nombre,
						 selected->Madre->nombre
						 );
	
	gtk_dialog_run (GTK_DIALOG (dialog));
	/* Destroy the dialog when the user responds to it (e.g. clicks a button) */
	g_signal_connect_swapped (dialog, "response",G_CALLBACK (gtk_widget_destroy), dialog);
	
	gtk_widget_destroy (dialog);
	}


	
void draw_tree(GtkWidget *widget,cairo_t *cr, Familia *node){
	cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
	cairo_select_font_face(cr, "Purisa",
							CAIRO_FONT_SLANT_NORMAL,
							CAIRO_FONT_WEIGHT_BOLD);

	cairo_set_font_size(cr, 13);
	cairo_move_to(cr, node->x, node->y);
	cairo_show_text(cr, node->nomb_Familia);
	
	if (node->sig != NULL)
		draw_tree(widget,cr,node->sig);
	}



/* Funcion recursiva que recorre el arbol asignando las coordenadas*/
void set_position(Familia *node){
	node->x = 1000;
	node->y = 100;
	set_position_to_left_tree(node->izquierda,node->x -100 ,node-> y + 100);
	set_position_to_right_tree(node->derecha,node->x +100 ,node-> y + 100);
	
	Familia *first = node;
	while(node != NULL){
		printf("Familia : %s\n",node->nomb_Familia);
		if (node->izquierda != NULL)
			printf("Izquierda %s\n",node->izquierda->nomb_Familia);
		if (node->derecha != NULL)
			printf("Derecha %s\n\n",node->derecha->nomb_Familia);
		if (node->ancestro != NULL)
			printf("Ancestro %s\n\n",node->ancestro->nomb_Familia);
		node = node->sig;
		}
	node = first;
	}
void set_position_to_left_tree(Familia *node,int x, int y){
	if (node != NULL){
		node->x = x;
		node->y = y;
		//printf("Familia : %s X = %d, Y = %d\n",node->nomb_Familia,node->x,node->y);
		
		set_position_to_left_tree(node->izquierda,x - 100 , y + 100);
		set_position_to_left_tree(node-> derecha ,x ,       y + 100 );
		
		}
	}
void set_position_to_right_tree(Familia *node,int x, int y){
	if (node != NULL){
		node->x = x;
		node->y = y;
		set_position_to_right_tree(node-> derecha ,x + 100 , y + 100 );
		set_position_to_right_tree(node->izquierda,x ,       y + 100);
		}
	}

/* Se relaciona a las personas con las familias */
void asig_dir_to_family(Familia *list){
	Familia *first = list;
	int i = 0;
	while(list != NULL){
		list->Padre->familia = list;
		list->Madre->familia = list;
		i = 0;
		while( i < 5){
			if (list->Hijos[i] != NULL)
				list->Hijos[i]->familia = list;
			i += 1;
			}
		list = list->sig;
		}
	list = first;
	}
/* Asigna las relaciones generacionales entre las familias */
void make_tree(Familia *list){
	asig_dir_to_family(list);
	Familia *first = list;
	Familia *aux;
	while(list != NULL){
		aux = first;
		while(aux != NULL){
			// Busca la familia padre segun el parentezco de el padre de familia
			int i = 0;	
			while (i < 5){
				
				if ( list->Padre == aux->Hijos[i]){				
					aux->izquierda = list;
					list->ancestro = aux;
					break;
				}
				i+=1;
			}
			// Busca la familia padre segun el parentezco de la madre de familia

			i = 0;
			while (i < 5){
				if ( list->Madre == aux->Hijos[i]){
					aux->derecha = list;
					list->ancestro = aux;
					break;
				}
				i+=1;
			}
			
			aux = aux->sig;
			
			}
		
		
		list = list->sig;
		
		}
	list = first;

	}
/* Encuentra el ancestro mas antiguo que existe entre las familias */
Familia* find_root(Familia *list){
	Familia *first = list;
	Familia *root;
	while(list != NULL){
		if (list->ancestro == NULL){
			root = list;
			}
		list = list->sig;
	}
	list = first;
	return root;
}
/*Lee el archivo y almacena todas las familias y personas contenidas */
void open_file(Persona *personas, Familia *familias){
	//~ Se define un archivo para trabajarlo
    FILE *archivo; 
    //~ Creamos dos variables de Persona Ref tiene la estructura inicial del problema y Pers indica el elemento actual
    Persona *pers,*ref=(Persona*)malloc (sizeof(Persona));
	Familia *ref_fam=(Familia*)malloc(sizeof(Familia)),*fami;
    int empieza=0,empieza_fam=0;
    char caracteres[50],aux[50],papa[50]="",hijo[50]="",mama[50]="",apellido[50],famil_nomb[50];
    //~ creo las variables de nombre que ayudaran en el proceso
    archivo = fopen("Test.txt","r");
    //~ reviso si el arhivo fue cargado o salgo del programa
    if (archivo == NULL) exit(1);
    //~ en caso de entrar al programa empieza a leer el archivo
    while (feof(archivo) ==0) {
        fgets(caracteres,50,archivo);
        //~ Revisa si la linea corresponde al padre
        if(strncmp(caracteres,"Nombre de la familia",10)==0){
			fgets(caracteres,50,archivo);
			strcpy(famil_nomb,caracteres);
		}
        else if (strncmp(caracteres,"Padre: ",7)==0) {
			//~ Corta la cadena solo con el nombre
			Recortar(caracteres,papa,7,strlen(caracteres));
			//~ Imprime el nombre para corroborar
            printf("%s ",papa);
            //~ Busca en la lista si el nombre ya fue ingresado
            if(Buscar(papa,ref)!=NULL) {
                printf("\nFue ingresado\n");
                
            }
            else if(empieza==0){
				ref=Crear(NULL,papa,"","");
				printf("  Creando.....\n");
				//~ Le indico a pers la posicion actual de ref para empezar a trabajar con el
				pers=ref;
				empieza++;
				
			}
            else{
				pers->sig=Crear(ref,papa,"","");
				pers=pers->sig;
				printf("  Creando.....\n");
			}
            int i=0;
            while(papa[i]!=' ') i++;
            i++;
            Recortar(papa,apellido,i,strlen(papa));
        }
        else if (strncmp(caracteres,"Madre: ",7)==0) {
            Recortar(caracteres,mama,7,strlen(caracteres));
            printf("%s",mama);
            if(Buscar(mama,ref)!=NULL) {
                printf("  Fue ingresado\n");
            }
            else{
				pers->sig=Crear(ref,mama,"","");
				pers=pers->sig;
				printf("  Creando.....\n");
			}
			if(empieza_fam==0){
				ref_fam=Crear_familia(ref,famil_nomb,papa,mama);
				fami=ref_fam;
				empieza_fam++;
			}
			else{
				fami->sig=Crear_familia(ref,famil_nomb,papa,mama);
				fami=fami->sig;
			}
        }
        else if (strncmp(caracteres,"Hijos: ",7)==0) {
            Recortar(caracteres,hijo,7,strlen(caracteres));
            if(strncmp(caracteres,"Hijos: sin hijos",16)==0) { 
				int x=0;
				for(x=0;x<50;x++){
					apellido[x]='\0';
					papa[x]='\0';
					caracteres[x]='\0';
					aux[x]='\0';
					hijo[x]='\0';
					mama[x]='\0';
					famil_nomb[x]='\0';
				}
				
			} 
            else {
				int len=0,num=0,verificador=0,num_hijo=0;
				while(verificador==0){
					num=0;
					while(hijo[len]!=',' && hijo[len]!='\n'&& hijo[len]!='\r'&& hijo[len]!=' ' ){
						aux[num]=hijo[len];
						len++;
						num++;
					}
					aux[num]=' ';
					aux[num+1]='\0';
					len+=2;
					printf("%s%s",aux,apellido);
					if(aux[strlen(aux)-1]!=' '){
						strcat(aux," ");
					}
					strcat(aux,apellido);
					if(Buscar(aux,ref)!=NULL) {
						printf("  Fue ingresado\n");
						Reemplazar(ref,aux,papa,mama);
						fami->Hijos[num_hijo]=Buscar(aux,ref);
						num_hijo++;
					}			
					else{
						pers->sig=Crear(ref,aux,papa,mama);
						pers=pers->sig;
						printf(" Creando.....\n");
						fami->Hijos[num_hijo]=Buscar(aux,ref);
						num_hijo++;
						
					}
					if(hijo[len-2]=='\n'|| hijo[len-2]=='\r'|| hijo[len-2]==' ' ){
						verificador=1;
						int x=0;
						for(x=0;x<50;x++){
							apellido[x]='\0';
							papa[x]='\0';
							caracteres[x]='\0';
							aux[x]='\0';
							hijo[x]='\0';
							mama[x]='\0';
							famil_nomb[x]='\0';
						}
					}
					
				}
				
				
			}            
        }
        else{
			 printf("%s",caracteres);
		}
	
    }
	
    *personas = *ref;
    *familias = *ref_fam;
   
    fclose(archivo);
	}
/*Agrega las familias al cuado de familias */
void add_family_to_list(GtkWidget* widget,Familia* list){
	Familia *first = list;
	GtkTreeStore *store;
	store = GTK_TREE_STORE(gtk_tree_view_get_model     (GTK_TREE_VIEW(widget)));
	GtkTreeIter   iter;
	while(list != NULL){
		gtk_tree_store_append (store, &iter, NULL);
		gtk_tree_store_set (store, &iter,
					DATO, list->nomb_Familia,
					PUNTERO, list,
					-1);
		list = list->sig;
		}
	list = first;
	}
/*Agrega las familias al cuadro de personas */
void add_member_to_list(GtkWidget* widget,Persona* list){
	GtkTreeStore *store;
	store = GTK_TREE_STORE(gtk_tree_view_get_model     (GTK_TREE_VIEW(widget)));
	GtkTreeIter   iter;
	 
	Familia *first = list;
	while(list != NULL){
		gtk_tree_store_append (store, &iter, NULL);
		gtk_tree_store_set (store, &iter,
							DATO, list->nombre,
							PUNTERO, list,
							-1);
		list = list->sig;
		}
	list = first;
	}

Persona * Crear(Persona *ref,char *name,char *Pap,char *Mom){
	Persona *pers=(Persona*)malloc (sizeof(Persona));
	strcpy(pers->nombre,name);
	pers->Madre=Buscar(Mom,ref);
	pers->Padre=Buscar(Pap,ref);
	pers->sig=NULL;
	return pers;
}

Familia * Crear_familia(Persona *ref,char *name,char *Pap,char *Mom){
	Familia *pers=(Familia*)malloc (sizeof(Familia));
	strcpy(pers->nomb_Familia,name);
	pers->Madre=Buscar(Mom,ref);
	pers->Padre=Buscar(Pap,ref);
	pers->sig=NULL;
	return pers;
}

void Recortar(char string[],char substr[],int base,int len){
	int i=0;
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	while(base<len && string[base]!='\n' && string[base]!='\0'){
		printf("%c",string[base]);
		substr[i]=string[base];
		base++;
		i++;
	}
	substr[base+i]='\0';
	printf("\n--------%s",string);
	printf("\n........%s\n",substr);
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}

void Reemplazar(Persona *pers,char *name,char *Pap,char *Mom){
	Persona *a=pers;
	while(a!=NULL){
		
		if(strncmp(a->nombre,name,10)==0 && a->Padre==NULL){
			pers->Madre=Buscar(Mom,pers);
			pers->Padre=Buscar(Pap,pers);
		}
		a=a->sig;
	}
}

Persona* Buscar(char *personaje, Persona *prim) {
    if(prim==NULL) {
        return NULL;
    }
    else {
        Persona *a=NULL;
        a=prim;
        
        while(a->sig!=NULL) {
            if(strncmp(a->nombre,personaje,8)==0) {
                return a;
            }
            else {
                a=a->sig;
            }
        }
        if(strncmp(a->nombre,personaje,8)==0) {
            return a;
        }
        else {
            return NULL;
        }
    }
}
void config_window(GtkWindow *window,int width,int height,const gchar *title, gboolean resizable){
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (window), title);
	gtk_window_set_default_size(GTK_WINDOW(window), width, height);
	gtk_window_set_resizable (GTK_WINDOW(window), resizable);
	}



GtkWidget* setup_tree (const gchar *title){
	GtkWidget *tree;
	GtkTreeStore *store;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	/* Create a model.  We are using the store model for now, though we
	* could use any other GtkTreeModel */
	store = gtk_tree_store_new (N_COLUMNS,
							   G_TYPE_STRING,
							   G_TYPE_POINTER);

	/* Create a view */
	tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));

	/* The view now holds a reference.  We can get rid of our own
	* reference */
	g_object_unref (G_OBJECT (store));

	/* Create a cell render

	/* First column. family's names. */
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (title,
													  renderer,
													  "text", DATO,
													  NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

	/* Now we can manipulate the view just like any other GTK widget */
	return tree;
}
